#include "Mechy.h"

#include <Keyboard.h>

Mechy::Mechy() {
    modifiers = 0;
    capsIsOn = false;
    layerStackPtr = NULL;
    firstResponderPtr = NULL;
    firstPluginPtr = NULL;
    firstKBDPtr = NULL;
    event = { .key = MCHY_NONE, .keyState = KEY_STATE_NONE, .duration = 0 };
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

void Mechy::processKeyEvent(Layout* layout, uint8_t row, uint8_t col, bool isPressed) {
    unsigned long now = millis();

    // find the prev key event data, if present, and trim away any key events
    // that are finished (!isPressed) and old (finished > 10ms ago).
    KBDDataPtr* findPtr = firstKBDPtr;
    KBDDataPtr* kbdData = NULL;
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
        KBDDataPtr* ptr = NULL;
        KBD* kbd = layout->getKey(row, col);

        if (kbd && kbdData) {
            ptr = kbdData;
        }
        else if (kbd) {
            ptr = (KBDDataPtr*)malloc(sizeof(KBDDataPtr));
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
    event.key = kbd->key;
    event.keyState = keyState;
    event.duration = duration;

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
    if (keyState == KEY_STATE_PRESSED) {
        ptr = firstPluginPtr;
        while (ptr) {
            processing = ptr->plugin->override(keyHandlerName, &event, plugin) && processing;
            ptr = ptr->next;
        }
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

void Mechy::sendKeyboardPress(uint8_t key) {
    uint16_t modMask = 0;
    uint16_t modShft = 0;
    switch (key) {
    case KEY_LEFT_CTRL:
        modMask = MCHY_MASK_L_CTRL;
        modShft = MCHY_BITL_L_CTRL;
        break;
    case KEY_RIGHT_CTRL:
        modMask = MCHY_MASK_R_CTRL;
        modShft = MCHY_BITL_R_CTRL;
        break;
    case KEY_LEFT_ALT:
        modMask = MCHY_MASK_L_ALT;
        modShft = MCHY_BITL_L_ALT;
        break;
    case KEY_RIGHT_ALT:
        modMask = MCHY_MASK_R_ALT;
        modShft = MCHY_BITL_R_ALT;
        break;
    case KEY_LEFT_GUI:
        modMask = MCHY_MASK_L_GUI;
        modShft = MCHY_BITL_L_GUI;
        break;
    case KEY_RIGHT_GUI:
        modMask = MCHY_MASK_R_GUI;
        modShft = MCHY_BITL_R_GUI;
        break;
    case KEY_LEFT_SHIFT:
        modMask = MCHY_MASK_L_SHIFT;
        modShft = MCHY_BITL_L_SHIFT;
        break;
    case KEY_RIGHT_SHIFT:
        modMask = MCHY_MASK_R_SHIFT;
        modShft = MCHY_BITL_R_SHIFT;
        break;
    case KEY_CAPS_LOCK:
        capsIsOn = !capsIsOn;
        break;
    }

    if (modMask) {
        uint16_t count = (modifiers & modMask) >> modShft;
        if (count < 3) {
            count += 1;
            modifiers |= (count << modShft);
            if (count == 1) {
                Keyboard.press(key);
            }
        }
    }
    else {
        Keyboard.press(key);
    }
}

void Mechy::sendKeyboardRelease(uint8_t key) {
    uint16_t modMask = 0;
    uint16_t modShft = 0;
    switch (key) {
    case KEY_LEFT_CTRL:
        modMask = MCHY_MASK_L_CTRL;
        modShft = MCHY_BITL_L_CTRL;
        break;
    case KEY_RIGHT_CTRL:
        modMask = MCHY_MASK_R_CTRL;
        modShft = MCHY_BITL_R_CTRL;
        break;
    case KEY_LEFT_ALT:
        modMask = MCHY_MASK_L_ALT;
        modShft = MCHY_BITL_L_ALT;
        break;
    case KEY_RIGHT_ALT:
        modMask = MCHY_MASK_R_ALT;
        modShft = MCHY_BITL_R_ALT;
        break;
    case KEY_LEFT_GUI:
        modMask = MCHY_MASK_L_GUI;
        modShft = MCHY_BITL_L_GUI;
        break;
    case KEY_RIGHT_GUI:
        modMask = MCHY_MASK_R_GUI;
        modShft = MCHY_BITL_R_GUI;
        break;
    case KEY_LEFT_SHIFT:
        modMask = MCHY_MASK_L_SHIFT;
        modShft = MCHY_BITL_L_SHIFT;
        break;
    case KEY_RIGHT_SHIFT:
        modMask = MCHY_MASK_R_SHIFT;
        modShft = MCHY_BITL_R_SHIFT;
        break;
    }

    if (modMask) {
        uint16_t count = (modifiers & modMask) >> modShft;
        if (count > 0) {
            count -= 1;
            modifiers &= ~(0b11 << modShft);
            modifiers |= (count << modShft);
            if (count == 0) {
                Keyboard.release(key);
            }
        }
    }
    else {
        Keyboard.release(key);
    }
}

uint16_t Mechy::currentModifiers() {
    return modifiers;
}

void Mechy::updateModifiers(uint16_t newModifiers) {
    if ((newModifiers & MCHY_MASK_L_CTRL) && !(modifiers & MCHY_MASK_L_CTRL)) {
        Keyboard.press(KEY_LEFT_CTRL);
    }
    else if (!(newModifiers & MCHY_MASK_L_CTRL) && (modifiers & MCHY_MASK_L_CTRL)) {
        Keyboard.release(KEY_LEFT_CTRL);
    }

    if ((newModifiers & MCHY_MASK_R_CTRL) && !(modifiers & MCHY_MASK_R_CTRL)) {
        Keyboard.press(KEY_RIGHT_CTRL);
    }
    else if (!(newModifiers & MCHY_MASK_R_CTRL) && (modifiers & MCHY_MASK_R_CTRL)) {
        Keyboard.release(KEY_RIGHT_CTRL);
    }

    if ((newModifiers & MCHY_MASK_L_ALT) && !(modifiers & MCHY_MASK_L_ALT)) {
        Keyboard.press(KEY_LEFT_ALT);
    }
    else if (!(newModifiers & MCHY_MASK_L_ALT) && (modifiers & MCHY_MASK_L_ALT)) {
        Keyboard.release(KEY_LEFT_ALT);
    }

    if ((newModifiers & MCHY_MASK_R_ALT) && !(modifiers & MCHY_MASK_R_ALT)) {
        Keyboard.press(KEY_RIGHT_ALT);
    }
    else if (!(newModifiers & MCHY_MASK_R_ALT) && (modifiers & MCHY_MASK_R_ALT)) {
        Keyboard.release(KEY_RIGHT_ALT);
    }

    if ((newModifiers & MCHY_MASK_L_GUI) && !(modifiers & MCHY_MASK_L_GUI)) {
        Keyboard.press(KEY_LEFT_GUI);
    }
    else if (!(newModifiers & MCHY_MASK_L_GUI) && (modifiers & MCHY_MASK_L_GUI)) {
        Keyboard.release(KEY_LEFT_GUI);
    }

    if ((newModifiers & MCHY_MASK_R_GUI) && !(modifiers & MCHY_MASK_R_GUI)) {
        Keyboard.press(KEY_RIGHT_GUI);
    }
    else if (!(newModifiers & MCHY_MASK_R_GUI) && (modifiers & MCHY_MASK_R_GUI)) {
        Keyboard.release(KEY_RIGHT_GUI);
    }

    if ((newModifiers & MCHY_MASK_L_SHIFT) && !(modifiers & MCHY_MASK_L_SHIFT)) {
        Keyboard.press(KEY_LEFT_SHIFT);
    }
    else if (!(newModifiers & MCHY_MASK_L_SHIFT) && (modifiers & MCHY_MASK_L_SHIFT)) {
        Keyboard.release(KEY_LEFT_SHIFT);
    }

    if ((newModifiers & MCHY_MASK_R_SHIFT) && !(modifiers & MCHY_MASK_R_SHIFT)) {
        Keyboard.press(KEY_RIGHT_SHIFT);
    }
    else if (!(newModifiers & MCHY_MASK_R_SHIFT) && (modifiers & MCHY_MASK_R_SHIFT)) {
        Keyboard.release(KEY_RIGHT_SHIFT);
    }

    modifiers = newModifiers;
}

void Mechy::clearModifiers() {
    if (modifiers & MCHY_MASK_L_CTRL) {
        Keyboard.release(KEY_LEFT_CTRL);
    }
    if (modifiers & MCHY_MASK_R_CTRL) {
        Keyboard.release(KEY_RIGHT_CTRL);
    }
    if (modifiers & MCHY_MASK_L_ALT) {
        Keyboard.release(KEY_LEFT_ALT);
    }
    if (modifiers & MCHY_MASK_R_ALT) {
        Keyboard.release(KEY_RIGHT_ALT);
    }
    if (modifiers & MCHY_MASK_L_GUI) {
        Keyboard.release(KEY_LEFT_GUI);
    }
    if (modifiers & MCHY_MASK_R_GUI) {
        Keyboard.release(KEY_RIGHT_GUI);
    }
    if (modifiers & MCHY_MASK_L_SHIFT) {
        Keyboard.release(KEY_LEFT_SHIFT);
    }
    if (modifiers & MCHY_MASK_R_SHIFT) {
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

inline void Mechy::pushKBDPtr(KBDDataPtr* ptr) {
    ptr->next = firstKBDPtr;
    firstKBDPtr = ptr;
}

inline KBDDataPtr* Mechy::removeKBDPtr(KBDDataPtr* ptr) {
    if (!firstKBDPtr || firstKBDPtr == ptr) {
        firstKBDPtr = ptr->next;
        free(ptr);
        return firstKBDPtr;
    }

    KBDDataPtr* kbdPtr = firstKBDPtr;
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

bool KBDDataPtr::matches(Layout* layout, uint8_t row, uint8_t col) {
    return this->layout == layout && this->row == row && this->col == col;
}

