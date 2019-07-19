/*#  Worker sending to Supervisor
  #
Supervisor | scanning &               +--<--<--<--<--<--<--<--+
---------- | processing               |                       |
 reading-> |_______..._          ___  v  read     _read_      | __-> scan
           |        |  \        / | \   /  | \   /      \more?^|
           |        |   | <40us|  |  |  |  | |   | x 11 |      |
        -> |        |   [______]  |  \__/  | \___/      \______/
   ready-  |        |             | ready  | ready
        -> |________+___t0_____t1 |   /....+...\ /....\ /done_____-> scan
           | scan & |   |      \  |  /     |    X      X
 reading-> |queue|wait||wait|   \_+__......+.../ \..../ \more?v
---------- |     |for ||for |   send  ^                       |
Worker     |     |HIGH||LOW |   ACK   +--<--<--<--<--<--<--<--+
                                if data

  #  Supervisor sending to Worker
  #
Supervisor | scanning &               +--<--<--<--<--<--<--<--+
---------- | processing               |                       |
 reading-> |_______..._          ___  v                       |
           |        |  \        / | \......+...\ /....\ /done_|____-> scan
           |        |   | >40us|  |  \     |    X      X      |
        -> |        |   [______]  |   \....+.../ \..../ \more?^
   ready-  |        |             | ready  | ready
        -> |________+___t0_____t1 |   __   |  ___        _______-> scan
           | scan & |   |      \  |  |  |  | |   | x 8  |
 reading-> |queue|wait||wait|   \_+_/   \__|_/   \______/more?v
---------- |     |for ||for |   send  ^  read      read       |
Worker     |     |HIGH||LOW |   ACK   +--<--<--<--<--<--<--<--+
                                to init
*/

#include "Wiring.h"
#include "Transmitter.h"
#include "../priv/RxTx.h"
#include "../priv/Constants.h"

#define QUEUE_LEN 20
static uint16_t queue[QUEUE_LEN];
static uint8_t queueLength = 0;
static uint16_t timeout = 0;
static bool didTimeout = false;

Transmitter::Transmitter(uint8_t dataPin, uint8_t clockPin, const uint8_t* _pinRows, const uint8_t* _pinCols, uint8_t _ROWS, uint8_t _COLS) {
    outputPin = dataPin;
    inputPin = clockPin;
    pinRows = _pinRows;
    pinCols = _pinCols;
    ROWS = _ROWS;
    COLS = _COLS;
    keyPressed = (bool*)malloc(sizeof(bool) * (ROWS * COLS));
    bool* pressedPtr = keyPressed;
    for (uint8_t i = 0; i < ROWS * COLS; i++) {
        *pressedPtr = false;
        ++pressedPtr;
    }
    dataFnPtr = NULL;
    hasData = false;
}

void Transmitter::setDataFunc(void (*fnPtr)(uint8_t)) {
    dataFnPtr = fnPtr;
}

void Transmitter::begin() {
    Wiring::pinMode(outputPin, OUTPUT);
    Wiring::pinMode(inputPin, INPUT);
    Wiring::digitalWrite(outputPin, HIGH);

    for (uint8_t i = 0; i < COLS; i++) {
        uint8_t colPin = pinCols[i];
        Wiring::pinMode(colPin, INPUT_PULLUP);
    }

    for (uint8_t i = 0; i < ROWS; i++) {
        uint8_t rowPin = pinRows[i];
        Wiring::pinMode(rowPin, OUTPUT);
        Wiring::digitalWrite(rowPin, HIGH);
    }

    // keyboards are tricky things - if any key is pressed at startup we stay in this loop until
    // all the keys are released.  That way if there's a bug it's usually easy to re-flash.
    bool anyPressed = false;
    do {
        for (uint8_t row = 0; row < ROWS; row++) {
            Wiring::digitalWrite(pinRows[row], LOW);
            for (uint8_t col = 0; col < COLS; col++) {
                anyPressed = !Wiring::digitalRead(pinCols[col]);
                if (anyPressed)  break;
            }
            Wiring::digitalWrite(pinRows[row], HIGH);
            if (anyPressed)  break;
        }
    } while (anyPressed);
}

