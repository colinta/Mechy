#include "GotoLayer.h"

GotoLayer::GotoLayer() {
}

uint8_t GotoLayer::defaultName() {
    return FN_GOTO_LAYER;
}

bool GotoLayer::is(uint8_t event_type, Event* UNUSED(event)) {
    return event_type == EVENT_META;
}

void GotoLayer::run(Event* event) {
    uint8_t layer = event->key & 0b1111;
    // uint8_t behavior = event->key >> 4;

    if (event->isPressed()) {
        mechy->pushLayer(layer);
    }
    else if (event->isReleased()) {
        mechy->popLayer();
    }
}
