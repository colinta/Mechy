#include "Wiring.h"
#include "Receiver.h"

void Receiver::construct(Layout* _layout, uint8_t _dataPin, uint8_t _clockPin) {
    layout = _layout;
    dataPin = _dataPin;
    clockPin = _clockPin;
    firstEventPtr = NULL;
}

Receiver::Receiver(Layout* layout, uint8_t dataPin, uint8_t clockPin) : Responder() {
    construct(layout, dataPin, clockPin);
}

Receiver::Receiver(KBD* keys, uint8_t ROWS, uint8_t COLS, uint8_t dataPin, uint8_t clockPin) : Responder() {
    Layout* layout = new Layout(ROWS, COLS, keys);
    construct(layout, dataPin, clockPin);
}

void Receiver::begin() {
    Wiring::pinMode(dataPin, INPUT);
    Wiring::pinMode(clockPin, OUTPUT);
    Wiring::digitalWrite(clockPin, HIGH);
}

void Receiver::scan() {
    listen();
    holdCheck();
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
    // if (hasDataToSend) { delayMicroseconds(TRANSMIT_TIME); }
    // else
    delayMicroseconds(LISTEN_TIME);
    sendReadingState();
    delayForTransmitter();
    bool workerDidAck = transmitterHasData();
    if (!workerDidAck)  return;

listenBody:
    uint16_t input = 0;
    for (uint8_t i = 0; i < NUM_BITS; i++) {
        bool oneBit = getOneTransmitterBit();
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

    bool hasMoreData = !getOneTransmitterBit();
    if (hasMoreData) {
        goto listenBody;
    }
}

inline void Receiver::delayForTransmitter() { delayMicroseconds(TX_LISTEN_TIME); }
inline bool Receiver::transmitterHasData() { return !Wiring::digitalRead(dataPin); }
inline void Receiver::sendReadyState() { Wiring::digitalWrite(clockPin, LOW); }
inline void Receiver::sendReadingState() { Wiring::digitalWrite(clockPin, HIGH); }

inline bool Receiver::getOneTransmitterBit() {
    sendReadyState();
    delayForTransmitter();
    sendReadingState();
    delayForTransmitter();
    return Wiring::digitalRead(dataPin);
}
