#pragma once

#include "Mechy.h"

enum {
    LOCK_1,
    LOCK_2,
};

class Lock: public Plugin {
public:
    Lock();
    bool override(uint8_t name, Event *event);
    void run(Event *event);

protected:
    bool is_locked;
    uint8_t lock_count;
};

#define LK_1 { .name = FN_LOCK, .key = LOCK_1, .isPressed = false, .started = 0 }
#define LK_2 { .name = FN_LOCK, .key = LOCK_2, .isPressed = false, .started = 0 }
