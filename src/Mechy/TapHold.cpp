#include "Mechy.h"
#include "TapHold.h"

#ifndef TAPHOLD_DELAY
#define TAPHOLD_DELAY 250
#endif

static uint8_t tapHoldKeys = 0;
static TapHoldKeyList* tapHoldKeyStack = NULL;

inline void sendKeyEvent(Mechy* mechy, uint16_t modifierSnapshot, KBD* kbd) {
    uint16_t mods = mechy->currentModifiers();
    mechy->updateModifiers(modifierSnapshot);
    Event event = {
        .name = kbd->name,
        .keyAndData = kbd->key,
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
}

void TapHold::run(Event* event) {
    TapHoldEvent* keyPtr = eventArray + event->key();

    if (event->isPressed()) {
        event->setIsActive(true);
    }

    switch (keyPtr->behavior) {
    case TH_TAP:
        goto runPress;
    case TH_HOLD:
        goto runModifier;
    default:
        event->setIsActive(false);
    }

    return;

runPress:
    if (event->isPressed()) {
        keyPtr->modifierSnapshot = mechy->currentModifiers();
    }
    else if (event->isActive()) {
        if (event->isHeld() && event->duration() > TAPHOLD_DELAY) {
            sendKeyEvent(mechy, keyPtr->modifierSnapshot, &keyPtr->holdKey);
            event->setIsActive(false);
        }
        else if (event->isReleased()) {
            sendKeyEvent(mechy, keyPtr->modifierSnapshot, &keyPtr->tapKey);
            event->setIsActive(false);
        }
    }
    return;

runModifier:
    if (event->isActive() && event->isHeld() && event->duration() > TAPHOLD_DELAY) {
        Event keyEvent = {
            .name = keyPtr->holdKey.name,
            .keyAndData = keyPtr->holdKey.key,
            .keyState = KEY_STATE_PRESSED,
            .started = event->started,
        };
        mechy->runPlugin(&keyEvent);
        event->setIsActive(false);
    }
    else if (event->isHeld() && event->duration() > TAPHOLD_DELAY) {
        Event keyEvent = {
            .name = keyPtr->holdKey.name,
            .keyAndData = keyPtr->holdKey.key,
            .keyState = KEY_STATE_HELD,
            .started = event->started,
        };
        mechy->runPlugin(&keyEvent);
    }
    else if (event->isReleased() && event->duration() > TAPHOLD_DELAY) {
        Event keyEvent = {
            .name = keyPtr->holdKey.name,
            .keyAndData = keyPtr->holdKey.key,
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
            .keyAndData = keyPtr->tapKey.key,
            .keyState = KEY_STATE_PRESSED,
            .started = millis(),
        };
        mechy->runPlugin(&keyEvent);
        keyEvent.keyState = KEY_STATE_RELEASED;
        mechy->runPlugin(&keyEvent);
        mechy->updateModifiers(mods);
        event->setIsActive(false);
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
