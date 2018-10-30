#include <Keyboard.h>
#include "Macro.h"


Macro::Macro(uint8_t _count, const char **_macros) {
    prev_macro = _count;
    count = _count;
    macros = (char**)_macros;
}

void Macro::run(Event *event) {
    int16_t macro_index = event->key - MCR_ZERO;

    if (macro_index < 0 || macro_index >= count) {
        prev_macro = count;
        return;
    }

    if (event->isPressed()) {
        if (prev_macro != macro_index) {
            prev_macro = macro_index;
        }
        else {
            Keyboard.print(macros[macro_index]);
            prev_macro = count;
        }
    }
}