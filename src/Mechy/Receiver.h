#pragma once

#include "Mechy.h"
#include "Responder.h"
#include "Layout.h"

struct ReceiverEventPtr {
    Layout* layout;
    uint8_t row;
    uint8_t col;
    ReceiverEventPtr* next;

    bool matches(Layout* layout, uint8_t row, uint8_t col);
};

class Receiver : public Responder {
public:
    Receiver(Layout* layout, uint8_t dataPin, uint8_t clockPin);
    Receiver(KBD* keys, uint8_t ROWS, uint8_t COLS, uint8_t dataPin, uint8_t clockPin);
    void begin();
    void scan();
    void gotoLayer(uint8_t layer);

protected:
    uint8_t dataPin;
    uint8_t clockPin;
    Layout* layout;
    ReceiverEventPtr* firstEventPtr;

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

    inline void pushEventPtr(ReceiverEventPtr* ptr);
    inline void removeEventPtr(ReceiverEventPtr* ptr);

private:
    void construct(Layout* layout, uint8_t dataPin, uint8_t clockPin);
};
