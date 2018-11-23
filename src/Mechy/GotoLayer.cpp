#include "GotoLayer.h"

GotoLayer::GotoLayer() {
}

uint8_t GotoLayer::defaultName() {
    return FN_GOTO_LAYER;
}

bool GotoLayer::is(uint8_t event_type, Event* UNUSED(event)) {
    return event_type == EVENT_META;
}

bool GotoLayer::override(uint8_t UNUSED(name), Event* event, Plugin* plugin) {
    if (!plugin->is(EVENT_META, event) && event->isPressed()) {
        // another key was pressed; find all the GotoLayer events in the current
        // stack and deactivate them.
        EventPtr* eventPtr = mechy->events();
        while (eventPtr) {
            if (eventPtr->event->name == FN_GOTO_LAYER
                && (eventPtr->event->data() & GO_PUSH)
                && eventPtr->event->isActive())
            {
                eventPtr->event->setIsActive(false);
            }
            eventPtr = eventPtr->next;
        }
    }
    return KBD_CONTINUE;
}

void GotoLayer::run(Event* event) {
    if (event->isHeld())  return;

    uint8_t layer = event->key();
    uint8_t behavior = event->userData();

    switch (behavior) {
    case GO_MOMENTARY:
        goto momentary;
    case GO_PUSH:
        goto sticky;
    case GO_NOW:
        goto now;
    case GO_BACK:
        goto back;
    }
    return;

momentary:
    if (event->isPressed()) {
        mechy->pushLayer(layer);
    }
    else if (event->isReleased()) {
        mechy->removeLayer(layer);
    }
    return;

sticky:
    if (event->isPressed()) {
        mechy->pushLayer(layer);
        event->setIsActive(true);
    }
    else if (event->isReleased()) {
        mechy->removeLayer(layer);
        // if the event is still active then no other key was pressed - make
        // this layer the new default.
        if (event->isActive()) {
            mechy->setDefaultLayer(layer);
            event->setIsActive(false);
        }
    }
    return;

now:
    if (event->isPressed()) {
        mechy->pushLayer(layer);
        mechy->setDefaultLayer(layer);
    }
    else if (event->isReleased()) {
        mechy->removeLayer(layer);
    }
    return;

back:
    if (event->isPressed()) {
        mechy->popLayer();
    }
    goto now;
}
