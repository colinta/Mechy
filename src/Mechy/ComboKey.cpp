#include "ComboKey.h"
#include "../priv/Alloc.h"

ComboKey::ComboKey(uint8_t count, const KBD _keys[], KBD _comboKey) {
    currentCount = count;
    totalCount = count;
    keys = NULL;
    if (totalCount) {
        // avr-libc malloc(0) returns NULL, so only allocate for a non-empty
        // combo; is()/run() already bounds-check before indexing keys
        keys = (KBD*)mechyAllocOrHalt(sizeof(KBD) * totalCount, MECHY_HALT_COMBOKEY);
        for (uint8_t i = 0; i < totalCount; ++i) {
            keys[i] = _keys[i];
        }
    }
    comboKey = _comboKey;
}

uint8_t ComboKey::defaultName() {
    return FN_COMBO_KEY;
}

bool ComboKey::is(uint8_t event_type, Event* event) {
    if (event->key() >= totalCount)  return false;

    Plugin* plugin = NULL;
    if (currentCount == 0) {
        plugin = mechy->pluginFor(comboKey.name);
        return plugin && plugin->is(event_type, event);
    }
    else {
        KBD kbd = keys[event->key()];
        plugin = mechy->pluginFor(kbd.name);
        if (!plugin)  return false;

        if (plugin->is(event_type, event)) {
            event->name = kbd.name;
            event->keyAndData = kbd.key;
            return true;
        }
    }
    return false;
}

void ComboKey::run(Event* comboEvent) {
    if (comboEvent->key() >= totalCount)  return;

    Event keyEvent = {
        .name = 0,
        .keyAndData = 0,
        .internalState = comboEvent->internalState,
        .started = comboEvent->started,
    };

    if (comboEvent->isPressed()) {
        if (--currentCount == 0) {
            keyEvent.name = comboKey.name;
            keyEvent.keyAndData = comboKey.key;
            mechy->runEvent(&keyEvent);
        }
        else {
            KBD kbd = keys[comboEvent->key()];
            keyEvent.name = kbd.name;
            keyEvent.keyAndData = kbd.key;
            mechy->runEvent(&keyEvent);
        }
    }
    else if (comboEvent->isReleased()) {
        if (currentCount++ == 0) {
            keyEvent.name = comboKey.name;
            keyEvent.keyAndData = comboKey.key;
            mechy->runEvent(&keyEvent);
        }
        else {
            KBD kbd = keys[comboEvent->key()];
            keyEvent.name = kbd.name;
            keyEvent.keyAndData = kbd.key;
            mechy->runEvent(&keyEvent);
        }
    }
    else if (comboEvent->isHeld()) {
        if (currentCount == 0) {
            keyEvent.name = comboKey.name;
            keyEvent.keyAndData = comboKey.key;
            mechy->runEvent(&keyEvent);
        }
        else {
            KBD kbd = keys[comboEvent->key()];
            keyEvent.name = kbd.name;
            keyEvent.keyAndData = kbd.key;
            mechy->runEvent(&keyEvent);
        }
    }
}
