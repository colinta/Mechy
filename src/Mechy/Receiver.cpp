#include "Wiring.h"
#include "Receiver.h"

void Receiver::construct(Layout* _layout, uint8_t _ROWS, uint8_t _COLS, uint8_t _dataPin, uint8_t _clockPin) {
    layout = _layout;
    ROWS = _ROWS;
    COLS = _COLS;
    dataPin = _dataPin;
    clockPin = _clockPin;
    firstKBDPtr = NULL;
}

Receiver::Receiver(Layout* layout, uint8_t rows, uint8_t cols, uint8_t dataPin, uint8_t clockPin) {
    construct(layout, rows, cols, dataPin, clockPin);
}

Receiver::Receiver(KBD* keys, uint8_t rows, uint8_t cols, uint8_t dataPin, uint8_t clockPin) {
    Layout* layout = new Layout(keys);
    construct(layout, rows, cols, dataPin, clockPin);
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

void Receiver::listen() {
    if (!transmitterHasData()) { return; }

    awaitAck();

listenBody:
    byte input = 0;
    for (uint8_t i = 0; i < 8; i++) {
        bool oneBit = getOneTransmitterBit();
        if (oneBit) {
            input |= 1 << i;
        }
    }

    uint8_t row = input & 0b111;
    uint8_t col = (input >> 3) & 0b1111;
    bool isPressed = !!(input >> 7);
    currentKey = layout->getKey(row, col, ROWS, COLS);
    mechy->processKeyEvent(isPressed, currentKey);

    if (isPressed) {
        KBDDataPtr* ptr = (KBDDataPtr*)malloc(sizeof(KBDDataPtr));
        ptr->kbd = currentKey;
        ptr->isPressed = true;
        pushKBDPtr(ptr);
    }
    else {
        KBDDataPtr* findPtr = firstKBDPtr;
        while (findPtr) {
            if (findPtr->kbd == currentKey) {
                removeKBDPtr(findPtr);
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
    KBDDataPtr* kbdData = firstKBDPtr;
    while (kbdData) {
        if (!kbdData->isPressed)  continue;
        mechy->processKeyEvent(true, kbdData->kbd);
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

inline void Receiver::pushKBDPtr(KBDDataPtr* ptr) {
    ptr->next = firstKBDPtr;
    firstKBDPtr = ptr;
}

inline void Receiver::removeKBDPtr(KBDDataPtr* ptr) {
    if (!firstKBDPtr || firstKBDPtr == ptr) {
        firstKBDPtr = ptr->next;
        free(ptr);
        return;
    }

    KBDDataPtr* kbdPtr = firstKBDPtr;
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
