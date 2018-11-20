#include "Layout.h"

void Layout::construct(uint8_t _ROWS, uint8_t _COLS) {
    ROWS = _ROWS;
    COLS = _COLS;
    currentLayerIndex = 0;
    firstLayerPtr = NULL;
}

Layout::Layout(uint8_t ROWS, uint8_t COLS) {
    construct(ROWS, COLS);
}

Layout::Layout(uint8_t ROWS, uint8_t COLS, KBDPROG layer1) {
    construct(ROWS, COLS);
    addLayer(layer1);
}

Layout::Layout(uint8_t ROWS, uint8_t COLS, KBDPROG layer1, KBDPROG layer2) {
    construct(ROWS, COLS);
    addLayer(layer1);
    addLayer(layer2);
}

Layout::Layout(uint8_t ROWS, uint8_t COLS, KBDPROG layer1, KBDPROG layer2, KBDPROG layer3) {
    construct(ROWS, COLS);
    addLayer(layer1);
    addLayer(layer2);
    addLayer(layer3);
}

Layout::Layout(uint8_t ROWS, uint8_t COLS, KBDPROG layer1, KBDPROG layer2, KBDPROG layer3, KBDPROG layer4) {
    construct(ROWS, COLS);
    addLayer(layer1);
    addLayer(layer2);
    addLayer(layer3);
    addLayer(layer4);
}

void Layout::addLayer(KBDPROG keys) {
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

KBDPROG Layout::getKey(uint8_t row, uint8_t col) {
    if (!firstLayerPtr)  return NULL;

    if (currentLayerIndex == 0) {
        return this->getInLayer(firstLayerPtr->keys, row, col);
    }

    uint8_t layerIndex = currentLayerIndex;
    LayerList* layerPtr = firstLayerPtr;
    while (layerPtr && layerIndex > 0) {
        layerPtr = layerPtr->next;
        layerIndex -= 1;
    }
    if (!layerPtr)  return NULL;

    KBDPROG kbd = this->getInLayer(layerPtr->keys, row, col);
    if (!kbd)  return NULL;

    if (kbd->getName() == FN_NONE && kbd->getKey() == MCHY_TRANS) {
        return this->getInLayer(firstLayerPtr->keys, row, col);
    }
    return kbd;
}

KBDPROG Layout::getInLayer(KBDPROG keys, uint8_t row, uint8_t col) {
    if (row >= ROWS || col >= COLS)  return NULL;
    return keys + (COLS * row) + col;
}
