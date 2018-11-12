#include "KeyPress.h"


Keypress::Keypress() {
}

uint8_t Keypress::defaultName() {
    return FN_KEYPRESS;
}

void Keypress::run(Event* event) {
    if (event->isPressed()) {
        mechy->sendKeyboardPress(event->key);
    }
    else if (event->isReleased()) {
        mechy->sendKeyboardRelease(event->key);
    }
}
