#include "Wiring.h"
#include "Receiver.h"
#include "priv/RxTx.h"
#include "../priv/Constants.h"

static uint16_t timeout = 0;
static bool didTimeout = false;

void Receiver::construct(Layout* _layout, uint8_t dataPin, uint8_t clockPin) {
    layout = _layout;
    inputPin = dataPin;
    outputPin = clockPin;
    firstEventPtr = NULL;
    data = 0;
}

Receiver::Receiver(Layout* layout, uint8_t dataPin, uint8_t clockPin) : Responder() {
    construct(layout, dataPin, clockPin);
}

Receiver::Receiver(KBD* keys, uint8_t ROWS, uint8_t COLS, uint8_t dataPin, uint8_t clockPin) : Responder() {
    Layout* layout = new Layout(ROWS, COLS, keys);
    construct(layout, dataPin, clockPin);
}

void Receiver::begin() {
    Wiring::pinMode(inputPin, INPUT);
    Wiring::pinMode(outputPin, OUTPUT);
    Wiring::digitalWrite(outputPin, HIGH);
}

void Receiver::scan() {
    listen();
    holdCheck();
}

void Receiver::send(uint8_t _data) {
    data = _data;
}

void Receiver::gotoLayer(uint8_t layer) {
    layout->gotoLayer(layer);
}

void Receiver::holdCheck() {
    ReceiverEventPtr* kbdData = firstEventPtr;
    while (kbdData) {
        mechy->processKeyEvent(layout, kbdData->row, kbdData->col, true);
        kbdData = kbdData->next;
    }
}

inline void Receiver::pushEventPtr(ReceiverEventPtr* ptr) {
    ptr->next = firstEventPtr;
    firstEventPtr = ptr;
}

inline void Receiver::removeEventPtr(ReceiverEventPtr* ptr) {
    ReceiverEventPtr** eventPtrPtr = &firstEventPtr;
    ReceiverEventPtr* eventPtr = firstEventPtr;
    while (eventPtr) {
        if (eventPtr == ptr) {
            *eventPtrPtr = ptr->next;
            free(ptr);
            return;
        }
        eventPtrPtr = &(eventPtr->next);
        eventPtr = eventPtr->next;
    }
}

bool ReceiverEventPtr::matches(Layout* layout, uint8_t row, uint8_t col) {
    return this->layout == layout && this->row == row && this->col == col;
}

void Receiver::listen() {
    while (transmitterHasData());
    // send "ready to listen" or "ready to transmit" signal
    sendReadyState();
    if (data) {
        delayMicroseconds(TRANSMIT_TIME);
        sendReadingState();
        delayForTransmit();
        uint16_t timeout = 0xFFFF;
        while (workerIsReady()) { if (--timeout == 0) { return; }}

        for (uint8_t bitIndex = 0; bitIndex < NUM_TRANSMIT_BITS; bitIndex++) {
            sendOneBit(bit_get(data, bit(bitIndex)));
        }
        waitForReading();
        if (didTimeout) {
            didTimeout = false;
        }
        else {
            data = 0;
        }
        return;
    }
    else {
        delayMicroseconds(LISTEN_TIME);
        sendReadingState();
        delayForTransmit();
        bool workerDidAck = transmitterHasData();
        if (!workerDidAck)  return;
        goto listenBody;
    }

listenBody:
    uint16_t input = 0;
    for (uint8_t i = 0; i < NUM_LISTEN_BITS; i++) {
        bool oneBit = receiveOneBit();
        if (oneBit) {
            input |= 1 << i;
        }
    }

    uint8_t row = input & 0b11111;
    uint8_t col = (input >> 5) & 0b11111;
    bool isPressed = !!(input >> 10);
    mechy->processKeyEvent(layout, row, col, isPressed);

    if (isPressed) {
        ReceiverEventPtr* ptr = (ReceiverEventPtr*)malloc(sizeof(ReceiverEventPtr));
        ptr->layout = layout;
        ptr->row = row;
        ptr->col = col;
        pushEventPtr(ptr);
    }
    else {
        ReceiverEventPtr* findPtr = firstEventPtr;
        while (findPtr) {
            if (findPtr->matches(layout, row, col)) {
                removeEventPtr(findPtr);
                break;
            }
            findPtr = findPtr->next;
        }
    }

    bool hasMoreData = !receiveOneBit();
    if (hasMoreData) {
        goto listenBody;
    }
}

inline bool Receiver::transmitterHasData() { return !Wiring::digitalRead(inputPin); }
inline void Receiver::sendHasData() { Wiring::digitalWrite(outputPin, LOW); }

// worker: ready = HIGH, reading = LOW
inline bool Receiver::workerIsReading()  { return !Wiring::digitalRead(inputPin); }
inline bool Receiver::workerIsReady()  { return Wiring::digitalRead(inputPin); }
inline void Receiver::waitForReady() {
    if (didTimeout)  return;
    timeout = 0;
    while (workerIsReading()) {
        if (timeout++ == 0xFFFF) {
            didTimeout = true;
            return;
        }
    }
}
inline void Receiver::waitForReading() {
    if (didTimeout)  return;
    timeout = 0;
    while (workerIsReady()) {
        if (timeout++ == 0xFFFF) {
            didTimeout = true;
            return;
        }
    }
}
// supervisor: ready = LOW, reading = HIGH
inline void Receiver::sendReadyState() { Wiring::digitalWrite(outputPin, LOW); }
inline void Receiver::sendReadingState() { Wiring::digitalWrite(outputPin, HIGH); }

inline void Receiver::sendOneBit(bool bit) {
    waitForReady();
    Wiring::digitalWrite(outputPin, bit);
    waitForReading();
}
inline bool Receiver::receiveOneBit() {
    sendReadyState();
    delayForTransmit();
    sendReadingState();
    delayForTransmit();
    return Wiring::digitalRead(inputPin);
}
