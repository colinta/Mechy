#include "Mechy.h"

Mechy::Mechy() {
    firstPlugin.plugin = 0x0;
    firstPlugin.next = 0x0;
    lastPtr = 0x0;
    event = { .key = 0x0, .keyState = KEY_NONE, .duration = 0 };
}

void Mechy::add(uint8_t name, Plugin *plugin) {
    PluginPtr *ptr;
    if (!lastPtr) {
        ptr = &firstPlugin;
    }
    else {
        ptr = (PluginPtr*)malloc(sizeof(PluginPtr));
        lastPtr->next = ptr;
    }

    ptr->name = name;
    ptr->plugin = plugin;
    ptr->next = 0x0;
    lastPtr = ptr;

    plugin->mechy = this;
}

void Mechy::_begin() {
    PluginPtr *ptr =&firstPlugin;
    while (ptr) {
        ptr->plugin->process_begin();
        ptr = ptr->next;
    }
}

void Mechy::_tick() {
    PluginPtr *ptr =&firstPlugin;
    while (ptr) {
        ptr->plugin->process_tick();
        ptr = ptr->next;
    }
}

void Mechy::process_key_event(bool isPressed, KBD *currentKey) {
    if (!currentKey->isPressed && isPressed) {
        // if the key was just released, ignore debouncing HIGH signals
        if (millis() - currentKey->started < 10) {
            return;
        }

        currentKey->isPressed = true;
        currentKey->started = millis();
        process_event(true, false, currentKey);
    }
    else if (currentKey->isPressed) {
        // if the key was just pressed, ignore debouncing LOW signals
        if (!isPressed && millis() - currentKey->started < 10) {
            return;
        }

        if (!isPressed) {
            currentKey->isPressed = false;
            process_event(false, true, currentKey);
            currentKey->started = millis();
        }
        else {
            process_event(false, false, currentKey);
        }
    }
}

void Mechy::process_event(bool isDown, bool isUp, KBD *currentKey) {
    event.key = currentKey->key;
    event.keyState = isDown ? KEY_PRESSED : (isUp ? KEY_RELEASED : KEY_HELD);
    event.duration = millis() - currentKey->started;

    bool processing = KBD_CONTINUE;
    PluginPtr *ptr =&firstPlugin;
    while (ptr) {
        processing = ptr->plugin->process_override(currentKey->name, &event) && processing;
        ptr = ptr->next;
    }

    if (processing == KBD_CONTINUE) {
        PluginPtr *ptr =&firstPlugin;
        while (ptr) {
            if (ptr->name == currentKey->name) {
                ptr->plugin->process_event(&event);
            }
            ptr = ptr->next;
        }
    }
}

void Plugin::process_begin() {
}

void Plugin::process_tick() {
}

bool Plugin::process_override(uint8_t name, Event *event) {
    return KBD_CONTINUE;
}
