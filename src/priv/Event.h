#pragma once

#include <Arduino.h>
#include "Constants.h"

struct KBD {
    uint8_t name;
    uint16_t key;
};

struct Event {
    uint8_t name;
    uint16_t key;
    uint8_t keyState;
    unsigned long started;

    unsigned long duration();
    bool isPressed();
    bool isReleased();
    bool isDown();
    bool isHeld();
};

#define ____ {.name = FN_NONE, .key = MCHY_NONE }
#define vvvv { .name = FN_NONE, .key = MCHY_TRANS }
