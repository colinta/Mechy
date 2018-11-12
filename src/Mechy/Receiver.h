#pragma once

#include "Mechy.h"
#include "Responder.h"
#include "Layout.h"

class Receiver : public Responder {
public:
    Receiver(Layout* layout, uint8_t ROWS, uint8_t COLS, uint8_t dataPin, uint8_t clockPin);
    Receiver(KBD* keys, uint8_t ROWS, uint8_t COLS, uint8_t dataPin, uint8_t clockPin);
    void begin();
    void scan();

protected:
    uint8_t dataPin;
    uint8_t clockPin;
    uint8_t ROWS;
    uint8_t COLS;
    Layout* layout;
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

private:
    void construct(Layout* layout, uint8_t ROWS, uint8_t COLS, uint8_t dataPin, uint8_t clockPin);
};
