#pragma once

#include "Mechy.h"

enum {
    MCR_ZERO
};

class Macro: public Plugin {
public:
    Macro(uint8_t count, const char **macros);
    void run(Event* event);

protected:
    uint8_t prev_macro;
    uint8_t count;
    char **macros;
};

#define MM(n) { .name = FN_MACRO, .key = (MCR_ZERO + n) }
#define MM_0 MM(0)
#define MM_1 MM(1)
#define MM_2 MM(2)
#define MM_3 MM(3)
#define MM_4 MM(4)
#define MM_5 MM(5)
#define MM_6 MM(6)
#define MM_7 MM(7)
#define MM_8 MM(8)
#define MM_9 MM(9)
