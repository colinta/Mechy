#pragma once

#include "Mechy.h"

enum {
    LOCK_1,
    LOCK_2,
};

class Lock: public Plugin {
public:
    Lock();
    uint8_t defaultName();
    bool override(uint8_t name, Event* event, Plugin* plugin);
    void run(Event* event);

protected:
    bool is_locked;
    uint8_t lock_count;
};

#define LK_1 { .name = FN_LOCK, .key = LOCK_1 }
#define LK_2 { .name = FN_LOCK, .key = LOCK_2 }
