#pragma once

#include "Mechy.h"

class Keypress: public Plugin {
public:
    Keypress();
    void process_begin();
    void process_event(Event *event);

protected:
};

#define KC_A { .name = FN_KEYPRESS, .key = 'a', .isPressed = false, .started = 0 }
#define KC_B { .name = FN_KEYPRESS, .key = 'b', .isPressed = false, .started = 0 }
#define KC_C { .name = FN_KEYPRESS, .key = 'c', .isPressed = false, .started = 0 }
#define KC_D { .name = FN_KEYPRESS, .key = 'd', .isPressed = false, .started = 0 }
#define KC_E { .name = FN_KEYPRESS, .key = 'e', .isPressed = false, .started = 0 }
#define KC_F { .name = FN_KEYPRESS, .key = 'f', .isPressed = false, .started = 0 }
#define KC_G { .name = FN_KEYPRESS, .key = 'g', .isPressed = false, .started = 0 }
#define KC_H { .name = FN_KEYPRESS, .key = 'h', .isPressed = false, .started = 0 }
#define KC_I { .name = FN_KEYPRESS, .key = 'i', .isPressed = false, .started = 0 }
#define KC_J { .name = FN_KEYPRESS, .key = 'j', .isPressed = false, .started = 0 }
#define KC_K { .name = FN_KEYPRESS, .key = 'k', .isPressed = false, .started = 0 }
#define KC_L { .name = FN_KEYPRESS, .key = 'l', .isPressed = false, .started = 0 }
#define KC_M { .name = FN_KEYPRESS, .key = 'm', .isPressed = false, .started = 0 }
#define KC_N { .name = FN_KEYPRESS, .key = 'n', .isPressed = false, .started = 0 }
#define KC_O { .name = FN_KEYPRESS, .key = 'o', .isPressed = false, .started = 0 }
#define KC_P { .name = FN_KEYPRESS, .key = 'p', .isPressed = false, .started = 0 }
#define KC_Q { .name = FN_KEYPRESS, .key = 'q', .isPressed = false, .started = 0 }
#define KC_R { .name = FN_KEYPRESS, .key = 'r', .isPressed = false, .started = 0 }
#define KC_S { .name = FN_KEYPRESS, .key = 's', .isPressed = false, .started = 0 }
#define KC_T { .name = FN_KEYPRESS, .key = 't', .isPressed = false, .started = 0 }
#define KC_U { .name = FN_KEYPRESS, .key = 'u', .isPressed = false, .started = 0 }
#define KC_V { .name = FN_KEYPRESS, .key = 'v', .isPressed = false, .started = 0 }
#define KC_W { .name = FN_KEYPRESS, .key = 'w', .isPressed = false, .started = 0 }
#define KC_X { .name = FN_KEYPRESS, .key = 'x', .isPressed = false, .started = 0 }
#define KC_Y { .name = FN_KEYPRESS, .key = 'y', .isPressed = false, .started = 0 }
#define KC_Z { .name = FN_KEYPRESS, .key = 'z', .isPressed = false, .started = 0 }

#define KC_0 { .name = FN_KEYPRESS, .key = '0', .isPressed = false, .started = 0 }
#define KC_1 { .name = FN_KEYPRESS, .key = '1', .isPressed = false, .started = 0 }
#define KC_2 { .name = FN_KEYPRESS, .key = '2', .isPressed = false, .started = 0 }
#define KC_3 { .name = FN_KEYPRESS, .key = '3', .isPressed = false, .started = 0 }
#define KC_4 { .name = FN_KEYPRESS, .key = '4', .isPressed = false, .started = 0 }
#define KC_5 { .name = FN_KEYPRESS, .key = '5', .isPressed = false, .started = 0 }
#define KC_6 { .name = FN_KEYPRESS, .key = '6', .isPressed = false, .started = 0 }
#define KC_7 { .name = FN_KEYPRESS, .key = '7', .isPressed = false, .started = 0 }
#define KC_8 { .name = FN_KEYPRESS, .key = '8', .isPressed = false, .started = 0 }
#define KC_9 { .name = FN_KEYPRESS, .key = '9', .isPressed = false, .started = 0 }

#define KC_GRV { .name = FN_KEYPRESS, .key = '`', .isPressed = false, .started = 0 }
#define KC_MINS { .name = FN_KEYPRESS, .key = '-', .isPressed = false, .started = 0 }
#define KC_EQL { .name = FN_KEYPRESS, .key = '=', .isPressed = false, .started = 0 }
#define KC_SCLN { .name = FN_KEYPRESS, .key = ';', .isPressed = false, .started = 0 }
#define KC_LBRK { .name = FN_KEYPRESS, .key = '[', .isPressed = false, .started = 0 }
#define KC_RBRK { .name = FN_KEYPRESS, .key = ']', .isPressed = false, .started = 0 }
#define KC_APOS { .name = FN_KEYPRESS, .key = '\'', .isPressed = false, .started = 0 }
#define KC_BSLH { .name = FN_KEYPRESS, .key = '\\', .isPressed = false, .started = 0 }
#define KC_COMM { .name = FN_KEYPRESS, .key = ',', .isPressed = false, .started = 0 }
#define KC_PERD { .name = FN_KEYPRESS, .key = '.', .isPressed = false, .started = 0 }
#define KC_FSLH { .name = FN_KEYPRESS, .key = '/', .isPressed = false, .started = 0 }

