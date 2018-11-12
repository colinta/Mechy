#include "Wiring.h"
#include "Scanner.h"

void Scanner::construct(Layout* _layout, const uint8_t* _pinRows, const uint8_t* _pinCols, uint8_t _ROWS, uint8_t _COLS) {
    layout = _layout;
    pinRows = _pinRows;
    pinCols = _pinCols;
    ROWS = _ROWS;
    COLS = _COLS;
}

Scanner::Scanner(Layout* layout, const uint8_t* pinRows, const uint8_t* pinCols, uint8_t rows, uint8_t cols) {
    construct(layout, pinRows, pinCols, rows, cols);
}

Scanner::Scanner(KBD* keys, const uint8_t* pinRows, const uint8_t* pinCols, uint8_t rows, uint8_t cols) {
    Layout* layout = new Layout(keys);
    construct(layout, pinRows, pinCols, rows, cols);
}

void Scanner::begin() {
    for (uint8_t i = 0; i < COLS; i++) {
        uint8_t colPin = pinCols[i];
        Wiring::pinMode(colPin, INPUT_PULLUP);
    }

    for (uint8_t i = 0; i < ROWS; i++) {
        uint8_t rowPin = pinRows[i];
        Wiring::pinMode(rowPin, OUTPUT);
        Wiring::digitalWrite(rowPin, HIGH);
    }
}

void Scanner::scan() {
    delay(1);
    for (uint8_t row = 0; row < ROWS; row++) {
        Wiring::digitalWrite(pinRows[row], LOW);
        for (uint8_t col = 0; col < COLS; col++) {
            bool isPressed = !Wiring::digitalRead(pinCols[col]);
            currentKey = layout->getKey(row, col, ROWS, COLS);
            mechy->processKeyEvent(isPressed, currentKey);
        }
        Wiring::digitalWrite(pinRows[row], HIGH);
    }
}
