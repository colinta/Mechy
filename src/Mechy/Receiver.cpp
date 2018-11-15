#include "Wiring.h"
#include "Receiver.h"

#define NUM_BITS 11

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

void Receiver::listen() {
    if (!transmitterHasData()) { return; }

    awaitAck();

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
        EventPtr* ptr = (EventPtr*)malloc(sizeof(EventPtr));
        ptr->layout = layout;
        ptr->row = row;
        ptr->col = col;
        ptr->isPressed = true;
        pushEventPtr(ptr);
    }
    else {
        EventPtr* findPtr = firstEventPtr;
        while (findPtr) {
            if (findPtr->matches(layout, row, col)) {
                removeEventPtr(findPtr);
                break;
            }
            findPtr = findPtr->next;
        }
    }

    bool done = getOneTransmitterBit();
    if (!done) {
        goto listenBody;
    }
}

void Receiver::holdCheck() {
    EventPtr* kbdData = firstEventPtr;
    while (kbdData) {
        if (!kbdData->isPressed)  continue;
        mechy->processKeyEvent(layout, kbdData->row, kbdData->col, true);
        kbdData = kbdData->next;
    }
}

void Receiver::awaitAck() {
    sendReadyState();
    if (!transmitterDidAck())  while (!transmitterDidAck()) {};
    delayForTransmitter();
    sendReadingState();
    debounce();
}

bool Receiver::getOneTransmitterBit() {
    sendReadyState();
    delayForTransmitter();
    sendReadingState();
    debounce();
    return Wiring::digitalRead(dataPin);
}

inline void Receiver::pushEventPtr(EventPtr* ptr) {
    ptr->next = firstEventPtr;
    firstEventPtr = ptr;
}

inline void Receiver::removeEventPtr(EventPtr* ptr) {
    if (!firstEventPtr || firstEventPtr == ptr) {
        firstEventPtr = ptr->next;
        free(ptr);
        return;
    }

    EventPtr* kbdPtr = firstEventPtr;
    while (kbdPtr->next) {
        if (kbdPtr->next == ptr) {
            kbdPtr->next = ptr->next;
            break;
        }
        kbdPtr = kbdPtr->next;
    }
    free(ptr);
}

void Receiver::debounce() { delayMicroseconds(10); }
void Receiver::delayForTransmitter() { delayMicroseconds(1000); }
bool Receiver::transmitterDidAck() { return Wiring::digitalRead(dataPin); }
bool Receiver::transmitterHasData() { return !Wiring::digitalRead(dataPin); }
void Receiver::sendReadyState() { Wiring::digitalWrite(clockPin, LOW); }
void Receiver::sendReadingState() { Wiring::digitalWrite(clockPin, HIGH); }
