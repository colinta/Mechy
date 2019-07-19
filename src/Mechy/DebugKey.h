#pragma once

#include <Keyboard.h>
#include "Mechy.h"

class DebugKey: public Plugin {
public:
    DebugKey();
    void begin();
    uint8_t defaultName();
    bool is(uint8_t event_type, Event* event);
    void run(Event* event);

protected:
};

#define DB(k) { .name = FN_DEBUG, .key = k }

#define DB_A DB('a')
#define DB_B DB('b')
#define DB_C DB('c')
#define DB_D DB('d')
#define DB_E DB('e')
#define DB_F DB('f')
#define DB_G DB('g')
#define DB_H DB('h')
#define DB_I DB('i')
#define DB_J DB('j')
#define DB_K DB('k')
#define DB_L DB('l')
#define DB_M DB('m')
#define DB_N DB('n')
#define DB_O DB('o')
#define DB_P DB('p')
#define DB_Q DB('q')
#define DB_R DB('r')
#define DB_S DB('s')
#define DB_T DB('t')
#define DB_U DB('u')
#define DB_V DB('v')
#define DB_W DB('w')
#define DB_X DB('x')
#define DB_Y DB('y')
#define DB_Z DB('z')

#define DB_0 DB('0')
#define DB_1 DB('1')
#define DB_2 DB('2')
#define DB_3 DB('3')
#define DB_4 DB('4')
#define DB_5 DB('5')
#define DB_6 DB('6')
#define DB_7 DB('7')
#define DB_8 DB('8')
#define DB_9 DB('9')

#define DB_GRV DB('`')
#define DB_MINS DB('-')
#define DB_EQL DB('=')
#define DB_SCLN DB(';')
#define DB_LBRC DB('[')
#define DB_RBRC DB(']')
#define DB_QUOT DB('\'')
#define DB_BSLS DB('\\')
#define DB_COMM DB(',')
#define DB_DOT  DB('.')
#define DB_SLSH DB('/')
#define DB_SPC DB(' ')

#define DB_F1 DB('A')
#define DB_F2 DB('B')
#define DB_F3 DB('C')
#define DB_F4 DB('D')
#define DB_F5 DB('E')
#define DB_F6 DB('F')
#define DB_F7 DB('G')
#define DB_F8 DB('H')
#define DB_F9 DB('I')
#define DB_F10 DB('J')
#define DB_F11 DB('K')
#define DB_F12 DB('L')

#define DB_LEFT DB('M')
#define DB_RGHT DB('N')
#define DB_UP DB('O')
#define DB_DOWN DB('P')
#define DB_PGUP DB('Q')
#define DB_PGDN DB('R')
#define DB_HOME DB('S')
#define DB_END DB('T')

#define DB_SFT DB('U')
#define DB_CTL DB('V')
#define DB_ALT DB('W')
#define DB_GUI DB('X')

#define DB_ESC DB('!')
#define DB_INS DB('?')
#define DB_DEL DB('>')
#define DB_BSPC DB('<')
#define DB_ENT DB('|')
#define DB_TAB DB(':')
#define DB_CAPS DB('@')

#define DB_FN1 DB('#')
#define DB_FN2 DB('$')

// #define DB_FN3 DB('%')
// #define DB_FN4 DB('^')
// #define DB_FN5 DB('&')
// #define DB_FN6 DB('*')
// #define DB_FN7 DB('(')
// #define DB_FN8 DB(')')
#define DB_CAG DB('_')
#define DB_PLAY DB('+')
