#pragma once

#include "Mechy.h"

class Receiver: public Plugin {
public:
    Receiver(KBD *keys, uint8_t rows, uint8_t cols, uint8_t dataPin, uint8_t clockPin);
    void begin();
    void tick();

protected:
    uint8_t dataPin;
    uint8_t clockPin;
    uint8_t ROWS;
    uint8_t COLS;
    KBD *currentKey;
    KBD *keys;

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
};
