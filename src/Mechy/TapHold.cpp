#include "Mechy.h"
#include "TapHold.h"
#include "Sticky.h"

#ifndef TAPHOLD_DELAY
#define TAPHOLD_DELAY 250
#endif

inline void sendUpDownEvent(Mechy* mechy, uint16_t modifierSnapshot, KBD* kbd) {
    uint16_t mods = mechy->currentModifiers();
    mechy->updateModifiers(modifierSnapshot);
    Event keyEvent = {
        .name = kbd->name,
        .keyAndData = kbd->key,
        .internalState = KEY_STATE_PRESSED,
        .started = millis(),
    };
    mechy->runEvent(&keyEvent);
    keyEvent.setKeyState(KEY_STATE_RELEASED);
    mechy->runEvent(&keyEvent);
    mechy->updateModifiers(mods);
}

TapHold::TapHold() {
    tapHoldKeys = 0;
    tapHoldKeyStack = NULL;
    eventArray = NULL;
}

uint8_t TapHold::defaultName() {
    return FN_TAP_HOLD;
}

bool TapHold::is(uint8_t event_type, Event* event) {
    TapHoldEvent* keyPtr = thEvent(event->key());

    switch (keyPtr->behavior) {
    case TH_TAP:
        goto isPress;
    case TH_HOLD:
        goto isModifier;
    }

    return false;

isPress:
    if (!event->isPressed() && event->isActive()) {
        if (event->isHeld() && event->duration() > TAPHOLD_DELAY) {
            Plugin* plugin = mechy->pluginFor(keyPtr->holdKey.name);
            if (plugin) {
                Event keyEvent = {
                    .name = keyPtr->holdKey.name,
                    .keyAndData = keyPtr->holdKey.key,
                    .internalState = KEY_STATE_PRESSED,
                    .started = event->started,
                };
                bool retval = plugin->is(event_type, &keyEvent);
                keyEvent.setKeyState(KEY_STATE_RELEASED);
                retval = retval || plugin->is(event_type, &keyEvent);
                return retval;
            }
        }
        else if (event->isReleased()) {
            Plugin* plugin = mechy->pluginFor(keyPtr->tapKey.name);
            if (plugin) {
                Event keyEvent = {
                    .name = keyPtr->tapKey.name,
                    .keyAndData = keyPtr->tapKey.key,
                    .internalState = KEY_STATE_PRESSED,
                    .started = event->started,
                };
                bool retval = plugin->is(event_type, &keyEvent);
                keyEvent.setKeyState(KEY_STATE_RELEASED);
                retval = retval || plugin->is(event_type, &keyEvent);
                return retval;
            }
        }
    }
    return false;

isModifier:
    if (event->isActive() && event->isHeld() && event->duration() > TAPHOLD_DELAY) {
        Plugin* plugin = mechy->pluginFor(keyPtr->holdKey.name);
        if (plugin) {
            Event keyEvent = {
                .name = keyPtr->holdKey.name,
                .keyAndData = keyPtr->holdKey.key,
                .internalState = KEY_STATE_PRESSED,
                .started = event->started,
            };
            return plugin->is(event_type, &keyEvent);
        }
    }
    else if (event->isHeld() && event->duration() > TAPHOLD_DELAY) {
        Plugin* plugin = mechy->pluginFor(keyPtr->holdKey.name);
        if (plugin) {
            Event keyEvent = {
                .name = keyPtr->holdKey.name,
                .keyAndData = keyPtr->holdKey.key,
                .internalState = KEY_STATE_HELD,
                .started = event->started,
            };
            return plugin->is(event_type, &keyEvent);
        }
    }
    else if (event->isReleased() && event->duration() > TAPHOLD_DELAY) {
        Plugin* plugin = mechy->pluginFor(keyPtr->holdKey.name);
        if (plugin) {
            Event keyEvent = {
                .name = keyPtr->holdKey.name,
                .keyAndData = keyPtr->holdKey.key,
                .internalState = KEY_STATE_RELEASED,
                .started = event->started,
            };
            return plugin->is(event_type, &keyEvent);
        }
    }
    else if (event->isReleased() && event->isActive()) {
        Plugin* plugin = mechy->pluginFor(keyPtr->tapKey.name);
        if (plugin) {
            Event keyEvent = {
                .name = keyPtr->tapKey.name,
                .keyAndData = keyPtr->tapKey.key,
                .internalState = KEY_STATE_PRESSED,
                .started = event->started,
            };
            bool retval = plugin->is(event_type, &keyEvent);
            keyEvent.setKeyState(KEY_STATE_RELEASED);
            retval = retval || plugin->is(event_type, &keyEvent);
            return retval;
        }
    }
    return false;
}

