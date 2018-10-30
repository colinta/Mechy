#pragma once

#include "Mechy.h"

class Receiver: public Plugin {
public:
    Receiver(KBD *workerKeys, uint8_t rows, uint8_t cols, uint8_t sdaPin, uint8_t sclPin);
    void begin();
    void tick();
    void run(Event *event);
protected:
    uint8_t SDA;
    uint8_t SCL;
    uint8_t ROWS;
    uint8_t COLS;
    KBD *currentKey;
    KBD *workerKeys;

    void listen();
    void holdCheck();
    void awaitAck();
    bool getOneWorkerBit();
    void debounce();
    void delayForWorker();
    bool workerDidAck();
    bool workerHasData();
    void sendReadyState();
    void sendReadingState();
};
