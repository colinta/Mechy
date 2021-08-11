#pragma once

#include <Keyboard.h>
#include "Mechy.h"

class KeyPress: public Plugin {
public:
    KeyPress();
    uint8_t defaultName();
    bool is(uint8_t event_type, Event* event);
    void run(Event* event);
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

#define KC_NLCK KC(229)
#define KC_PSLS KC(220)
#define KC_PAST KC(221)
#define KC_PMNS KC(222)
#define KC_PPLS KC(223)
#define KC_PENT KC(224)
#define KC_PDOT KC(235)
#define KC_P0   KC(234)
#define KC_P1   KC(225)
#define KC_P2   KC(226)
#define KC_P3   KC(227)
#define KC_P4   KC(228)
#define KC_P5   KC(229)
#define KC_P6   KC(230)
#define KC_P7   KC(231)
#define KC_P8   KC(232)
#define KC_P9   KC(233)

#define KC_GRV KC('`')
#define KC_MINS KC('-')
#define KC_EQL KC('=')
#define KC_SCLN KC(';')
#define KC_LBRC KC('[')
#define KC_RBRC KC(']')
#define KC_QUOT KC('\'')
#define KC_BSLS KC('\\')
#define KC_COMM KC(',')
#define KC_DOT  KC('.')
#define KC_SLSH KC('/')
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
#define KC_F13 KC(KEY_F13)
#define KC_F14 KC(KEY_F14)
#define KC_F15 KC(KEY_F15)
#define KC_F16 KC(KEY_F16)
#define KC_F17 KC(KEY_F17)
#define KC_F18 KC(KEY_F18)
#define KC_F19 KC(KEY_F19)
#define KC_F20 KC(KEY_F20)
#define KC_F21 KC(KEY_F21)
#define KC_F22 KC(KEY_F22)
#define KC_F23 KC(KEY_F23)
#define KC_F24 KC(KEY_F24)

#define KC_ESC KC(KEY_ESC)
#define KC_INS KC(KEY_INSERT)
#define KC_DEL KC(KEY_DELETE)
#define KC_BSPC KC(KEY_BACKSPACE)
#define KC_ENT KC(KEY_RETURN)
#define KC_TAB KC(KEY_TAB)
#define KC_CAPS KC(KEY_CAPS_LOCK)

#define KC_LEFT KC(KEY_LEFT_ARROW)
#define KC_RGHT KC(KEY_RIGHT_ARROW)
#define KC_UP KC(KEY_UP_ARROW)
#define KC_DOWN KC(KEY_DOWN_ARROW)
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

#define KC_HYP KC(DATA_MOD_LSFT | DATA_MOD_LGUI | DATA_MOD_LCTL | DATA_MOD_LALT)
#define KC_MEH KC(DATA_MOD_LSFT | DATA_MOD_LCTL | DATA_MOD_LALT)
#define KC_CAG KC(DATA_MOD_LGUI | DATA_MOD_LCTL | DATA_MOD_LALT)

#define LSFT(k) ((k) | (DATA_MOD_LSFT << EVENT_DATA_SHIFT))
#define LGUI(k) ((k) | (DATA_MOD_LGUI << EVENT_DATA_SHIFT))
#define LCTL(k) ((k) | (DATA_MOD_LCTL << EVENT_DATA_SHIFT))
#define LALT(k) ((k) | (DATA_MOD_LALT << EVENT_DATA_SHIFT))

#define KC_TILD KC(LSFT('`'))
#define KC_EXLM KC(LSFT('1'))
#define KC_AT   KC(LSFT('2'))
#define KC_HASH KC(LSFT('3'))
#define KC_DLR  KC(LSFT('4'))
#define KC_PERC KC(LSFT('5'))
#define KC_CIRC KC(LSFT('6'))
#define KC_AMPR KC(LSFT('7'))
#define KC_ASTR KC(LSFT('8'))
#define KC_LPRN KC(LSFT('9'))
#define KC_RPRN KC(LSFT('0'))
#define KC_UNDS KC(LSFT('_'))
#define KC_PLUS KC(LSFT('+'))
#define KC_LCBR KC(LSFT('['))
#define KC_RCBR KC(LSFT(']'))
#define KC_PIPE KC(LSFT('\\'))
#define KC_COLN KC(LSFT(';'))
#define KC_DQUO KC(LSFT('\''))
#define KC_LABK KC(LSFT(','))
#define KC_RABK KC(LSFT('.'))
#define KC_QUES KC(LSFT('/'))
