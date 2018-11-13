#pragma once

#include "Mechy.h"
#include "Responder.h"

class Receiver : public Responder {
public:
    Receiver(KBD* keys, uint8_t ROWS, uint8_t COLS, uint8_t dataPin, uint8_t clockPin);
    void begin();
    void scan();

protected:
    uint8_t dataPin;
    uint8_t clockPin;
    uint8_t ROWS;
    uint8_t COLS;
    KBD* currentKey;
    KBD* keys;
    KBDDataPtr* firstKBDPtr;

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
