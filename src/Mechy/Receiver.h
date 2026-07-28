#pragma once

#include "Mechy.h"
#include "Responder.h"
#include "Layout.h"

// maximum simultaneously held remote keys.  Override with a build flag
// (e.g. -DMECHY_MAX_RECEIVER_KEYS=12) if a configuration needs more.
#ifndef MECHY_MAX_RECEIVER_KEYS
#define MECHY_MAX_RECEIVER_KEYS 8
#endif

struct ReceiverKey {
    uint8_t row;
    uint8_t col;
};

class Receiver : public Responder {
public:
    Receiver(Layout* layout, uint8_t dataPin, uint8_t clockPin);
    Receiver(KBDPROG keys, uint8_t rows, uint8_t cols, uint8_t dataPin, uint8_t clockPin);
    void begin();
    void scan();
    void send(uint8_t byte);
    void gotoLayer(uint8_t layer);

protected:
    uint8_t inputPin;
    uint8_t outputPin;
    uint8_t data;
    bool hasData;
    Layout* layout;
    ReceiverKey trackedKeys[MECHY_MAX_RECEIVER_KEYS];
    uint8_t trackedKeyCount;

    void listen();
    void holdCheck();
    bool isTracked(uint8_t row, uint8_t col);
    bool trackKey(uint8_t row, uint8_t col);
    bool untrackKey(uint8_t row, uint8_t col);

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
