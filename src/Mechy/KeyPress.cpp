#include "KeyPress.h"


Keypress::Keypress() {
}

void Keypress::begin() {
    Keyboard.begin();
}

void Keypress::run(Event *event) {
    if (event->isPressed()) {
        Keyboard.press(event->key);
    }
    else if (event->isReleased()) {
        Keyboard.release(event->key);
    }
}
