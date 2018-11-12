#pragma once

#include "Mechy.h"

class Receiver {
public:
    Receiver(KBD* keys, uint8_t rows, uint8_t cols, uint8_t dataPin, uint8_t clockPin);
    void begin(Mechy* mechy);
    void scan();

protected:
    uint8_t dataPin;
    uint8_t clockPin;
    uint8_t ROWS;
    uint8_t COLS;
    KBD* currentKey;
    KBD* keys;
    KBDDataPtr* firstKBDPtr;
    Mechy* mechy;

    void listen();
    void holdCheck();
    void awaitAck();
    bool getOneTransmitterBit();
    void debounce();
    void delayForTransmitter();
    bool transmitterDidAck();
    bool transmitterHasData();
    void sendReadyState();
    void sendReadingState();

    inline void pushKBDPtr(KBDDataPtr* ptr);
    inline void removeKBDPtr(KBDDataPtr* ptr);
};
