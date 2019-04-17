#include <Keyboard.h>
#include "Password.h"


Password::Password(uint8_t _count, const char** _macros) {
    prev_macro = _count;
    count = _count;
    macros = (char**)_macros;
}

uint8_t Password::defaultName() {
    return FN_PASSWORD;
}

void Password::run(Event* event) {
    uint8_t macro_index = event->key();

    if (macro_index >= count) {
        prev_macro = count;
        return;
    }

    if (event->isPressed()) {
        if (prev_macro != macro_index) {
            prev_macro = macro_index;
        }
        else {
            uint16_t mods = mechy->currentModifiers();
            mechy->clearModifiers();
            Keyboard.print(macros[macro_index]);
            mechy->updateModifiers(mods);
            prev_macro = count;
            event->setIsActive(true);
        }
    }
}
