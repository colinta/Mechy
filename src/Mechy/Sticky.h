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
    void tick();
    bool override(uint8_t name, Event *event);
    void run(Event *event);

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

    uint8_t modBit(uint16_t key);
    uint8_t currentMods();
    void updateMods();
    void clearStickyMods();
};

#define ST_SFT { .name = FN_STICKY, .key = STK_SFT }
#define ST_CTL { .name = FN_STICKY, .key = STK_CTL }
#define ST_ALT { .name = FN_STICKY, .key = STK_ALT }
#define ST_GUI { .name = FN_STICKY, .key = STK_GUI }
#define ST_HYP { .name = FN_STICKY, .key = HYPER }
