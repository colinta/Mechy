#include <Keyboard.h>
#include "KeyPress.h"


Keypress::Keypress() {
}

void Keypress::process_begin() {
    Keyboard.begin();
}

void Keypress::process_event(Event *event) {
    if (event->isPressed()) {
        Keyboard.press(event->key);
    }
    else if (event->isReleased()) {
        Keyboard.release(event->key);
    }
}
