#include "ComboKey.h"

ComboKey::ComboKey(uint8_t _count, const KBD _keys[], KBD _comboKey) {
    count = _count;
    keys = (KBD*)malloc(sizeof(KBD) * count);
    for (uint8_t i = 0; i < count; ++i) {
        keys[i] = _keys[i];
    }
    comboKey = _comboKey;
}

uint8_t ComboKey::defaultName() {
    return FN_COMBO_KEY;
}

bool ComboKey::is(uint8_t event_type, Event* event) {
    Plugin *plugin = NULL;
    if (count == 0) {
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
    Event keyEvent = {
        .name = 0,
        .keyAndData = 0,
        .keyState = comboEvent->keyState,
        .started = comboEvent->started,
    };

    if (comboEvent->isPressed()) {
        if (--count == 0) {
            keyEvent.name = comboKey.name;
            keyEvent.keyAndData = comboKey.key;
            mechy->runPlugin(&keyEvent);
        }
        else {
            KBD kbd = keys[comboEvent->key()];
            keyEvent.name = kbd.name;
            keyEvent.keyAndData = kbd.key;
            mechy->runPlugin(&keyEvent);
        }
    }
    else if (comboEvent->isReleased()) {
        if (count++ == 0) {
            keyEvent.name = comboKey.name;
            keyEvent.keyAndData = comboKey.key;
            mechy->runPlugin(&keyEvent);
        }
        else {
            KBD kbd = keys[comboEvent->key()];
            keyEvent.name = kbd.name;
            keyEvent.keyAndData = kbd.key;
            mechy->runPlugin(&keyEvent);
        }
    }
    else if (comboEvent->isHeld()) {
        if (count == 0) {
            keyEvent.name = comboKey.name;
            keyEvent.keyAndData = comboKey.key;
            mechy->runPlugin(&keyEvent);
        }
        else {
            KBD kbd = keys[comboEvent->key()];
            keyEvent.name = kbd.name;
            keyEvent.keyAndData = kbd.key;
            mechy->runPlugin(&keyEvent);
        }
    }
}
