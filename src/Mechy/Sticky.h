#pragma once

#include "Mechy.h"

enum {
    STK_SFT,
    STK_CTL,
    STK_ALT,
    STK_GUI,
    STK_HYP,
    STK_MEH,
    STK_NAV,
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
    uint8_t defaultName();
    void tick();
    bool override(uint8_t name, Event* event, Plugin* plugin);
    void run(Event* event);

protected:
    bool should_clear;
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

#define ST_HYP { .name = FN_STICKY, .key = STK_HYP }
#define ST_MEH { .name = FN_STICKY, .key = STK_MEH }
#define ST_NAV { .name = FN_STICKY, .key = STK_NAV }