void Transmitter::scan() {
    bool anyChange = false;
    for (uint8_t row = 0; row < ROWS; row++) {
        Wiring::digitalWrite(pinRows[row], LOW);
        for (uint8_t col = 0; col < COLS; col++) {
            bool isPressed = !Wiring::digitalRead(pinCols[col]);
            anyChange = detectKeyChange(isPressed, row, col) || anyChange;
        }
        Wiring::digitalWrite(pinRows[row], HIGH);
    }

    flushQueue();

    if (hasData && dataFnPtr) {
        (*dataFnPtr)(data);
        hasData = false;
    }
}

// return true if wasPressed != isPressed, ie. change event
bool Transmitter::detectKeyChange(bool isPressed, uint8_t row, uint8_t col) {
    bool* wasPressed = keyPressed + (COLS * row) + col;
    if (*wasPressed == isPressed) { return false; }

    *wasPressed = isPressed;
    pushEvent(row, col, isPressed);
    return true;
}

void Transmitter::pushEvent(uint8_t row, uint8_t col, bool isPressed) {
    if (queueLength == QUEUE_LEN)  return;

    // 11 10 9 8 7 6   5 4 3 2 1
    // _   _________   _________
    // |   \  col  /   \  row  /
    // |    -------     -------
    // \--isPressed
    uint16_t bits = (isPressed ? 0b10000000000 : 0b00000000000);
    bits |= ((uint16_t)col) << 5;
    bits |= (uint16_t)row;

    queue[queueLength] = bits;
    queueLength++;
}

void Transmitter::flushQueue() {
    waitForReading();  // if the signal is low, wait for high
    waitForReady();    // wait for signal to go low
    unsigned long t0 = micros();
    waitForReading();
    unsigned long dt = micros() - t0;

    if (didTimeout) {
        didTimeout = false;
        return;
    }

    if (dt > TRANSMIT_DETECT) {
        receiveTransmission();
    }
    else if (queueLength > 0) {
        sendTransmission();
    }
    else {
        return;
    }

    sendReadyState();
}

void Transmitter::receiveTransmission() {
    sendReadingState();
    delayForTransmit();
    data = 0;
    hasData = true;
    for (uint8_t bitIndex = 0; bitIndex < NUM_TRANSMIT_BITS; bitIndex++) {
        if (!receiveOneBit())  continue;
        bit_on(data, bit(bitIndex));
    }
}

void Transmitter::sendTransmission() {
    sendHasData();  // transmitter goes low, indicating "data present"

    for (uint8_t queueIndex = 0; queueIndex < queueLength; queueIndex++) {
        uint16_t bits = queue[queueIndex];
        // transmitted as:
        // [row]         [col]        [isPressed]
        // 0 1 2 3 4 5   6 7 8 9 10   11
        for (uint8_t bitIndex = 0; bitIndex < NUM_LISTEN_BITS; bitIndex++) {
            sendOneBit((bits >> bitIndex) & 1);
        }

        bool noMoreData = (queueIndex == queueLength - 1);
        sendOneBit(noMoreData);
    }

    queueLength = 0;
}

inline void Transmitter::sendHasData() { Wiring::digitalWrite(outputPin, LOW); }

// supervisor: ready = LOW, reading = HIGH
inline bool Transmitter::supervisorIsReading()  { return Wiring::digitalRead(inputPin); }
inline bool Transmitter::supervisorIsReady()  { return !Wiring::digitalRead(inputPin); }
inline void Transmitter::waitForReady() {
    if (didTimeout)  return;
    timeout = 0;
    while (supervisorIsReading()) {
        if (timeout++ == 0xFFFF) {
            didTimeout = true;
            return;
        }
    }
}
inline void Transmitter::waitForReading() {
    if (didTimeout)  return;
    timeout = 0;
    while (supervisorIsReady()) {
        if (timeout++ == 0xFFFF) {
            didTimeout = true;
            return;
        }
    }
}
// worker: ready = HIGH, reading = LOW
inline void Transmitter::sendReadyState() { Wiring::digitalWrite(outputPin, HIGH); }
inline void Transmitter::sendReadingState() { Wiring::digitalWrite(outputPin, LOW); }

inline void Transmitter::sendOneBit(bool bit) {
    waitForReady();
    Wiring::digitalWrite(outputPin, bit);
    waitForReading();
}
inline bool Transmitter::receiveOneBit() {
    sendReadyState();
    delayForTransmit();
    sendReadingState();
    delayForTransmit();
    return Wiring::digitalRead(inputPin);
}
