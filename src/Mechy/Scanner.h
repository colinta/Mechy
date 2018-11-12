#pragma once

#include "Mechy.h"

class Scanner {
public:
    Scanner(KBD* keys, const uint8_t* pinRows, const uint8_t* pinCols, uint8_t rows, uint8_t cols);
    void begin(Mechy* mechy);
    void scan();

protected:
    uint8_t ROWS;
    uint8_t COLS;
    KBD* currentKey;
    KBD* keys;
    const uint8_t* pinRows;
    const uint8_t* pinCols;
    Mechy* mechy;
};
