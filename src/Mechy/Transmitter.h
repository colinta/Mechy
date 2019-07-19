#pragma once

class Transmitter {
public:
    Transmitter(uint8_t dataPin, uint8_t clockPin, const uint8_t* pinRows, const uint8_t* pinCols, uint8_t ROWS, uint8_t COLS);
    void begin();
    void scan();
    void setDataFunc(void (*fnPtr)(uint8_t));
protected:
    bool hasData;
    uint8_t data;
    uint8_t inputPin;
    uint8_t outputPin;
    const uint8_t* pinRows;
    const uint8_t* pinCols;
    uint8_t ROWS;
    uint8_t COLS;
    bool* keyPressed;
    void (*dataFnPtr)(uint8_t);

    bool detectKeyChange(bool isPressed, uint8_t row, uint8_t col);
    void pushEvent(uint8_t row, uint8_t col, bool isPressed);
    void flushQueue();
    void receiveTransmission();
    void sendTransmission();

    inline void sendHasData();
    inline bool supervisorIsReading();
    inline bool supervisorIsReady();
    inline void waitForReady();
    inline void waitForReading();
    inline void sendReadyState();
    inline void sendReadingState();
    inline void sendOneBit(bool bit);
    inline bool receiveOneBit();
};
