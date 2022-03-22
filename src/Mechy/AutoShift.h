#pragma once

#include "Plugin.h"
#include "../priv/Event.h"

class AutoShift : public Plugin {
public:
    AutoShift();
    uint8_t defaultName();
    bool is(uint8_t event_type, Event* event);
    bool override(Event* event, Plugin* plugin);
    void run(Event* event);
};

#define AS(k) { .name = FN_AUTO_SHIFT, .key = k }

#define AS_A AS('a')
#define AS_B AS('b')
#define AS_C AS('c')
#define AS_D AS('d')
#define AS_E AS('e')
#define AS_F AS('f')
#define AS_G AS('g')
#define AS_H AS('h')
#define AS_I AS('i')
#define AS_J AS('j')
#define AS_K AS('k')
#define AS_L AS('l')
#define AS_M AS('m')
#define AS_N AS('n')
#define AS_O AS('o')
#define AS_P AS('p')
#define AS_Q AS('q')
#define AS_R AS('r')
#define AS_S AS('s')
#define AS_T AS('t')
#define AS_U AS('u')
#define AS_V AS('v')
#define AS_W AS('w')
#define AS_X AS('x')
#define AS_Y AS('y')
#define AS_Z AS('z')

#define AS_0 AS('0')
#define AS_1 AS('1')
#define AS_2 AS('2')
#define AS_3 AS('3')
#define AS_4 AS('4')
#define AS_5 AS('5')
#define AS_6 AS('6')
#define AS_7 AS('7')
#define AS_8 AS('8')
#define AS_9 AS('9')

#define AS_GRV AS('`')
#define AS_MINS AS('-')
#define AS_EQL AS('=')
#define AS_SCLN AS(';')
#define AS_LBRC AS('[')
#define AS_RBRC AS(']')
#define AS_QUOT AS('\'')
#define AS_BSLS AS('\\')
#define AS_COMM AS(',')
#define AS_DOT  AS('.')
#define AS_SLSH AS('/')
#define AS_SPC AS(' ')
