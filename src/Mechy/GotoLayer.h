#pragma once

#include "Mechy.h"

enum {
    GO_MOMENTARY,
    GO_STICKY,
    GO_NOW,
};

class GotoLayer : public Plugin {
public:
    GotoLayer();
    uint8_t defaultName();
    bool is(uint8_t event_type, Event* event);
    bool override(uint8_t name, Event* event, Plugin* plugin);
    void run(Event* event);
};

#define MO(n) { .name = FN_GOTO_LAYER, .key = (n) | (GO_MOMENTARY << EVENT_DATA_SHIFT) }
#define MS(n) { .name = FN_GOTO_LAYER, .key = (n) | (GO_STICKY << EVENT_DATA_SHIFT) }
#define TO(n) { .name = FN_GOTO_LAYER, .key = (n) | (GO_NOW << EVENT_DATA_SHIFT) }
#define MO_0 MO(0)
#define MO_1 MO(1)
#define MO_2 MO(2)
#define MO_3 MO(3)
#define MS_0 MS(0)
#define MS_1 MS(1)
#define MS_2 MS(2)
#define MS_3 MS(3)
#define TO_0 TO(0)
#define TO_1 TO(1)
#define TO_2 TO(2)
#define TO_3 TO(3)

#define LOWER MS(1)
#define RAISE MS(2)
#define DEFLT TO(0)
