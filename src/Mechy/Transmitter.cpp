#include <Arduino.h>

#include "Wiring.h"
#include "Transmitter.h"

#define QUEUE_LEN 20
byte queue[QUEUE_LEN];
uint8_t queuePtr = 0;


Transmitter::Transmitter(uint8_t _dataPin, uint8_t _clockPin, const uint8_t *_pinRows, const uint8_t *_pinCols, uint8_t rows, uint8_t cols) {
    dataPin = _dataPin;
    clockPin = _clockPin;
    pinRows = _pinRows;
    pinCols = _pinCols;
    ROWS = rows;
    COLS = cols;
    keyPressed = (bool*)malloc(sizeof(bool) * (rows * cols));
    bool *pressedPtr = keyPressed;
    for (uint8_t i = 0; i < ROWS * COLS; i++) {
        *pressedPtr = false;
        ++pressedPtr;
    }
}

void Transmitter::begin() {
    Wiring::pinMode(dataPin, OUTPUT);
    Wiring::pinMode(clockPin, INPUT);
    Wiring::digitalWrite(dataPin, HIGH);

    for (uint8_t i = 0; i < COLS; i++) {
        uint8_t colPin = pinCols[i];
        Wiring::pinMode(colPin, INPUT_PULLUP);
    }

    for (uint8_t i = 0; i < ROWS; i++) {
        uint8_t rowPin = pinRows[i];
        Wiring::pinMode(rowPin, OUTPUT);
        Wiring::digitalWrite(rowPin, HIGH);
    }
}

void Transmitter::scan() {
    bool anyChange = false;
    for (uint8_t row = 0; row < ROWS; row++) {
        Wiring::digitalWrite(pinRows[row], LOW);
        for (uint8_t col = 0; col < COLS; col++) {
            bool isPressed = !Wiring::digitalRead(pinCols[col]);
            anyChange = processKeyEvent(isPressed, row, col) || anyChange;
        }
        Wiring::digitalWrite(pinRows[row], HIGH);
    }

    flushQueue();
    // ug terrible debouncing
    if (anyChange) {
        delay(10);
    }
}

// return true if wasPressed != isPressed, ie. change event
bool Transmitter::processKeyEvent(bool isPressed, uint8_t row, uint8_t col) {
    bool *wasPressed = keyPressed + (COLS * row) + col;
    if (*wasPressed == isPressed) { return false; }

    *wasPressed = isPressed;
    pushEvent(row, col, isPressed);
    return true;
}

void Transmitter::pushEvent(uint8_t row, uint8_t col, bool isPressed) {
    if (queuePtr == QUEUE_LEN)  return;

    // 0   1 2 3 4   5 6 7
    // _   _______   _____
    // |   \ col /   \row/
    // |    -----     ---
    // \--isPressed
    byte bits = row | (col << 3) | (isPressed ? 0b10000000 : 0b00000000);
    queue[queuePtr] = bits;
    queuePtr++;

    sendHasData();
}

void Transmitter::flushQueue() {
    if (!supervisorIsReady()) { return; }

    if (queuePtr == 0) {
        return;
    }

    sendAckAndWait();

    for (uint8_t queueIndex = 0; queueIndex < queuePtr; queueIndex++) {
        byte bits = queue[queueIndex];
        // transmitted as:
        // [row]   [col]     [isPressed]
        // 7 6 5   4 3 2 1   0
        for (uint8_t bitIndex = 0; bitIndex < 8; bitIndex++) {
            sendOneBit((bits >> bitIndex) & 1);
        }

        waitForReady();
        if (queueIndex == queuePtr - 1) {
            sendNoData();
        }
        else {
            sendHasData();
        }
        waitForReading();
    }

    queuePtr = 0;
}

void Transmitter::sendOneBit(bool bit) {
    waitForReady();
    Wiring::digitalWrite(dataPin, bit);
    waitForReading();
}

void Transmitter::debounce()  { delayMicroseconds(100); }
bool Transmitter::supervisorIsReady()  { return !Wiring::digitalRead(clockPin); }
void Transmitter::waitForReady() {
    if (!supervisorIsReady())  while (!supervisorIsReady()) {};
    debounce();
}
void Transmitter::waitForReading() {
    if (supervisorIsReady())  while (supervisorIsReady()) {};
    debounce();
}
void Transmitter::sendHasData() { Wiring::digitalWrite(dataPin, LOW); }
void Transmitter::sendNoData() { Wiring::digitalWrite(dataPin, HIGH); }
void Transmitter::sendAckAndWait() {
    delayMicroseconds(1000);
    Wiring::digitalWrite(dataPin, HIGH);
    waitForReading();
}
