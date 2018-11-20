#pragma once

#include "Mechy.h"

enum {
    GO_MOMENTARY,
    GO_PUSH,
    GO_NOW,
    GO_BACK,
};

class GotoLayer : public Plugin {
public:
    GotoLayer();
    uint8_t defaultName();
    bool is(uint8_t event_type, Event* event);
    bool override(uint8_t name, Event* event, Plugin* plugin);
    void run(Event* event);
};

#define GOTO(n) { .name = FN_GOTO_LAYER, .key = (n) | (GO_MOMENTARY << EVENT_DATA_SHIFT) }
#define GOTO_0 GOTO(0)
#define GOTO_1 GOTO(1)
#define GOTO_2 GOTO(2)
#define GOTO_3 GOTO(3)

#define PUSH(n) { .name = FN_GOTO_LAYER, .key = (n) | (GO_PUSH << EVENT_DATA_SHIFT) }
#define PUSH_0 PUSH(0)
#define PUSH_1 PUSH(1)
#define PUSH_2 PUSH(2)
#define PUSH_3 PUSH(3)

#define LSET(n) { .name = FN_GOTO_LAYER, .key = (n) | (GO_NOW << EVENT_DATA_SHIFT) }
#define LSET_0 LSET(0)
#define LSET_1 LSET(1)
#define LSET_2 LSET(2)
#define LSET_3 LSET(3)

#define LOWER PUSH(1)
#define RAISE PUSH(2)
#define BACK { .name = FN_GOTO_LAYER, .key = (GO_BACK << EVENT_DATA_SHIFT) }
