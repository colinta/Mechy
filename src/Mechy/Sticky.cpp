#include <Keyboard.h>
#include "Sticky.h"


#define STK_BIT_LSFT 1
#define STK_BIT_LCTL 2
#define STK_BIT_LALT 4
#define STK_BIT_LGUI 8


Sticky::Sticky() {
    mods_down_state = 0;
    sticky_state = 0;
    sticky_lock = 0;
    prev_mods = 0;
    should_clear = false;
}

uint8_t Sticky::defaultName() {
    return FN_STICKY;
}

void Sticky::tick() {
    if (should_clear) {
        clearStickyMods();
        should_clear = false;
    }
}

bool Sticky::is(uint8_t event_type, Event* UNUSED(event)) {
    return event_type == EVENT_MODIFIER;
}

bool Sticky::override(Event* event, Plugin* plugin) {
    if ((plugin->is(EVENT_KEYPRESS, event) || plugin->is(EVENT_MOUSE, event)) && sticky_state) {
        should_clear = true;
    }
    return KBD_CONTINUE;
}

void Sticky::run(Event* event) {
    uint8_t modkey_mask = modBit(event->key());
    if (!modkey_mask)  return;

    if (event->isPressed()) {
        if (sticky_lock & modkey_mask) {
            sticky_state &= ~modkey_mask;
            sticky_lock  &= ~modkey_mask;
        }
        else if ((sticky_state & modkey_mask) == modkey_mask) {
             if ((millis() - sticky_lock_timer) < STICKY_LOCK_DELAY) {
                sticky_lock |= modkey_mask;
            }
            else {
                sticky_state &= ~modkey_mask;
            }
        }
        else {
            sticky_state |= modkey_mask;
            sticky_auto_off_timer = millis();
        }

        mods_down_state |= modkey_mask;
    }
    else if (event->isHeld() && event->duration() > STICKY_AUTO_OFF) {
        sticky_state &= ~modkey_mask;
    }
    else if (event->isReleased()) {
        if (sticky_state & modkey_mask) {
            sticky_lock_timer = millis();
        }
        mods_down_state &= ~modkey_mask;

        if (!mods_down_state) {
            sticky_auto_off_timer = 0;
        }
    }

    updateMods();
}


uint8_t Sticky::modBit(uint8_t key) {
    switch (key) {
    case STK_SFT: case KEY_LEFT_SHIFT:  return STK_BIT_LSFT;
    case STK_CTL: case KEY_LEFT_CTRL:   return STK_BIT_LCTL;
    case STK_ALT: case KEY_LEFT_ALT:    return STK_BIT_LALT;
    case STK_GUI: case KEY_LEFT_GUI:    return STK_BIT_LGUI;
    case STK_HYP:    return STK_BIT_LSFT | STK_BIT_LGUI | STK_BIT_LCTL | STK_BIT_LALT;
    case STK_MEH:    return STK_BIT_LSFT | STK_BIT_LCTL | STK_BIT_LALT;
    case STK_CAG:    return STK_BIT_LGUI | STK_BIT_LCTL | STK_BIT_LALT;
    }
    return 0;
}

uint8_t Sticky::currentMods() {
    return mods_down_state | sticky_state | sticky_lock;
}

void Sticky::updateMods() {
    uint8_t current = currentMods();
    uint16_t mods[] = { KEY_LEFT_SHIFT, KEY_LEFT_CTRL, KEY_LEFT_ALT, KEY_LEFT_GUI };
    for (uint8_t mod_index = 0; mod_index < 4; mod_index++ ) {
        uint8_t mod_key = mods[mod_index];
        uint8_t mod_mask = modBit(mod_key);

        if ( (prev_mods & mod_mask) == (current & mod_mask)) {
            continue;
        }

        if (prev_mods & mod_mask) {
            mechy->releaseKey(mod_key);
        }
        else {
            mechy->pressKey(mod_key);
        }
    }
    prev_mods = current;
}

void Sticky::clearStickyMods() {
    sticky_state = 0;
    updateMods();
}
