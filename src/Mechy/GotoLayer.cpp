#include "GotoLayer.h"

GotoLayer::GotoLayer() {
}

uint8_t GotoLayer::defaultName() {
    return FN_GOTO_LAYER;
}

bool GotoLayer::is(uint8_t event_type, Event* UNUSED(event)) {
    return event_type == EVENT_LAYER;
}

bool GotoLayer::override(Event* event, Plugin* UNUSED(plugin)) {
    if (event->isPressed()) {
        // if another layer key was pressed find all the GotoLayer events in the
        // current stack and deactivate them.
        EventPtr* eventPtr = mechy->events();
        while (eventPtr) {
            if (eventPtr->event->name == FN_GOTO_LAYER
                && (eventPtr->event->userData() & GO_PUSH)
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
        goto push;
    case GO_LSET:
        goto lset;
    case GO_BACK:
        goto back;
    }
    return;

momentary:
    if (event->isPressed()) {
        if (mechy->pushLayer(layer)) {
            event->setDataBit(EVENT_GOTO_DID_PUSH_BIT, true);
        }
    }
    else if (event->isReleased()) {
        // only remove the layer if the paired press actually pushed it
        if (event->dataBit(EVENT_GOTO_DID_PUSH_BIT)) {
            mechy->removeLayer(layer);
        }
    }
    return;

push:
    if (event->isPressed()) {
        if (mechy->pushLayer(layer)) {
            event->setDataBit(EVENT_GOTO_DID_PUSH_BIT, true);
            event->setIsActive(true);
        }
        // if the push failed, the event is not marked active: the one-shot
        // layer was never activated
    }
    else if (event->isReleased() && event->isActive()) {
        // if the event is still active then no other key was pressed
        // do nothing, ie keep this layer on the stack
    }
    else if (event->isReleased() && event->dataBit(EVENT_GOTO_DID_PUSH_BIT)) {
        // if the event is no longer active then it can be removed from the
        // stack - but only if the paired press actually pushed it
        mechy->removeLayer(layer);
    }
    return;

lset:
    if (event->isPressed()) {
        mechy->clearLayers();
        mechy->setDefaultLayer(layer);
    }
    return;

back:
    if (event->isPressed()) {
        mechy->popLayer();
    }
    return;
}
