#pragma once

#include "Mechy.h"
#include "Responder.h"
#include "Layout.h"

// usually the diode points from the COL pin to the ROW pin, so this is the
// default.
#define COL_TO_ROW 0
// but reversed polarity is easy, call setDiodeDirection(ROW_TO_COL)
#define ROW_TO_COL 1

class Scanner : public Responder {
public:
    Scanner(Layout* layout, const uint8_t* pinRows, const uint8_t* pinCols, uint8_t ROWS, uint8_t COLS);
    Scanner(KBD* keys, const uint8_t* pinRows, const uint8_t* pinCols, uint8_t ROWS, uint8_t COLS);
    void begin();
    void scan();
    void setDiodeDirection(uint8_t direction);
    void gotoLayer(uint8_t layer);

protected:
    uint8_t ROWS;
    uint8_t COLS;
    uint8_t diodeDirection; // defaults to COL_TO_ROW
    Layout* layout;
    const uint8_t* pinRows;
    const uint8_t* pinCols;

private:
    void construct(Layout* _layout, const uint8_t* _pinRows, const uint8_t* _pinCols, uint8_t ROWS, uint8_t COLS);
};
