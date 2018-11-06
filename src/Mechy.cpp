#include "Mechy.h"

Mechy::Mechy() {
    firstPluginPtr = NULL;
    firstKBDPtr = NULL;
    event = { .key = MCHY_NONE, .keyState = KEY_STATE_NONE, .duration = 0 };
}

inline void Mechy::appendPluginPtr(PluginPtr *ptr) {
    if (firstPluginPtr) {
        PluginPtr *lastPtr = firstPluginPtr;
        while (lastPtr->next) {
            lastPtr = lastPtr->next;
        }
        lastPtr->next = ptr;
    }
    else {
        firstPluginPtr = ptr;
    }
}

inline void Mechy::appendKBDPtr(KBDDataPtr *ptr) {
    ptr->next = NULL;
    if (firstKBDPtr) {
        KBDDataPtr *lastPtr = firstKBDPtr;
        while (lastPtr->next) {
            lastPtr = lastPtr->next;
        }
        lastPtr->next = ptr;
    }
    else {
        firstKBDPtr = ptr;
    }
}

inline KBDDataPtr *Mechy::removeKBDPtr(KBDDataPtr *ptr) {
    if (!firstKBDPtr || firstKBDPtr == ptr) {
        firstKBDPtr = ptr->next;
        free(ptr);
        return firstKBDPtr;
    }

    KBDDataPtr *kbdPtr = firstKBDPtr;
    while (kbdPtr->next) {
        if (kbdPtr->next == ptr) {
            kbdPtr->next = ptr->next;
            free(ptr);
            return kbdPtr->next;
        }
        kbdPtr = kbdPtr->next;
    }
    free(ptr);
    return NULL;
}

void Mechy::add(uint8_t name, Plugin *plugin) {
    PluginPtr *ptr = (PluginPtr*)malloc(sizeof(PluginPtr));
    ptr->name = name;
    ptr->plugin = plugin;
    ptr->next = NULL;
    plugin->mechy = this;

    appendPluginPtr(ptr);
}

void Mechy::_begin() {
    PluginPtr *ptr = firstPluginPtr;
    while (ptr) {
        ptr->plugin->begin();
        ptr = ptr->next;
    }
}

void Mechy::_tick() {
    PluginPtr *ptr = firstPluginPtr;
    while (ptr) {
        ptr->plugin->tick();
        ptr = ptr->next;
    }
}

bool anywhere = false;
void Mechy::processKeyEvent(bool isPressed, KBD *currentKey) {
    if (isPressed) {
        anywhere = true;
    }
    else if (anywhere) {
    }

    unsigned long now = millis();

    // find the prev key event data, if present, and trim away any key events
    // that are finished (!isPressed) and old (finished > 10ms ago).
    KBDDataPtr *findPtr = firstKBDPtr;
    KBDDataPtr *kbdData = NULL;
    while (findPtr) {
        if (findPtr->kbd == currentKey) {
            kbdData = findPtr;
        }

        // remove "stale" keyboard events
        if (!findPtr->isPressed && now - findPtr->started > 10) {
            // remove the ptr and return the next one or NULL
            findPtr = removeKBDPtr(findPtr);
        }
        else {
            findPtr = findPtr->next;
        }
    }

    bool currentKeyIsPressed = kbdData ? kbdData->isPressed : false;
    uint16_t currentKeyDuration = kbdData ? now - kbdData->started : 0;

    // ignore all debouncing signals, HIGH or LOW
    if (kbdData && currentKeyDuration < 10) {
        return;
    }

    if (!currentKeyIsPressed && isPressed) {

        // kbdData may or may not be NULL, if it exists reuse it, otherwise
        // create it and append it.
        KBDDataPtr *ptr;
        if (kbdData) {
            ptr = kbdData;
        }
        else {
            ptr = (KBDDataPtr*)malloc(sizeof(KBDDataPtr));
            ptr->kbd = currentKey;
            appendKBDPtr(ptr);
        }

        ptr->isPressed = true;
        ptr->started = now;
        runPlugin(KEY_STATE_PRESSED, ptr, currentKeyDuration);
    }
    else if (currentKeyIsPressed) {
        if (!isPressed) {
            kbdData->isPressed = false;
            kbdData->started = now;  // reset timer for debouncing
            runPlugin(KEY_STATE_RELEASED, kbdData, currentKeyDuration);
        }
        else {
            runPlugin(KEY_STATE_HELD, kbdData, currentKeyDuration);
        }
    }
}

void Mechy::runPlugin(uint8_t keyState, KBDDataPtr *kbdData, uint16_t duration) {
    event.key = kbdData->kbd->key;
    event.keyState = keyState;
    event.duration = duration;

    bool processing = KBD_CONTINUE;
    PluginPtr *ptr = firstPluginPtr;
    while (ptr) {
        processing = ptr->plugin->override(kbdData->kbd->name, &event) && processing;
        ptr = ptr->next;
    }

    if (processing == KBD_CONTINUE) {
        PluginPtr *ptr = firstPluginPtr;
        while (ptr) {
            if (ptr->name == kbdData->kbd->name) {
                ptr->plugin->run(&event);
            }
            ptr = ptr->next;
        }
    }
}
