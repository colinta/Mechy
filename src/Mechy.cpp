#include "Mechy.h"

#include <Keyboard.h>

Mechy::Mechy() {
    modifiers = 0;
    capsIsOn = false;
    layerStackPtr = NULL;
    firstResponderPtr = NULL;
    firstPluginPtr = NULL;
    firstEventPtr = NULL;
}

void Mechy::begin() {
    Keyboard.begin();

    ResponderPtr* respPtr = firstResponderPtr;
    while (respPtr) {
        respPtr->responder->begin();
        respPtr = respPtr->next;
    }

    PluginPtr* pluginPtr = firstPluginPtr;
    while (pluginPtr) {
        pluginPtr->plugin->begin();
        pluginPtr = pluginPtr->next;
    }
}

void Mechy::tick() {
    ResponderPtr* respPtr = firstResponderPtr;
    while (respPtr) {
        respPtr->responder->scan();
        respPtr = respPtr->next;
    }

    PluginPtr* pluginPtr = firstPluginPtr;
    while (pluginPtr) {
        pluginPtr->plugin->tick();
        pluginPtr = pluginPtr->next;
    }
}

void Mechy::attach(Responder* responder) {
    ResponderPtr* ptr = (ResponderPtr*)malloc(sizeof(ResponderPtr));
    ptr->responder = responder;
    ptr->next = NULL;
    responder->mechy = this;

    pushResponderPtr(ptr);
}

void Mechy::add(Plugin* plugin) {
    add(plugin->defaultName(), plugin);
}

void Mechy::add(uint8_t name, Plugin* plugin) {
    PluginPtr* ptr = (PluginPtr*)malloc(sizeof(PluginPtr));
    ptr->name = name;
    ptr->plugin = plugin;
    ptr->next = NULL;
    plugin->mechy = this;

    pushPluginPtr(ptr);
}

void Mechy::pushLayer(uint8_t layer) {
    LayerStackPtr* layerPtr = (LayerStackPtr*)malloc(sizeof(LayerStackPtr));
    layerPtr->value = layer;
    layerPtr->prev = layerStackPtr;
    layerStackPtr = layerPtr;

    updateLayer(layer);
}

void Mechy::popLayer() {
    if (layerStackPtr) {
        free(layerStackPtr);
        layerStackPtr = layerStackPtr->prev;
    }

    updateLayer(layerStackPtr ? layerStackPtr->value : 0);
}

uint8_t Mechy::currentLayer() {
    return (layerStackPtr ? layerStackPtr->value : 0);
}

void Mechy::updateLayer(uint8_t layer) {
    ResponderPtr* respPtr = firstResponderPtr;
    while (respPtr) {
        respPtr->responder->gotoLayer(layer);
        respPtr = respPtr->next;
    }
}

EventPtr* Mechy::events() {
    return firstEventPtr;
}

