#pragma once

#include "Mechy.h"
#include "Responder.h"
#include "Layout.h"

class Scanner : public Responder {
public:
    Scanner(Layout* layout, const uint8_t* pinRows, const uint8_t* pinCols, uint8_t ROWS, uint8_t COLS);
    Scanner(KBD* keys, const uint8_t* pinRows, const uint8_t* pinCols, uint8_t ROWS, uint8_t COLS);
    void begin();
    void scan();
    void gotoLayer(uint8_t layer);
    void usePullup(bool shouldUsePullup);

protected:
    uint8_t ROWS;
    uint8_t COLS;
    Layout* layout;
    const uint8_t* pinRows;
    const uint8_t* pinCols;
    bool shouldUsePullup;

private:
    void construct(Layout* _layout, const uint8_t* _pinRows, const uint8_t* _pinCols, uint8_t ROWS, uint8_t COLS);
};
