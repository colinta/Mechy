#include "Scanner.h"

Scanner::Scanner(KBD *_keys, const uint8_t *_pinRows, const uint8_t *_pinCols, uint8_t rows, uint8_t cols) {
    keys = _keys;
    pinRows = _pinRows;
    pinCols = _pinCols;
    ROWS = rows;
    COLS = cols;
}

void Scanner::begin(Mechy *_mechy) {
    for (uint8_t i = 0; i < COLS; i++) {
        uint8_t colPin = pinCols[i];
        pinMode(colPin, INPUT_PULLUP);
    }

    for (uint8_t i = 0; i < ROWS; i++) {
        uint8_t rowPin = pinRows[i];
        pinMode(rowPin, OUTPUT);
        digitalWrite(rowPin, HIGH);
    }

    mechy = _mechy;
    mechy->_begin();
}

void Scanner::scan() {
    delay(10);
    mechy->_tick();
    for (uint8_t row = 0; row < ROWS; row++) {
        digitalWrite(pinRows[row], LOW);
        for (uint8_t col = 0; col < COLS; col++) {
            bool isPressed = !digitalRead(pinCols[col]);
            currentKey = keys + (COLS * row) + col;
            mechy->processKeyEvent(isPressed, currentKey);
        }
        digitalWrite(pinRows[row], HIGH);
    }
}
