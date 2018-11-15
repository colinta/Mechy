#pragma once

#include "Plugin.h"
#include "../priv/Event.h"

enum {
    MOUSEKEY_UP,
    MOUSEKEY_DOWN,
    MOUSEKEY_LEFT,
    MOUSEKEY_RIGHT,
    MOUSEKEY_LCLK,
    MOUSEKEY_RCLK,
    MOUSEKEY_MCLK,
    MOUSEKEY_CLK4,
    MOUSEKEY_CLK5,
};

class MouseKey : public Plugin {
public:
    MouseKey();
    uint8_t defaultName();
    void begin();
    bool is(uint8_t event_type, Event* event);
    void run(Event* event);
};

#define MK(k) { .name = FN_MOUSE_KEY, .key = k }
#define MK_UP   MK(MOUSEKEY_UP)
#define MK_DOWN MK(MOUSEKEY_DOWN)
#define MK_LEFT MK(MOUSEKEY_LEFT)
#define MK_RGHT MK(MOUSEKEY_RIGHT)
#define MK_LCLK MK(MOUSEKEY_LCLK)
#define MK_RCLK MK(MOUSEKEY_RCLK)
#define MK_MCLK MK(MOUSEKEY_MCLK)
#define MK_CLK4 MK(MOUSEKEY_CLK4)
#define MK_CLK5 MK(MOUSEKEY_CLK5)
