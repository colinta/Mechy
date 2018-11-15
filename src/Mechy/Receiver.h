#pragma once

#include "Mechy.h"
#include "Responder.h"
#include "Layout.h"

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
    EventPtr* firstEventPtr;

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

    inline void pushEventPtr(EventPtr* ptr);
    inline void removeEventPtr(EventPtr* ptr);

private:
    void construct(Layout* layout, uint8_t dataPin, uint8_t clockPin);
};
