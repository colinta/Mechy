#pragma once

#include <Keyboard.h>
#include "Mechy.h"

class Keypress: public Plugin {
public:
    Keypress();
    void begin();
    void run(Event *event);

protected:
};

#define KC(k) { .name = FN_KEYPRESS, .key = k }

#define KC_A KC('a')
#define KC_B KC('b')
#define KC_C KC('c')
#define KC_D KC('d')
#define KC_E KC('e')
#define KC_F KC('f')
#define KC_G KC('g')
#define KC_H KC('h')
#define KC_I KC('i')
#define KC_J KC('j')
#define KC_K KC('k')
#define KC_L KC('l')
#define KC_M KC('m')
#define KC_N KC('n')
#define KC_O KC('o')
#define KC_P KC('p')
#define KC_Q KC('q')
#define KC_R KC('r')
#define KC_S KC('s')
#define KC_T KC('t')
#define KC_U KC('u')
#define KC_V KC('v')
#define KC_W KC('w')
#define KC_X KC('x')
#define KC_Y KC('y')
#define KC_Z KC('z')

#define KC_0 KC('0')
#define KC_1 KC('1')
#define KC_2 KC('2')
#define KC_3 KC('3')
#define KC_4 KC('4')
#define KC_5 KC('5')
#define KC_6 KC('6')
#define KC_7 KC('7')
#define KC_8 KC('8')
#define KC_9 KC('9')

#define KC_GRV KC('`')
#define KC_MINS KC('-')
#define KC_EQL KC('=')
#define KC_SCLN KC(';')
#define KC_LBRK KC('[')
#define KC_RBRK KC(']')
#define KC_APOS KC('\'')
#define KC_BSLH KC('\\')
#define KC_COMM KC(',')
#define KC_PERD KC('.')
#define KC_FSLH KC('/')
#define KC_SPC KC(' ')

#define KC_F1 KC(KEY_F1)
#define KC_F2 KC(KEY_F2)
#define KC_F3 KC(KEY_F3)
#define KC_F4 KC(KEY_F4)
#define KC_F5 KC(KEY_F5)
#define KC_F6 KC(KEY_F6)
#define KC_F7 KC(KEY_F7)
#define KC_F8 KC(KEY_F8)
#define KC_F9 KC(KEY_F9)
#define KC_F10 KC(KEY_F10)
#define KC_F11 KC(KEY_F11)
#define KC_F12 KC(KEY_F12)

#define KC_ESC KC(KEY_ESC)
#define KC_DEL KC(KEY_DELETE)
#define KC_BSPC KC(KEY_BACKSPACE)
#define KC_ENT KC(KEY_RETURN)
#define KC_TAB KC(KEY_TAB)

#define KC_LT KC(KEY_LEFT_ARROW)
#define KC_RT KC(KEY_RIGHT_ARROW)
#define KC_UP KC(KEY_UP_ARROW)
#define KC_DN KC(KEY_DOWN_ARROW)
#define KC_PGUP KC(KEY_PAGE_UP)
#define KC_PGDN KC(KEY_PAGE_DOWN)
#define KC_HOME KC(KEY_HOME)
#define KC_END KC(KEY_END)

#define KC_LSFT KC(KEY_LEFT_SHIFT)
#define KC_LCTL KC(KEY_LEFT_CTRL)
#define KC_LALT KC(KEY_LEFT_ALT)
#define KC_LGUI KC(KEY_LEFT_GUI)
#define KC_RSFT KC(KEY_RIGHT_SHIFT)
#define KC_RCTL KC(KEY_RIGHT_CTRL)
#define KC_RALT KC(KEY_RIGHT_ALT)
#define KC_RGUI KC(KEY_RIGHT_GUI)
