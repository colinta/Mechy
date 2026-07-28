#include "Wiring.h"
#include "Receiver.h"
#include "priv/RxTx.h"
#include "../priv/Constants.h"
#include "../priv/Alloc.h"

static uint16_t timeout = 0;
static bool didTimeout = false;

void Receiver::construct(Layout* _layout, uint8_t dataPin, uint8_t clockPin) {
    layout = _layout;
    inputPin = dataPin;
    outputPin = clockPin;
    firstEventPtr = NULL;
    data = 0;
    hasData = false;
}

Receiver::Receiver(Layout* layout, uint8_t dataPin, uint8_t clockPin) : Responder() {
    construct(layout, dataPin, clockPin);
}

Receiver::Receiver(KBDPROG keys, uint8_t ROWS, uint8_t COLS, uint8_t dataPin, uint8_t clockPin) : Responder() {
    // the AVR core's global `new` does not null-check malloc before running
    // the constructor, so allocate explicitly (halting on failure) and
    // construct in place
    void* mem = mechyAllocOrHalt(sizeof(Layout), MECHY_HALT_LAYOUT);
    Layout* layout = new (MechyPlacement(), mem) Layout(ROWS, COLS, keys);
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
    hasData = true;
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
            mechyFree(ptr);
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
    if (hasData) {
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
            hasData = false;
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

    if (isPressed) {
        // track the key *before* processing the press: holdCheck() re-drives
        // the pressed state on every scan, so a tracked press automatically
        // retries any transient event-allocation failure in processKeyEvent
        bool isTracked = false;
        ReceiverEventPtr* findPtr = firstEventPtr;
        while (findPtr) {
            if (findPtr->matches(layout, row, col)) {
                isTracked = true;
                break;
            }
            findPtr = findPtr->next;
        }

        if (!isTracked) {
            ReceiverEventPtr* ptr = (ReceiverEventPtr*)mechyAlloc(sizeof(ReceiverEventPtr));
            if (ptr) {
                ptr->layout = layout;
                ptr->row = row;
                ptr->col = col;
                pushEventPtr(ptr);
                isTracked = true;
            }
        }

        // never emit a press that cannot be tracked through its release; if
        // allocation failed, this press packet is discarded (the matching
        // release packet will find no tracked key and is safely ignored)
        if (isTracked) {
            mechy->processKeyEvent(layout, row, col, true);
        }
    }
    else {
        ReceiverEventPtr* findPtr = firstEventPtr;
        while (findPtr) {
            if (findPtr->matches(layout, row, col)) {
                mechy->processKeyEvent(layout, row, col, false);
                removeEventPtr(findPtr);
                break;
            }
            findPtr = findPtr->next;
        }
        // an untracked release (its press was never tracked) is ignored
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
