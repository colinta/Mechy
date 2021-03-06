#include "KeyPress.h"


KeyPress::KeyPress() {
}

uint8_t KeyPress::defaultName() {
    return FN_KEYPRESS;
}

bool KeyPress::is(uint8_t event_type, Event* event) {
    // does this key have modifiers in the data channel?  if so, match EVENT_MODIFIER
    if (event->data() & DATA_MOD_ANY && event_type == EVENT_MODIFIER)  return true;

    // is this a single modifier key?  only match EVENT_MODIFIER
    else if (
        event->key() == KEY_LEFT_SHIFT ||
        event->key() == KEY_LEFT_CTRL ||
        event->key() == KEY_LEFT_ALT ||
        event->key() == KEY_LEFT_GUI ||
        event->key() == KEY_RIGHT_SHIFT ||
        event->key() == KEY_RIGHT_CTRL ||
        event->key() == KEY_RIGHT_ALT ||
        event->key() == KEY_RIGHT_GUI)
    {
        return event_type == EVENT_MODIFIER;
    }

    // lastly if there is anything in the key mask, match EVENT_KEYPRESS
    return event->key() && event_type == EVENT_KEYPRESS;
}

void KeyPress::run(Event* event) {
    if (event->isPressed()) {
        if (event->data() & DATA_MOD_LSFT) { mechy->pressKey(KEY_LEFT_SHIFT); }
        if (event->data() & DATA_MOD_LGUI) { mechy->pressKey(KEY_LEFT_GUI); }
        if (event->data() & DATA_MOD_LCTL) { mechy->pressKey(KEY_LEFT_CTRL); }
        if (event->data() & DATA_MOD_LALT) { mechy->pressKey(KEY_LEFT_ALT); }

        mechy->pressKey(event->key());
    }
    else if (event->isReleased()) {
        mechy->releaseKey(event->key());

        if (event->data() & DATA_MOD_LSFT) { mechy->releaseKey(KEY_LEFT_SHIFT); }
        if (event->data() & DATA_MOD_LGUI) { mechy->releaseKey(KEY_LEFT_GUI); }
        if (event->data() & DATA_MOD_LCTL) { mechy->releaseKey(KEY_LEFT_CTRL); }
        if (event->data() & DATA_MOD_LALT) { mechy->releaseKey(KEY_LEFT_ALT); }
    }
}
