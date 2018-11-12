#include "Layout.h"

void Layout::construct() {
    currentLayerIndex = 0;
    firstLayerPtr = NULL;
}

Layout::Layout() {
    construct();
}

Layout::Layout(KBD* layer1) {
    construct();
    addLayer(layer1);
}

Layout::Layout(KBD* layer1, KBD* layer2) {
    construct();
    addLayer(layer1);
    addLayer(layer2);
}

Layout::Layout(KBD* layer1, KBD* layer2, KBD* layer3) {
    construct();
    addLayer(layer1);
    addLayer(layer2);
    addLayer(layer3);
}

Layout::Layout(KBD* layer1, KBD* layer2, KBD* layer3, KBD* layer4) {
    construct();
    addLayer(layer1);
    addLayer(layer2);
    addLayer(layer3);
    addLayer(layer4);
}

void Layout::addLayer(KBD* keys) {
    LayerList* ptr = (LayerList*)malloc(sizeof(LayerList));
    ptr->keys = keys;
    ptr->next = NULL;

    if (!firstLayerPtr) {
        firstLayerPtr = ptr;
    }
    else {
        LayerList* lastLayerPtr = firstLayerPtr;
        while (lastLayerPtr->next) {
            lastLayerPtr = lastLayerPtr->next;
        }
        lastLayerPtr->next = ptr;
    }
}

void Layout::gotoLayer(uint8_t layerIndex) {
    currentLayerIndex = layerIndex;
}

uint8_t Layout::layer() {
    return currentLayerIndex;
}

KBD* Layout::getKey(uint8_t row, uint8_t col, uint8_t ROWS, uint8_t COLS) {
    if (!firstLayerPtr)  return NULL;

    if (currentLayerIndex == 0) {
        return this->getInLayer(firstLayerPtr->keys, row, col, ROWS, COLS);
    }

    uint8_t layerIndex = currentLayerIndex;
    LayerList* layerPtr = firstLayerPtr;
    while (layerPtr && layerIndex > 0) {
        layerPtr = layerPtr->next;
        layerIndex -= 1;
    }
    if (!layerPtr)  return NULL;

    KBD* kbd = this->getInLayer(layerPtr->keys, row, col, ROWS, COLS);
    if (!kbd)  return NULL;

    if (kbd->name == FN_NONE && kbd->key == MCHY_TRANS) {
        return this->getInLayer(firstLayerPtr->keys, row, col, ROWS, COLS);
    }
    return kbd;
}

KBD* Layout::getInLayer(KBD* keys, uint8_t row, uint8_t col, uint8_t ROWS, uint8_t COLS) {
    if (row >= ROWS || col >= COLS)  return NULL;
    return keys + (COLS * row) + col;
}
