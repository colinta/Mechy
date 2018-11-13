#pragma once

#include "Mechy.h"
#include "Responder.h"

class Scanner : public Responder {
public:
    Scanner(KBD* keys, const uint8_t* pinRows, const uint8_t* pinCols, uint8_t rows, uint8_t cols);
    void begin();
    void scan();

protected:
    uint8_t ROWS;
    uint8_t COLS;
    KBD* currentKey;
    KBD* keys;
    const uint8_t* pinRows;
    const uint8_t* pinCols;
};
