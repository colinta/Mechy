#include "KeyPress.h"


Keypress::Keypress() {
}

void Keypress::run(Event *event) {
    if (event->isPressed()) {
        mechy->sendKeyboardPress(event->key);
    }
    else if (event->isReleased()) {
        mechy->sendKeyboardRelease(event->key);
    }
}
