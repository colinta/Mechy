#pragma once

#include "Mechy.h"

enum {
    GO_ZERO
};

enum {
    GO_MOMENTARY = 0,
};

class GotoLayer : public Plugin {
public:
    GotoLayer();
    uint8_t defaultName();
    bool is(uint8_t event_type, Event* event);
    void run(Event* event);
};

#define GOTO(n) { .name = FN_GOTO_LAYER, .key = (GO_ZERO + n) | (GO_MOMENTARY) }
#define GOTO_0 GOTO(0)
#define GOTO_1 GOTO(1)
#define GOTO_2 GOTO(2)
#define GOTO_3 GOTO(3)
#define LOWER GOTO(1)
#define RAISE GOTO(2)
