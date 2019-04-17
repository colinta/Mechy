#pragma once

#include "Mechy.h"

class Password: public Plugin {
public:
    Password(uint8_t count, const char** macros);
    uint8_t defaultName();
    void run(Event* event);

protected:
    uint8_t prev_macro;
    uint8_t count;
    char** macros;
};

#define PW(n) { .name = FN_PASSWORD, .key = (n) }
#define PW_0 PW(0)
#define PW_1 PW(1)
#define PW_2 PW(2)
#define PW_3 PW(3)
#define PW_4 PW(4)
#define PW_5 PW(5)
#define PW_6 PW(6)
#define PW_7 PW(7)
#define PW_8 PW(8)
#define PW_9 PW(9)
