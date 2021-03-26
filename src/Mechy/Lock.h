#pragma once

#include "Mechy.h"

class Lock: public Plugin {
public:
    Lock(uint8_t need = 2);
    uint8_t defaultName();
    bool is(uint8_t event_type, Event* event);
    bool override(Event* event, Plugin* plugin);
    void run(Event* event);

protected:
    bool is_locked;
    uint8_t need;
    uint8_t lock_count;
};

#define LK { .name = FN_LOCK, .key = 0 }
