#include "KeyPress.h"


KeyPress::KeyPress() {
}

uint8_t KeyPress::defaultName() {
    return FN_KEYPRESS;
}

void KeyPress::run(Event* event) {
    if (event->isPressed()) {
        if (event->key & MOD_LSFT) { mechy->sendKeyboardPress(KEY_LEFT_SHIFT); }
        if (event->key & MOD_LGUI) { mechy->sendKeyboardPress(KEY_LEFT_GUI); }
        if (event->key & MOD_LCTL) { mechy->sendKeyboardPress(KEY_LEFT_CTRL); }
        if (event->key & MOD_LALT) { mechy->sendKeyboardPress(KEY_LEFT_ALT); }

        mechy->sendKeyboardPress(event->key & 0b11111111);
    }
    else if (event->isReleased()) {
        mechy->sendKeyboardRelease(event->key & 0b11111111);

        if (event->key & MOD_LSFT) { mechy->sendKeyboardRelease(KEY_LEFT_SHIFT); }
        if (event->key & MOD_LGUI) { mechy->sendKeyboardRelease(KEY_LEFT_GUI); }
        if (event->key & MOD_LCTL) { mechy->sendKeyboardRelease(KEY_LEFT_CTRL); }
        if (event->key & MOD_LALT) { mechy->sendKeyboardRelease(KEY_LEFT_ALT); }
    }
}
