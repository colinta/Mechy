#pragma once

#include <Arduino.h>
#include "../priv/Event.h"

struct LayerList {
    KBD* keys;
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
    Layout();
    Layout(KBD* layer1);
    Layout(KBD* layer1, KBD* layer2);
    Layout(KBD* layer1, KBD* layer2, KBD* layer3);
    Layout(KBD* layer1, KBD* layer2, KBD* layer3, KBD* layer4);

    void addLayer(KBD* layer);
    KBD* getKey(uint8_t row, uint8_t col, uint8_t ROWS, uint8_t COLS);
    void gotoLayer(uint8_t layerIndex);
    uint8_t layer();

protected:
    KBD* getInLayer(KBD* keys, uint8_t row, uint8_t col, uint8_t ROWS, uint8_t COLS);
    uint8_t currentLayerIndex;
    LayerList* firstLayerPtr;

private:
    void construct();
};
