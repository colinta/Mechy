#pragma once

#include <Arduino.h>
#include "../priv/Event.h"

struct LayerList {
    KBDPROG keys;
    LayerList* next;
};

struct KeyboardList {
    uint8_t row, col;
    uint8_t name;
    uint16_t key;
    unsigned long started;
    KeyboardList* next;
};

class Layout {
public:
    Layout(uint8_t ROWS, uint8_t COLS);
    Layout(uint8_t ROWS, uint8_t COLS, KBDPROG layer1);
    Layout(uint8_t ROWS, uint8_t COLS, KBDPROG layer1, KBDPROG layer2);
    Layout(uint8_t ROWS, uint8_t COLS, KBDPROG layer1, KBDPROG layer2, KBDPROG layer3);
    Layout(uint8_t ROWS, uint8_t COLS, KBDPROG layer1, KBDPROG layer2, KBDPROG layer3, KBDPROG layer4);
    Layout(uint8_t ROWS, uint8_t COLS, KBDPROG layer1, KBDPROG layer2, KBDPROG layer3, KBDPROG layer4, KBDPROG layer5);

    void addLayer(KBDPROG layer);
    KBDPROG getKey(uint8_t row, uint8_t col);
    void gotoLayer(uint8_t layerIndex);
    uint8_t layer();

protected:
    uint8_t ROWS;
    uint8_t COLS;
    uint8_t currentLayerIndex;
    LayerList* firstLayerPtr;
    KBDPROG getInLayer(KBDPROG keys, uint8_t row, uint8_t col);

private:
    void construct(uint8_t ROWS, uint8_t COLS);
};
