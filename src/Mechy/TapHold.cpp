#include "Mechy.h"
#include "TapHold.h"

#ifndef TAPHOLD_DELAY
#define TAPHOLD_DELAY 250
#endif

TapHold::TapHold() {
    eventArray = NULL;
}

uint8_t TapHold::defaultName() {
    return FN_TAP_HOLD;
}

void TapHold::begin() {
    TapHoldEvent* array = (TapHoldEvent*)malloc(sizeof(TapHoldEvent) * TapHold::keys);
    TapHoldKey* ptr = TapHold::keyPtrStack;
    while (ptr) {
        array[ptr->keyIndex].tapKey = ptr->tapKey;
        array[ptr->keyIndex].holdKey = ptr->holdKey;
        array[ptr->keyIndex].behavior = ptr->behavior;
        array[ptr->keyIndex].modifierSnapshot = 0;
        free(ptr);
        ptr = ptr->next;
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
        if (event->isHeld() && event->duration > TAPHOLD_DELAY) {
            uint16_t mods = mechy->currentModifiers();
            mechy->updateModifiers(keyPtr->modifierSnapshot);
            mechy->runPlugin(KEY_STATE_PRESSED, &keyPtr->holdKey, 0);
            mechy->runPlugin(KEY_STATE_RELEASED, &keyPtr->holdKey, 1);
            mechy->updateModifiers(mods);
            unsetEventActive(event);
        }
        else if (event->isReleased()) {
            uint16_t mods = mechy->currentModifiers();
            mechy->updateModifiers(keyPtr->modifierSnapshot);
            mechy->runPlugin(KEY_STATE_PRESSED, &keyPtr->tapKey, 0);
            mechy->runPlugin(KEY_STATE_RELEASED, &keyPtr->tapKey, 1);
            mechy->updateModifiers(mods);
            unsetEventActive(event);
        }
    }
    return;

runModifier:
    if (isEventActive(event) && event->isHeld() && event->duration > TAPHOLD_DELAY) {
        mechy->runPlugin(KEY_STATE_PRESSED, &keyPtr->holdKey, 0);
        unsetEventActive(event);
    }
    else if (event->isHeld() && event->duration > TAPHOLD_DELAY) {
        mechy->runPlugin(KEY_STATE_HELD, &keyPtr->holdKey, event->duration - TAPHOLD_DELAY);
    }
    else if (event->isReleased() && event->duration > TAPHOLD_DELAY) {
        mechy->runPlugin(KEY_STATE_RELEASED, &keyPtr->holdKey, event->duration - TAPHOLD_DELAY);
    }
    else if (event->isReleased()) {
        uint16_t mods = mechy->currentModifiers();
        mechy->updateModifiers(keyPtr->modifierSnapshot);
        mechy->runPlugin(KEY_STATE_PRESSED, &keyPtr->tapKey, 0);
        mechy->runPlugin(KEY_STATE_RELEASED, &keyPtr->tapKey, 1);
        mechy->updateModifiers(mods);
        unsetEventActive(event);
    }
    return;
}

uint16_t TapHold::add(KBD tapKey, KBD holdKey, THBehavior behavior) {
    uint8_t keyIndex = TapHold::keys;
    TapHoldKey* ptr = (TapHoldKey*)malloc(sizeof(TapHoldKey));
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
TapHoldKey* TapHold::keyPtrStack = NULL;