bool TapHold::override(uint8_t UNUSED(name), Event* event, Plugin* UNUSED(plugin)) {
    if (!event->isPressed())  return KBD_CONTINUE;

    bool didFinishEvent = false;
    EventPtr* eventPtr = mechy->events();
    while (eventPtr) {
        if (eventPtr->event->name == FN_TAP_HOLD && eventPtr->event->isActive() && eventPtr->event->isHeld()) {
            mechy->finishEvent(eventPtr->event);
            didFinishEvent = true;
        }
        eventPtr = eventPtr->next;
    }

    Sticky* sticky = didFinishEvent ? (Sticky*)mechy->pluginFor(FN_STICKY) : NULL;
    if (didFinishEvent && sticky) {
        sticky->clearStickyMods();
    }
    return KBD_CONTINUE;
}

void TapHold::begin() {
    if (!tapHoldKeys)  return;

    TapHoldEvent* array = (TapHoldEvent*)malloc(sizeof(TapHoldEvent) * tapHoldKeys);
    TapHoldKeyList* ptr = tapHoldKeyStack;
    TapHoldKeyList* next = NULL;
    while (ptr) {
        array[ptr->keyIndex].tapKey = ptr->tapKey;
        array[ptr->keyIndex].holdKey = ptr->holdKey;
        array[ptr->keyIndex].behavior = ptr->behavior;
        array[ptr->keyIndex].modifierSnapshot = 0;
        next = ptr->next;
        free(ptr);
        ptr = next;
    }
    eventArray = array;
    tapHoldKeyStack = NULL;
}

TapHoldEvent* TapHold::thEvent(uint8_t offset) {
    if (!eventArray)  return NULL;
    return eventArray + offset;
}

void TapHold::run(Event* event) {
    TapHoldEvent* keyPtr = thEvent(event->key());

    if (event->isPressed()) {
        event->setIsActive(true);
        keyPtr->modifierSnapshot = mechy->currentModifiers();
    }

    switch (keyPtr->behavior) {
    case TH_TAP:
        goto runPress;
    case TH_HOLD:
        goto runModifier;
    }

    return;

runPress:
    if (!event->isPressed() && event->isActive()) {
        // setIsActive must be called before sendUpDownEvent, otherwise override
        // above will think that the holdKey press event is a random keypress,
        // and will activate the tapKey event
        if (event->isHeld() && event->duration() > TAPHOLD_DELAY) {
            event->setIsActive(false);
            sendUpDownEvent(mechy, keyPtr->modifierSnapshot, &keyPtr->holdKey);
        }
        else if (event->isReleased()) {
            event->setIsActive(false);
            sendUpDownEvent(mechy, keyPtr->modifierSnapshot, &keyPtr->tapKey);
        }
    }
    return;

runModifier:
    if (event->isActive() && event->isHeld() && event->duration() > TAPHOLD_DELAY) {
        event->setIsActive(false);
        Event keyEvent = {
            .name = keyPtr->holdKey.name,
            .keyAndData = keyPtr->holdKey.key,
            .internalState = KEY_STATE_PRESSED,
            .started = event->started,
        };
        mechy->runEvent(&keyEvent);
    }
    else if (event->isHeld() && event->duration() > TAPHOLD_DELAY) {
        Event keyEvent = {
            .name = keyPtr->holdKey.name,
            .keyAndData = keyPtr->holdKey.key,
            .internalState = KEY_STATE_HELD,
            .started = event->started,
        };
        mechy->runEvent(&keyEvent);
    }
    else if (event->isReleased() && event->duration() > TAPHOLD_DELAY) {
        Event keyEvent = {
            .name = keyPtr->holdKey.name,
            .keyAndData = keyPtr->holdKey.key,
            .internalState = KEY_STATE_RELEASED,
            .started = event->started,
        };
        mechy->runEvent(&keyEvent);
    }
    else if (event->isReleased() && event->isActive()) {
        event->setIsActive(false);
        sendUpDownEvent(mechy, keyPtr->modifierSnapshot, &keyPtr->tapKey);
    }
    return;
}

void TapHold::add(KBD tapKey, KBD holdKey, THBehavior behavior) {
    uint8_t keyIndex = tapHoldKeys++;
    TapHoldKeyList* ptr = (TapHoldKeyList*)malloc(sizeof(TapHoldKeyList));
    ptr->keyIndex = keyIndex;
    ptr->tapKey = tapKey;
    ptr->holdKey = holdKey;
    ptr->behavior = behavior;
    ptr->next = tapHoldKeyStack;
    tapHoldKeyStack = ptr;
}
