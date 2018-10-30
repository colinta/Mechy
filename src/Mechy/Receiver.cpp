#include "Receiver.h"

Receiver::Receiver(KBD *_workerKeys, uint8_t rows, uint8_t cols, uint8_t sdaPin, uint8_t sclPin) {
    workerKeys = _workerKeys;
    ROWS = rows;
    COLS = cols;
    SDA = sdaPin;
    SCL = sclPin;
}

void Receiver::begin() {
  pinMode(SDA, INPUT);
  pinMode(SCL, OUTPUT);
  digitalWrite(SCL, HIGH);
}

void Receiver::tick() {
    listen();
    holdCheck();
}

void Receiver::run(Event *event) {
}

void Receiver::listen() {
    if (!workerHasData()) { return; }

    awaitAck();

listenBody:
    byte input = 0;
    for (uint8_t i = 0; i < 8; i++) {
        bool oneBit = getOneWorkerBit();
        if (oneBit) {
            input |= 1 << i;
        }
    }

    uint8_t row = input & 0b111;
    uint8_t col = (input >> 3) & 0b1111;
    bool isPressed = !!(input >> 7);
    currentKey = workerKeys + (COLS * row) + col;
    mechy->processKeyEvent(isPressed, currentKey);

    bool done = getOneWorkerBit();
    if (!done) {
        goto listenBody;
    }
}

void Receiver::holdCheck() {
    for (uint8_t row = 0; row < ROWS; row++) {
        for (uint8_t col = 0; col < COLS; col++) {
            currentKey = workerKeys + (COLS * row) + col;
            if (currentKey->isPressed) {
                mechy->processKeyEvent(true, currentKey);
            }
        }
    }
}

void Receiver::awaitAck() {
    sendReadyState();
    if (!workerDidAck())  while (!workerDidAck()) {};
    delayForWorker();
    sendReadingState();
    debounce();
}

bool Receiver::getOneWorkerBit() {
    sendReadyState();
    delayForWorker();
    sendReadingState();
    debounce();
    return digitalRead(SDA);
}

void Receiver::debounce() { delayMicroseconds(10); }
void Receiver::delayForWorker() { delayMicroseconds(1000); }
bool Receiver::workerDidAck() { return digitalRead(SDA); }
bool Receiver::workerHasData() { return !digitalRead(SDA); }
void Receiver::sendReadyState() { digitalWrite(SCL, LOW); }
void Receiver::sendReadingState() { digitalWrite(SCL, HIGH); }
