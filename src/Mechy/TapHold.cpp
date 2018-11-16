#include "Mechy.h"
#include "TapHold.h"

#ifndef TAPHOLD_DELAY
#define TAPHOLD_DELAY 250
#endif

inline void sendKeyEvent(Mechy* mechy, uint16_t modifierSnapshot, KBD* kbd) {
    uint16_t mods = mechy->currentModifiers();
    mechy->updateModifiers(modifierSnapshot);
    Event event = {
        .name = kbd->name,
        .key  = kbd->key,
        .keyState = KEY_STATE_PRESSED,
        .started = millis(),
    };
    mechy->runPlugin(&event);
    event.keyState = KEY_STATE_RELEASED;
    mechy->runPlugin(&event);
    mechy->updateModifiers(mods);
}

TapHold::TapHold() {
    eventArray = NULL;
}

uint8_t TapHold::defaultName() {
    return FN_TAP_HOLD;
}

void TapHold::begin() {
    TapHoldEvent* array = (TapHoldEvent*)malloc(sizeof(TapHoldEvent) * TapHold::keys);
    TapHoldKeyList* ptr = TapHold::keyPtrStack;
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
}

void TapHold::run(Event* event) {
    TapHoldEvent* keyPtr = eventArray + (event->key & EVENT_KEY_MASK);

    if (event->isPressed()) {
        setEventActive(event);
    }

    switch (keyPtr->behavior) {
    case TH_PRESS:
        goto runPress;
    case TH_MODIFIER:
        goto runModifier;
    default:
        unsetEventActive(event);
    }

    return;

runPress:
    if (event->isPressed()) {
        keyPtr->modifierSnapshot = mechy->currentModifiers();
    }
    else if (isEventActive(event)) {
        if (event->isHeld() && event->duration() > TAPHOLD_DELAY) {
            sendKeyEvent(mechy, keyPtr->modifierSnapshot, &keyPtr->holdKey);
            unsetEventActive(event);
        }
        else if (event->isReleased()) {
            sendKeyEvent(mechy, keyPtr->modifierSnapshot, &keyPtr->tapKey);
            unsetEventActive(event);
        }
    }
    return;

runModifier:
    if (isEventActive(event) && event->isHeld() && event->duration() > TAPHOLD_DELAY) {
        Event keyEvent = {
            .name = keyPtr->holdKey.name,
            .key  = keyPtr->holdKey.key,
            .keyState = KEY_STATE_PRESSED,
            .started = event->started,
        };
        mechy->runPlugin(&keyEvent);
        unsetEventActive(event);
    }
    else if (event->isHeld() && event->duration() > TAPHOLD_DELAY) {
        Event keyEvent = {
            .name = keyPtr->holdKey.name,
            .key  = keyPtr->holdKey.key,
            .keyState = KEY_STATE_HELD,
            .started = event->started,
        };
        mechy->runPlugin(&keyEvent);
    }
    else if (event->isReleased() && event->duration() > TAPHOLD_DELAY) {
        Event keyEvent = {
            .name = keyPtr->holdKey.name,
            .key  = keyPtr->holdKey.key,
            .keyState = KEY_STATE_RELEASED,
            .started = event->started,
        };
        mechy->runPlugin(&keyEvent);
    }
    else if (event->isReleased()) {
        uint16_t mods = mechy->currentModifiers();
        mechy->updateModifiers(keyPtr->modifierSnapshot);
        Event keyEvent = {
            .name = keyPtr->tapKey.name,
            .key  = keyPtr->tapKey.key,
            .keyState = KEY_STATE_PRESSED,
            .started = millis(),
        };
        mechy->runPlugin(&keyEvent);
        keyEvent.keyState = KEY_STATE_RELEASED;
        mechy->runPlugin(&keyEvent);
        mechy->updateModifiers(mods);
        unsetEventActive(event);
    }
    return;
}

uint16_t TapHold::add(KBD tapKey, KBD holdKey, THBehavior behavior) {
    uint8_t keyIndex = TapHold::keys;
    TapHoldKeyList* ptr = (TapHoldKeyList*)malloc(sizeof(TapHoldKeyList));
    ptr->keyIndex = keyIndex;
    ptr->tapKey = tapKey;
    ptr->holdKey = holdKey;
    ptr->behavior = behavior;
    ptr->next = TapHold::keyPtrStack;
    TapHold::keyPtrStack = ptr;
    ++TapHold::keys;
    return keyIndex;
}

uint8_t TapHold::keys = 0;
TapHoldKeyList* TapHold::keyPtrStack = NULL;
