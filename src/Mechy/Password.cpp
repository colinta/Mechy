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

bool Password::override(uint8_t name, Event* event, Plugin* UNUSED(plugin)) {
    if (event->isHeld()) { return KBD_CONTINUE; }

    if (name != FN_PASSWORD) {
        prev_macro = count;
    }
    return KBD_CONTINUE;
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
            if (!(mods & MOD_SHIFT)) {
                // send "\n" unless Shift is pressed
                Keyboard.print('\n');
            }
            mechy->updateModifiers(mods);
            prev_macro = count;
            event->setIsActive(true);
        }
    }
}
