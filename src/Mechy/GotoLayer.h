#pragma once

#include "Mechy.h"

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

#define MO(n) { .name = FN_GOTO_LAYER, .key = (n) | (GO_MOMENTARY << EVENT_DATA_SHFT) }
#define MO_0 MO(0)
#define MO_1 MO(1)
#define MO_2 MO(2)
#define MO_3 MO(3)
#define LOWER MO(1)
#define RAISE MO(2)
