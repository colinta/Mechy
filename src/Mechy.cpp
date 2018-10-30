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
        ptr->plugin->begin();
        ptr = ptr->next;
    }
}

void Mechy::_tick() {
    PluginPtr *ptr =&firstPlugin;
    while (ptr) {
        ptr->plugin->tick();
        ptr = ptr->next;
    }
}

void Mechy::processKeyEvent(bool isPressed, KBD *currentKey) {
    if (!currentKey->isPressed && isPressed) {
        // if the key was just released, ignore debouncing HIGH signals
        if (millis() - currentKey->started < 10) {
            return;
        }

        currentKey->isPressed = true;
        currentKey->started = millis();
        runPlugin(true, false, currentKey);
    }
    else if (currentKey->isPressed) {
        // if the key was just pressed, ignore debouncing LOW signals
        if (!isPressed && millis() - currentKey->started < 10) {
            return;
        }

        if (!isPressed) {
            currentKey->isPressed = false;
            runPlugin(false, true, currentKey);
            currentKey->started = millis();
        }
        else {
            runPlugin(false, false, currentKey);
        }
    }
}

void Mechy::runPlugin(bool isDown, bool isUp, KBD *currentKey) {
    event.key = currentKey->key;
    event.keyState = isDown ? KEY_PRESSED : (isUp ? KEY_RELEASED : KEY_HELD);
    event.duration = millis() - currentKey->started;

    bool processing = KBD_CONTINUE;
    PluginPtr *ptr =&firstPlugin;
    while (ptr) {
        processing = ptr->plugin->override(currentKey->name, &event) && processing;
        ptr = ptr->next;
    }

    if (processing == KBD_CONTINUE) {
        PluginPtr *ptr =&firstPlugin;
        while (ptr) {
            if (ptr->name == currentKey->name) {
                ptr->plugin->run(&event);
            }
            ptr = ptr->next;
        }
    }
}

void Plugin::begin() {
}

void Plugin::tick() {
}

bool Plugin::override(uint8_t name, Event *event) {
    return KBD_CONTINUE;
}
