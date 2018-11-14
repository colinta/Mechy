#include "KeyPress.h"


Keypress::Keypress() {
}

uint8_t Keypress::defaultName() {
    return FN_KEYPRESS;
}

void Keypress::run(Event* event) {
    uint16_t mods = (event->key & (MOD_LSFT | MOD_LGUI | MOD_LCTL | MOD_LALT));

    if (event->isPressed()) {
        if (mods & MOD_LSFT) { mechy->sendKeyboardPress(KEY_LEFT_SHIFT); }
        if (mods & MOD_LGUI) { mechy->sendKeyboardPress(KEY_LEFT_GUI); }
        if (mods & MOD_LCTL) { mechy->sendKeyboardPress(KEY_LEFT_CTRL); }
        if (mods & MOD_LALT) { mechy->sendKeyboardPress(KEY_LEFT_ALT); }

        mechy->sendKeyboardPress(event->key & 0b11111111);
    }
    else if (event->isReleased()) {
        mechy->sendKeyboardRelease(event->key & 0b11111111);

        if (mods & MOD_LSFT) { mechy->sendKeyboardRelease(KEY_LEFT_SHIFT); }
        if (mods & MOD_LGUI) { mechy->sendKeyboardRelease(KEY_LEFT_GUI); }
        if (mods & MOD_LCTL) { mechy->sendKeyboardRelease(KEY_LEFT_CTRL); }
        if (mods & MOD_LALT) { mechy->sendKeyboardRelease(KEY_LEFT_ALT); }
    }
}
