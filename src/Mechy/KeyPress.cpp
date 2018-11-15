#include "KeyPress.h"


KeyPress::KeyPress() {
}

uint8_t KeyPress::defaultName() {
    return FN_KEYPRESS;
}

bool KeyPress::is(uint8_t event_type, Event* event) {
    if (event->key & MOD_ANY && event_type == EVENT_MODIFIER)  return true;
    else if (
        event->key == KEY_LEFT_SHIFT ||
        event->key == KEY_LEFT_CTRL ||
        event->key == KEY_LEFT_ALT ||
        event->key == KEY_LEFT_GUI ||
        event->key == KEY_RIGHT_SHIFT ||
        event->key == KEY_RIGHT_CTRL ||
        event->key == KEY_RIGHT_ALT ||
        event->key == KEY_RIGHT_GUI)
    {
        return event_type == EVENT_MODIFIER;
    }

    return event_type == EVENT_KEYPRESS;
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
