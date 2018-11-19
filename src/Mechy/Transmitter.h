#pragma once

#include "../priv/RxTx.h"

class Transmitter {
public:
    Transmitter(uint8_t dataPin, uint8_t clockPin, const uint8_t* pinRows, const uint8_t* pinCols, uint8_t ROWS, uint8_t COLS);
    void begin();
    void scan();
protected:
    uint8_t dataPin;
    uint8_t clockPin;
    const uint8_t* pinRows;
    const uint8_t* pinCols;
    uint8_t ROWS;
    uint8_t COLS;
    bool* keyPressed;

    bool detectKeyChange(bool isPressed, uint8_t row, uint8_t col);
    void pushEvent(uint8_t row, uint8_t col, bool isPressed);
    void flushQueue();
    void receiveTransmission();
    void sendTransmission();
    inline void sendHasData();
    inline void sendOneBit(bool bit);
    inline bool receiveOneBit();
    inline bool supervisorIsReading();
    inline bool supervisorIsReady();
    inline void waitForReady();
    inline void waitForReading();
};