void Mechy::processKeyEvent(Layout* layout, uint8_t row, uint8_t col, bool isPressed) {
    unsigned long now = millis();

    // find the prev key event data, if present, and trim away any key events
    // that are finished (!isPressed) and old (finished > 10ms ago).
    EventPtr* findPtr = firstEventPtr;
    EventPtr* kbdData = NULL;
    while (findPtr) {
        if (findPtr->matches(layout, row, col)) {
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
        EventPtr* ptr = NULL;
        KBD* kbd = layout->getKey(row, col);

        if (kbd && kbdData) {
            ptr = kbdData;
        }
        else if (kbd) {
            ptr = (EventPtr*)malloc(sizeof(EventPtr));
            ptr->layout = layout;
            ptr->row = row;
            ptr->col = col;
            pushKBDPtr(ptr);
        }

        if (ptr) {
            ptr->kbd = kbd;
            ptr->isPressed = true;
            ptr->started = now;
            runPlugin(KEY_STATE_PRESSED, kbd, currentKeyDuration);
        }
    }
    else if (currentKeyIsPressed) {
        if (!isPressed) {
            kbdData->isPressed = false;
            runPlugin(KEY_STATE_RELEASED, kbdData->kbd, currentKeyDuration);
            kbdData->started = now;  // reset timer for debouncing
        }
        else {
            runPlugin(KEY_STATE_HELD, kbdData->kbd, currentKeyDuration);
        }
    }
}

void Mechy::runPlugin(uint8_t keyState, KBD* kbd, uint16_t duration) {
    uint8_t keyHandlerName = kbd->name;

    Event event = {
        .key = kbd->key,
        .keyState = keyState,
        .duration = duration
    };

    PluginPtr* ptr = firstPluginPtr;
    Plugin* plugin = NULL;
    while (ptr) {
        if (ptr->name == keyHandlerName) {
            plugin = ptr->plugin;
            break;
        }
        ptr = ptr->next;
    }
    if (!plugin)  return;

    bool processing = KBD_CONTINUE;
    ptr = firstPluginPtr;
    while (ptr) {
        processing = ptr->plugin->override(keyHandlerName, &event, plugin) && processing;
        ptr = ptr->next;
    }

    if (processing == KBD_CONTINUE) {
        plugin->run(&event);
    }

    // changes to event.key need to be treated as "data storage"
    kbd->key = event.key;
}

bool Mechy::isCapsOn() {
    return capsIsOn;
}

void Mechy::pressKey(uint8_t key) {
    switch (key) {
    case KEY_LEFT_CTRL:
        pressModifier(MOD_LCTL);
        return;
    case KEY_RIGHT_CTRL:
        pressModifier(MOD_RCTL);
        return;
    case KEY_LEFT_ALT:
        pressModifier(MOD_LALT);
        return;
    case KEY_RIGHT_ALT:
        pressModifier(MOD_RALT);
        return;
    case KEY_LEFT_GUI:
        pressModifier(MOD_LGUI);
        return;
    case KEY_RIGHT_GUI:
        pressModifier(MOD_RGUI);
        return;
    case KEY_LEFT_SHIFT:
        pressModifier(MOD_LSFT);
        return;
    case KEY_RIGHT_SHIFT:
        pressModifier(MOD_RSFT);
        return;
    case KEY_CAPS_LOCK:
        capsIsOn = !capsIsOn;
        break;
    }

    Keyboard.press(key);
}

void Mechy::releaseKey(uint8_t key) {
    switch (key) {
    case KEY_LEFT_CTRL:
        releaseModifier(MOD_LCTL);
        return;
    case KEY_RIGHT_CTRL:
        releaseModifier(MOD_RCTL);
        return;
    case KEY_LEFT_ALT:
        releaseModifier(MOD_LALT);
        return;
    case KEY_RIGHT_ALT:
        releaseModifier(MOD_RALT);
        return;
    case KEY_LEFT_GUI:
        releaseModifier(MOD_LGUI);
        return;
    case KEY_RIGHT_GUI:
        releaseModifier(MOD_RGUI);
        return;
    case KEY_LEFT_SHIFT:
        releaseModifier(MOD_LSFT);
        return;
    case KEY_RIGHT_SHIFT:
        releaseModifier(MOD_RSFT);
        return;
    }

    Keyboard.release(key);
}

void Mechy::pressModifier(uint16_t modMask) {
    uint16_t key = 0;
    uint16_t modShft = 0;
    switch (modMask) {
    case MOD_LCTL:
        key = KEY_LEFT_CTRL;
        modShft = MOD_BITL_LCTL;
        break;
    case MOD_RCTL:
        key = KEY_RIGHT_CTRL;
        modShft = MOD_BITL_RCTL;
        break;
    case MOD_LALT:
        key = KEY_LEFT_ALT;
        modShft = MOD_BITL_LALT;
        break;
    case MOD_RALT:
        key = KEY_RIGHT_ALT;
        modShft = MOD_BITL_RALT;
        break;
    case MOD_LGUI:
        key = KEY_LEFT_GUI;
        modShft = MOD_BITL_LGUI;
        break;
    case MOD_RGUI:
        key = KEY_RIGHT_GUI;
        modShft = MOD_BITL_RGUI;
        break;
    case MOD_LSFT:
        key = KEY_LEFT_SHIFT;
        modShft = MOD_BITL_LSFT;
        break;
    case MOD_RSFT:
        key = KEY_RIGHT_SHIFT;
        modShft = MOD_BITL_RSFT;
        break;
    default:
        return;
    }

    uint16_t count = (modifiers & modMask) >> modShft;
    if (count < 3) {
        count += 1;
        modifiers |= (count << modShft);
        if (count == 1) {
            Keyboard.press(key);
        }
    }
}

void Mechy::releaseModifier(uint16_t modMask) {
    uint16_t key = 0;
    uint16_t modShft = 0;
    switch (modMask) {
    case MOD_LCTL:
        key = KEY_LEFT_CTRL;
        modShft = MOD_BITL_LCTL;
        break;
    case MOD_RCTL:
        key = KEY_RIGHT_CTRL;
        modShft = MOD_BITL_RCTL;
        break;
    case MOD_LALT:
        key = KEY_LEFT_ALT;
        modShft = MOD_BITL_LALT;
        break;
    case MOD_RALT:
        key = KEY_RIGHT_ALT;
        modShft = MOD_BITL_RALT;
        break;
    case MOD_LGUI:
        key = KEY_LEFT_GUI;
        modShft = MOD_BITL_LGUI;
        break;
    case MOD_RGUI:
        key = KEY_RIGHT_GUI;
        modShft = MOD_BITL_RGUI;
        break;
    case MOD_LSFT:
        key = KEY_LEFT_SHIFT;
        modShft = MOD_BITL_LSFT;
        break;
    case MOD_RSFT:
        key = KEY_RIGHT_SHIFT;
        modShft = MOD_BITL_RSFT;
        break;
    default:
        return;
    }

    uint16_t count = (modifiers & modMask) >> modShft;
    if (count > 0) {
        count -= 1;
        modifiers &= ~(0b11 << modShft);
        if (count == 0) {
            Keyboard.release(key);
        }
        else {
            modifiers |= (count << modShft);
        }
    }
}

uint16_t Mechy::currentModifiers() {
    return modifiers;
}

void Mechy::updateModifiers(uint16_t newModifiers) {
    if ((newModifiers & MOD_LCTL) && !(modifiers & MOD_LCTL)) {
        Keyboard.press(KEY_LEFT_CTRL);
    }
    else if (!(newModifiers & MOD_LCTL) && (modifiers & MOD_LCTL)) {
        Keyboard.release(KEY_LEFT_CTRL);
    }

    if ((newModifiers & MOD_RCTL) && !(modifiers & MOD_RCTL)) {
        Keyboard.press(KEY_RIGHT_CTRL);
    }
    else if (!(newModifiers & MOD_RCTL) && (modifiers & MOD_RCTL)) {
        Keyboard.release(KEY_RIGHT_CTRL);
    }

    if ((newModifiers & MOD_LALT) && !(modifiers & MOD_LALT)) {
        Keyboard.press(KEY_LEFT_ALT);
    }
    else if (!(newModifiers & MOD_LALT) && (modifiers & MOD_LALT)) {
        Keyboard.release(KEY_LEFT_ALT);
    }

    if ((newModifiers & MOD_RALT) && !(modifiers & MOD_RALT)) {
        Keyboard.press(KEY_RIGHT_ALT);
    }
    else if (!(newModifiers & MOD_RALT) && (modifiers & MOD_RALT)) {
        Keyboard.release(KEY_RIGHT_ALT);
    }

    if ((newModifiers & MOD_LGUI) && !(modifiers & MOD_LGUI)) {
        Keyboard.press(KEY_LEFT_GUI);
    }
    else if (!(newModifiers & MOD_LGUI) && (modifiers & MOD_LGUI)) {
        Keyboard.release(KEY_LEFT_GUI);
    }

    if ((newModifiers & MOD_RGUI) && !(modifiers & MOD_RGUI)) {
        Keyboard.press(KEY_RIGHT_GUI);
    }
    else if (!(newModifiers & MOD_RGUI) && (modifiers & MOD_RGUI)) {
        Keyboard.release(KEY_RIGHT_GUI);
    }

    if ((newModifiers & MOD_LSFT) && !(modifiers & MOD_LSFT)) {
        Keyboard.press(KEY_LEFT_SHIFT);
    }
    else if (!(newModifiers & MOD_LSFT) && (modifiers & MOD_LSFT)) {
        Keyboard.release(KEY_LEFT_SHIFT);
    }

    if ((newModifiers & MOD_RSFT) && !(modifiers & MOD_RSFT)) {
        Keyboard.press(KEY_RIGHT_SHIFT);
    }
    else if (!(newModifiers & MOD_RSFT) && (modifiers & MOD_RSFT)) {
        Keyboard.release(KEY_RIGHT_SHIFT);
    }

    modifiers = newModifiers;
}

void Mechy::clearModifiers() {
    if (modifiers & MOD_LCTL) {
        Keyboard.release(KEY_LEFT_CTRL);
    }
    if (modifiers & MOD_RCTL) {
        Keyboard.release(KEY_RIGHT_CTRL);
    }
    if (modifiers & MOD_LALT) {
        Keyboard.release(KEY_LEFT_ALT);
    }
    if (modifiers & MOD_RALT) {
        Keyboard.release(KEY_RIGHT_ALT);
    }
    if (modifiers & MOD_LGUI) {
        Keyboard.release(KEY_LEFT_GUI);
    }
    if (modifiers & MOD_RGUI) {
        Keyboard.release(KEY_RIGHT_GUI);
    }
    if (modifiers & MOD_LSFT) {
        Keyboard.release(KEY_LEFT_SHIFT);
    }
    if (modifiers & MOD_RSFT) {
        Keyboard.release(KEY_RIGHT_SHIFT);
    }
    modifiers = 0;
}

inline void Mechy::pushPluginPtr(PluginPtr* ptr) {
    ptr->next = firstPluginPtr;
    firstPluginPtr = ptr;
}

inline void Mechy::pushResponderPtr(ResponderPtr* ptr) {
    ptr->next = firstResponderPtr;
    firstResponderPtr = ptr;
}

inline void Mechy::pushKBDPtr(EventPtr* ptr) {
    ptr->next = firstEventPtr;
    firstEventPtr = ptr;
}

inline EventPtr* Mechy::removeKBDPtr(EventPtr* ptr) {
    if (!firstEventPtr || firstEventPtr == ptr) {
        firstEventPtr = ptr->next;
        free(ptr);
        return firstEventPtr;
    }

    EventPtr* kbdPtr = firstEventPtr;
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

bool EventPtr::matches(Layout* layout, uint8_t row, uint8_t col) {
    return this->layout == layout && this->row == row && this->col == col;
}
