#include "Receiver.h"

Receiver::Receiver(KBD *_keys, uint8_t rows, uint8_t cols, uint8_t _dataPin, uint8_t _clockPin) {
    keys = _keys;
    ROWS = rows;
    COLS = cols;
    dataPin = _dataPin;
    clockPin = _clockPin;
    firstKBDPtr = NULL;
}

inline void Receiver::appendKBDPtr(KBDDataPtr *ptr) {
    ptr->next = NULL;
    if (firstKBDPtr) {
        KBDDataPtr *lastPtr = firstKBDPtr;
        while (lastPtr->next) {
            lastPtr = lastPtr->next;
        }
        lastPtr->next = ptr;
    }
    else {
        firstKBDPtr = ptr;
    }
}

inline void Receiver::removeKBDPtr(KBDDataPtr *ptr) {
    if (!firstKBDPtr || firstKBDPtr == ptr) {
        firstKBDPtr = ptr->next;
        free(ptr);
        return;
    }

    KBDDataPtr *kbdPtr = firstKBDPtr;
    while (kbdPtr->next) {
        if (kbdPtr->next == ptr) {
            kbdPtr->next = ptr->next;
            break;
        }
        kbdPtr = kbdPtr->next;
    }
    free(ptr);
}

void Receiver::begin() {
    pinMode(dataPin, INPUT);
    pinMode(clockPin, OUTPUT);
    digitalWrite(clockPin, HIGH);
}

void Receiver::tick() {
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
    currentKey = keys + (COLS * row) + col;
    mechy->processKeyEvent(isPressed, currentKey);

    if (isPressed) {
        KBDDataPtr *ptr = (KBDDataPtr*)malloc(sizeof(KBDDataPtr));
        ptr->kbd = currentKey;
        ptr->isPressed = true;
        appendKBDPtr(ptr);
    }
    else {
        KBDDataPtr *findPtr = firstKBDPtr;
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
    KBDDataPtr *kbdData = firstKBDPtr;
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
    return digitalRead(dataPin);
}

void Receiver::debounce() { delayMicroseconds(10); }
void Receiver::delayForTransmitter() { delayMicroseconds(1000); }
bool Receiver::transmitterDidAck() { return digitalRead(dataPin); }
bool Receiver::transmitterHasData() { return !digitalRead(dataPin); }
void Receiver::sendReadyState() { digitalWrite(clockPin, LOW); }
void Receiver::sendReadingState() { digitalWrite(clockPin, HIGH); }
