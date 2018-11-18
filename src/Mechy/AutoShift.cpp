#include "Mechy.h"
#include "AutoShift.h"

#ifndef AUTOSHIFT_DELAY
#define AUTOSHIFT_DELAY 250
#endif

inline void sendUpper(Mechy* mechy, uint8_t key) {
    mechy->pressModifier(MOD_LSFT);
    mechy->pressKey(key);
    mechy->releaseKey(key);
    mechy->releaseModifier(MOD_LSFT);
}

inline void sendLower(Mechy* mechy, uint8_t key) {
    mechy->pressKey(key);
    mechy->releaseKey(key);
}

AutoShift::AutoShift() {
}

uint8_t AutoShift::defaultName() {
    return FN_AUTO_SHIFT;
}

bool eventIsTap(Event* event) {
    return event->isActive() && event->isReleased();
}

bool eventIsHold(Event* event) {
    return event->isActive() && event->duration() > AUTOSHIFT_DELAY;
}

bool AutoShift::is(uint8_t event_type, Event* event) {
    // a tap event occurs when event is still active and on release
    if (eventIsTap(event)) {
        return event_type == EVENT_KEYPRESS;
    }

    // a hold event occurs once, after the key is held for AUTOSHIFT_DELAY ms
    if (eventIsHold(event)) {
        return event_type == EVENT_KEYPRESS;
    }

    return false;
}

bool AutoShift::override(uint8_t UNUSED(name), Event* event, Plugin* plugin) {
    if (plugin->is(EVENT_KEYPRESS, event) && event->isPressed()) {
        // another key was pressed; find all the AutoShift events in the current stack
        // and send the lower case key
        EventPtr* eventPtr = mechy->events();
        while (eventPtr) {
            if (eventPtr->event->name == FN_AUTO_SHIFT && eventPtr->event->isActive()) {
                sendLower(mechy, eventPtr->event->key());
                eventPtr->event->setIsActive(false);
            }
            eventPtr = eventPtr->next;
        }
    }
    return KBD_CONTINUE;
}

void AutoShift::run(Event* event) {
    if (event->isPressed()) {
        event->setIsActive(true);
    }
    else if (eventIsHold(event)) {
        sendUpper(mechy, event->key());
        event->setIsActive(false);
    }
    else if (eventIsTap(event)) {
        sendLower(mechy, event->key());
    }

    if (event->isReleased()) {
        uint16_t mods = mechy->currentModifiers();
        mechy->clearModifiers();
        mechy->updateModifiers(mods);
        event->setIsActive(false);
    }
}
