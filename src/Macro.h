#pragma once

#include "Mechy.h"

enum {
    MCR_ZERO = 2048
};

class Macro: public Plugin {
public:
    Macro(uint8_t count, const char **macros);
    void process_event(Event *event);

protected:
    uint8_t prev_macro;
    uint8_t count;
    char **macros;
};

#define MACRO(n) (MCR_ZERO + n)
#define MM_0 { .name = FN_MACRO, .key = MACRO(0), .isPressed = false, .started = 0 }
#define MM_1 { .name = FN_MACRO, .key = MACRO(1), .isPressed = false, .started = 0 }
#define MM_2 { .name = FN_MACRO, .key = MACRO(2), .isPressed = false, .started = 0 }
#define MM_3 { .name = FN_MACRO, .key = MACRO(3), .isPressed = false, .started = 0 }
#define MM_4 { .name = FN_MACRO, .key = MACRO(4), .isPressed = false, .started = 0 }
#define MM_5 { .name = FN_MACRO, .key = MACRO(5), .isPressed = false, .started = 0 }
#define MM_6 { .name = FN_MACRO, .key = MACRO(6), .isPressed = false, .started = 0 }
#define MM_7 { .name = FN_MACRO, .key = MACRO(7), .isPressed = false, .started = 0 }
#define MM_8 { .name = FN_MACRO, .key = MACRO(8), .isPressed = false, .started = 0 }
#define MM_9 { .name = FN_MACRO, .key = MACRO(9), .isPressed = false, .started = 0 }
