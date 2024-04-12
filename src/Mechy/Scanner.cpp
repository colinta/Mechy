#include "Wiring.h"
#include "Scanner.h"

void Scanner::construct(Layout* _layout, const uint8_t* _pinRows, const uint8_t* _pinCols, uint8_t _ROWS, uint8_t _COLS) {
    layout = _layout;
    pinRows = _pinRows;
    pinCols = _pinCols;
    ROWS = _ROWS;
    COLS = _COLS;
    diodeDirection = COL_TO_ROW;
}

Scanner::Scanner(Layout* layout, const uint8_t* pinRows, const uint8_t* pinCols, uint8_t ROWS, uint8_t COLS) : Responder() {
    construct(layout, pinRows, pinCols, ROWS, COLS);
}

Scanner::Scanner(KBD* keys, const uint8_t* pinRows, const uint8_t* pinCols, uint8_t ROWS, uint8_t COLS) : Responder() {
    Layout* layout = new Layout(ROWS, COLS, keys);
    construct(layout, pinRows, pinCols, ROWS, COLS);
}

void Scanner::setDiodeDirection(uint8_t direction) {
    diodeDirection = direction;
}

void Scanner::begin() {
    for (uint8_t i = 0; i < COLS; i++) {
        uint8_t colPin = pinCols[i];
        if (diodeDirection == COL_TO_ROW) {
            Wiring::pinMode(colPin, INPUT_PULLUP);
        }
        else {
            Wiring::pinMode(colPin, OUTPUT);
            Wiring::digitalWrite(colPin, HIGH);
        }
    }

    for (uint8_t i = 0; i < ROWS; i++) {
        uint8_t rowPin = pinRows[i];
        if (diodeDirection == COL_TO_ROW) {
            Wiring::pinMode(rowPin, OUTPUT);
            Wiring::digitalWrite(rowPin, HIGH);
        }
        else {
            Wiring::pinMode(rowPin, INPUT_PULLUP);
        }
    }

    // keyboards are tricky things, and sometimes they freak out, so we need a
    // way to disable key events at startup. If any key is pressed at startup we
    // stay in this loop until all the keys are released.  That way if there's a
    // bug it's usually easy to re-flash.
    bool anyPressed = false;
    do {
        if (diodeDirection == COL_TO_ROW) {
            for (uint8_t row = 0; row < ROWS; row++) {
                Wiring::digitalWrite(pinRows[row], LOW);
                for (uint8_t col = 0; col < COLS; col++) {
                    anyPressed = !Wiring::digitalRead(pinCols[col]);
                    if (anyPressed)  break;
                }
                Wiring::digitalWrite(pinRows[row], HIGH);
                if (anyPressed)  break;
            }
        }
        else {
            for (uint8_t col = 0; col < COLS; col++) {
                Wiring::digitalWrite(pinCols[col], LOW);
                for (uint8_t row = 0; row < ROWS; row++) {
                    anyPressed = !Wiring::digitalRead(pinRows[row]);
                    if (anyPressed)  break;
                }
                Wiring::digitalWrite(pinCols[col], HIGH);
                if (anyPressed)  break;
            }
        }
    } while (anyPressed);
}

void Scanner::scan() {
    bool shouldBreak = false;
    if (diodeDirection == COL_TO_ROW) {
        for (uint8_t row = 0; row < ROWS; row++) {
            Wiring::digitalWrite(pinRows[row], LOW);
            for (uint8_t col = 0; col < COLS; col++) {
                bool isPressed = !Wiring::digitalRead(pinCols[col]);
                if (mechy->processKeyEvent(layout, row, col, isPressed) == KBD_HALT) {
                    shouldBreak = true;
                    break;
                }
            }
            Wiring::digitalWrite(pinRows[row], HIGH);
            if (shouldBreak) {
                break;
            }
        }
    }
    else {
        for (uint8_t col = 0; col < COLS; col++) {
            Wiring::digitalWrite(pinCols[col], LOW);
            for (uint8_t row = 0; row < ROWS; row++) {
                bool isPressed = !Wiring::digitalRead(pinRows[row]);
                if (mechy->processKeyEvent(layout, row, col, isPressed) == KBD_HALT) {
                    shouldBreak = true;
                    break;
                }
            }
            Wiring::digitalWrite(pinCols[col], HIGH);
            if (shouldBreak) {
                break;
            }
        }
    }

    if (shouldBreak) {
        delay(DEBOUNCE);
    }
    else {
        delay(5);
    }
}

void Scanner::gotoLayer(uint8_t layer) {
    layout->gotoLayer(layer);
}
