#pragma once

#include <Arduino.h>

#define KEY_NONE 0
#define KEY_PRESSED 1
#define KEY_HELD 2
#define KEY_RELEASED 3

#define KBD_HALT false
#define KBD_CONTINUE true

#define FN_NONE     0
#define FN_KEYPRESS 1
#define FN_MEDIA    2
#define FN_STICKY   3
#define FN_LOCK     4
#define FN_MACRO    5


struct KBD {
    uint8_t name;
    uint16_t key;
    bool isPressed;
    unsigned long started;
};

struct Event {
    uint16_t key;
    uint8_t keyState;
    unsigned long duration;

    bool isPressed();
    bool isReleased();
    bool isDown();
    bool isHeld();
};

#define ____ {.name = FN_NONE, .key = 0, .isPressed = false, .started = 0 }
