#include "GotoLayer.h"

GotoLayer::GotoLayer() {
}

uint8_t GotoLayer::defaultName() {
    return FN_GOTO_LAYER;
}

void GotoLayer::run(Event* event) {
    if (event->isPressed()) {
        mechy->pushLayer(1);
    }
    else if (event->isReleased()) {
        mechy->popLayer();
    }
}
