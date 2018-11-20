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
    void send(uint8_t byte);
    void gotoLayer(uint8_t layer);

protected:
    uint8_t inputPin;
    uint8_t outputPin;
    uint8_t data;
    Layout* layout;
    ReceiverEventPtr* firstEventPtr;

    void listen();
    void holdCheck();
    inline void pushEventPtr(ReceiverEventPtr* ptr);
    inline void removeEventPtr(ReceiverEventPtr* ptr);

    inline bool transmitterHasData();
    inline void sendHasData();
    inline bool workerIsReading();
    inline bool workerIsReady();
    inline void sendReadyState();
    inline void sendReadingState();
    inline void waitForReady();
    inline void waitForReading();
    inline void sendOneBit(bool);
    inline bool receiveOneBit();

private:
    void construct(Layout* layout, uint8_t dataPin, uint8_t clockPin);
};
