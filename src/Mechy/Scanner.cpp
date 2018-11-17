#include "Wiring.h"
#include "Scanner.h"

void Scanner::construct(Layout* _layout, const uint8_t* _pinRows, const uint8_t* _pinCols, uint8_t _ROWS, uint8_t _COLS) {
    layout = _layout;
    pinRows = _pinRows;
    pinCols = _pinCols;
    ROWS = _ROWS;
    COLS = _COLS;
}

Scanner::Scanner(Layout* layout, const uint8_t* pinRows, const uint8_t* pinCols, uint8_t ROWS, uint8_t COLS) : Responder() {
    construct(layout, pinRows, pinCols, ROWS, COLS);
}

Scanner::Scanner(KBD* keys, const uint8_t* pinRows, const uint8_t* pinCols, uint8_t ROWS, uint8_t COLS) : Responder() {
    Layout* layout = new Layout(ROWS, COLS, keys);
    construct(layout, pinRows, pinCols, ROWS, COLS);
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

    // keyboards are tricky things - if any key is pressed at startup we stay in this loop until
    // all the keys are released.  That way if there's a bug it's usually easy to re-flash.
    bool anyPressed = false;
    do {
        for (uint8_t row = 0; row < ROWS; row++) {
            Wiring::digitalWrite(pinRows[row], LOW);
            for (uint8_t col = 0; col < COLS; col++) {
                anyPressed = !Wiring::digitalRead(pinCols[col]);
                if (anyPressed)  break;
            }
            Wiring::digitalWrite(pinRows[row], HIGH);
            if (anyPressed)  break;
        }
    } while (anyPressed);
}

void Scanner::scan() {
    delay(1);
    for (uint8_t row = 0; row < ROWS; row++) {
        Wiring::digitalWrite(pinRows[row], LOW);
        for (uint8_t col = 0; col < COLS; col++) {
            bool isPressed = !Wiring::digitalRead(pinCols[col]);
            mechy->processKeyEvent(layout, row, col, isPressed);
        }
        Wiring::digitalWrite(pinRows[row], HIGH);
    }
}

void Scanner::gotoLayer(uint8_t layer) {
    layout->gotoLayer(layer);
}
