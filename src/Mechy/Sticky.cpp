#include <Keyboard.h>
#include "Sticky.h"


#define KEY_BIT_LSFT 1
#define KEY_BIT_LCTL 2
#define KEY_BIT_LALT 4
#define KEY_BIT_LGUI 8


Sticky::Sticky() {
  hyper_state = 0;
  hyper_down = 0;
  mods_down_state = 0;
  sticky_state = 0;
  sticky_lock = 0;
  prev_mods = 0;
  should_clear = false;
}

void Sticky::process_tick() {
    if (should_clear) {
        clear_sticky_mods();
        should_clear = false;
    }
}

bool Sticky::process_override(uint8_t name, Event *event) {
    if (name != FN_STICKY && event->isPressed() && (hyper_state || sticky_state)) {
        should_clear = true;
    }
    return KBD_CONTINUE;
}

void Sticky::process_event(Event *event) {
    uint8_t modkey_mask = 0;
    switch (event->key) {
        case STK_SFT:
        case STK_CTL:
        case STK_ALT:
        case STK_GUI:
            modkey_mask = mod_bit(event->key);
            break;
        case HYPER:
            if (event->isPressed()) {
                sticky_state = 0;
                sticky_lock = 0;
                hyper_state = !hyper_state;
            }
            hyper_down = event->isDown();
            break;
    }

    if (modkey_mask) {
        if (event->isPressed()) {
            if (sticky_lock & modkey_mask) {
                sticky_state &= ~modkey_mask;
                sticky_lock  &= ~modkey_mask;
            }
            else if (sticky_state & modkey_mask) {
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
        else if (event->isHeld() && event->duration > STICKY_AUTO_OFF) {
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
    }

    update_mods();
}


uint8_t Sticky::mod_bit(uint16_t key) {
    switch (key) {
        case STK_SFT: case KEY_LEFT_SHIFT:  return KEY_BIT_LSFT;
        case STK_CTL: case KEY_LEFT_CTRL:   return KEY_BIT_LCTL;
        case STK_ALT: case KEY_LEFT_ALT:    return KEY_BIT_LALT;
        case STK_GUI: case KEY_LEFT_GUI:    return KEY_BIT_LGUI;
    }
    return 0;
}

uint8_t Sticky::current_mods() {
    if (hyper_state || hyper_down) {
        return KEY_BIT_LCTL | KEY_BIT_LALT | KEY_BIT_LGUI;
    }
    return mods_down_state | sticky_state | sticky_lock;
}

void Sticky::update_mods() {
    uint8_t current = current_mods();
    uint16_t mods[] = { KEY_LEFT_SHIFT, KEY_LEFT_CTRL, KEY_LEFT_ALT, KEY_LEFT_GUI };
    for (uint8_t mod_index = 0; mod_index < 4; mod_index++ ) {
        uint8_t mod_key = mods[mod_index];
        uint8_t mod_mask = mod_bit(mod_key);

        if ( (prev_mods & mod_mask) == (current & mod_mask)) {
            continue;
        }

        if (prev_mods & mod_mask) {
            Keyboard.release(mod_key);
        }
        else {
            Keyboard.press(mod_key);
        }
    }
    prev_mods = current;
}

void Sticky::clear_sticky_mods() {
    sticky_state = 0;
    hyper_state = false;
    update_mods();
}
