#pragma once

#include "Mechy.h"

enum {
    STK_SFT,
    STK_CTL,
    STK_ALT,
    STK_GUI,
    HYPER,
};


#ifndef STICKY_LOCK_DELAY
#define STICKY_LOCK_DELAY 250
#endif

#ifndef STICKY_AUTO_OFF
#define STICKY_AUTO_OFF 250
#endif

class Sticky: public Plugin {
public:
    Sticky();
    void process_tick();
    bool process_override(uint8_t name, Event *event);
    void process_event(Event *event);
    uint8_t mod_bit(uint16_t key);
    uint8_t current_mods();

protected:
    bool should_clear;
    bool hyper_state;
    bool hyper_down;
    uint8_t mods_down_state;
    uint8_t sticky_state;
    uint8_t sticky_lock;
    uint8_t prev_mods;

    unsigned long sticky_lock_timer;
    unsigned long sticky_auto_off_timer;

    void update_mods();
    void clear_sticky_mods();
};

#define ST_SFT { .name = FN_STICKY, .key = STK_SFT, .isPressed = false, .started = 0 }
#define ST_CTL { .name = FN_STICKY, .key = STK_CTL, .isPressed = false, .started = 0 }
#define ST_ALT { .name = FN_STICKY, .key = STK_ALT, .isPressed = false, .started = 0 }
#define ST_GUI { .name = FN_STICKY, .key = STK_GUI, .isPressed = false, .started = 0 }
#define ST_HYP { .name = FN_STICKY, .key = HYPER, .isPressed = false, .started = 0 }