#define KC_F1 { .name = FN_KEYPRESS, .key = KEY_F1, .isPressed = false, .started = 0 }
#define KC_F2 { .name = FN_KEYPRESS, .key = KEY_F2, .isPressed = false, .started = 0 }
#define KC_F3 { .name = FN_KEYPRESS, .key = KEY_F3, .isPressed = false, .started = 0 }
#define KC_F4 { .name = FN_KEYPRESS, .key = KEY_F4, .isPressed = false, .started = 0 }
#define KC_F5 { .name = FN_KEYPRESS, .key = KEY_F5, .isPressed = false, .started = 0 }
#define KC_F6 { .name = FN_KEYPRESS, .key = KEY_F6, .isPressed = false, .started = 0 }
#define KC_F7 { .name = FN_KEYPRESS, .key = KEY_F7, .isPressed = false, .started = 0 }
#define KC_F8 { .name = FN_KEYPRESS, .key = KEY_F8, .isPressed = false, .started = 0 }
#define KC_F9 { .name = FN_KEYPRESS, .key = KEY_F9, .isPressed = false, .started = 0 }
#define KC_F10 { .name = FN_KEYPRESS, .key = KEY_F10, .isPressed = false, .started = 0 }
#define KC_F11 { .name = FN_KEYPRESS, .key = KEY_F11, .isPressed = false, .started = 0 }
#define KC_F12 { .name = FN_KEYPRESS, .key = KEY_F12, .isPressed = false, .started = 0 }

#define KC_ESC { .name = FN_KEYPRESS, .key = KEY_ESC, .isPressed = false, .started = 0 }
#define KC_DEL { .name = FN_KEYPRESS, .key = KEY_DELETE, .isPressed = false, .started = 0 }
#define KC_BSPC { .name = FN_KEYPRESS, .key = KEY_BACKSPACE, .isPressed = false, .started = 0 }
#define KC_ENT { .name = FN_KEYPRESS, .key = KEY_RETURN, .isPressed = false, .started = 0 }
#define KC_TAB { .name = FN_KEYPRESS, .key = KEY_TAB, .isPressed = false, .started = 0 }
#define KC_SPC { .name = FN_KEYPRESS, .key = ' ', .isPressed = false, .started = 0 }

#define KC_LT { .name = FN_KEYPRESS, .key = KEY_LEFT_ARROW, .isPressed = false, .started = 0 }
#define KC_RT { .name = FN_KEYPRESS, .key = KEY_RIGHT_ARROW, .isPressed = false, .started = 0 }
#define KC_UP { .name = FN_KEYPRESS, .key = KEY_UP_ARROW, .isPressed = false, .started = 0 }
#define KC_DN { .name = FN_KEYPRESS, .key = KEY_DOWN_ARROW, .isPressed = false, .started = 0 }
#define KC_PGUP { .name = FN_KEYPRESS, .key = KEY_PAGE_UP, .isPressed = false, .started = 0 }
#define KC_PGDN { .name = FN_KEYPRESS, .key = KEY_PAGE_DOWN, .isPressed = false, .started = 0 }
#define KC_HOME { .name = FN_KEYPRESS, .key = KEY_HOME, .isPressed = false, .started = 0 }
#define KC_END { .name = FN_KEYPRESS, .key = KEY_END, .isPressed = false, .started = 0 }

#define KC_LSFT { .name = FN_KEYPRESS, .key = KEY_LEFT_SHIFT, .isPressed = false, .started = 0 }
#define KC_LCTL { .name = FN_KEYPRESS, .key = KEY_LEFT_CTRL, .isPressed = false, .started = 0 }
#define KC_LALT { .name = FN_KEYPRESS, .key = KEY_LEFT_ALT, .isPressed = false, .started = 0 }
#define KC_LGUI { .name = FN_KEYPRESS, .key = KEY_LEFT_GUI, .isPressed = false, .started = 0 }
#define KC_RSFT { .name = FN_KEYPRESS, .key = KEY_RIGHT_SHIFT, .isPressed = false, .started = 0 }
#define KC_RCTL { .name = FN_KEYPRESS, .key = KEY_RIGHT_CTRL, .isPressed = false, .started = 0 }
#define KC_RALT { .name = FN_KEYPRESS, .key = KEY_RIGHT_ALT, .isPressed = false, .started = 0 }
#define KC_RGUI { .name = FN_KEYPRESS, .key = KEY_RIGHT_GUI, .isPressed = false, .started = 0 }
