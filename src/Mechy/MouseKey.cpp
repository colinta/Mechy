#include <Mouse.h>
#include "Mechy.h"
#include "MouseKey.h"

MouseKey::MouseKey() {
}

uint8_t MouseKey::defaultName() {
    return FN_MOUSE_KEY;
}

void MouseKey::begin() {
    Mouse.begin();
}

bool MouseKey::is(uint8_t event_type, Event* UNUSED(event)) {
    switch (event->key) {
    case MOUSEKEY_LCLK:
    case MOUSEKEY_RCLK:
    case MOUSEKEY_MCLK:
    case MOUSEKEY_CLK4:
        return event_type == EVENT_MOUSE;
    default:
        return false;
    }
}

void MouseKey::run(Event* event) {
    char dx = 0, dy = 0;
    char button;

    switch (event->key) {
    case MOUSEKEY_UP:
        dy = -1;
        goto mouseMove;
    case MOUSEKEY_DOWN:
        dy = 1;
        goto mouseMove;
    case MOUSEKEY_LEFT:
        dx = -1;
        goto mouseMove;
    case MOUSEKEY_RIGHT:
        dx = 1;
        goto mouseMove;
    case MOUSEKEY_LCLK:
        button = MOUSE_LEFT;
        goto mouseClick;
    case MOUSEKEY_RCLK:
        button = MOUSE_RIGHT;
        goto mouseClick;
    case MOUSEKEY_MCLK:
        button = MOUSE_MIDDLE;
        goto mouseClick;
    case MOUSEKEY_CLK4:
        button = 4;
        goto mouseClick;
    case MOUSEKEY_CLK5:
        button = 5;
        goto mouseClick;
    }
    return;

mouseMove:
    if (mechy->currentModifiers() & MOD_SHIFT) {
        dy *= 5;
        dx *= 5;
    }

    if (event->isDown()) {
        Mouse.move(dx, dy, 0);
    }
    return;

mouseClick:
    if (event->isPressed()) {
        Mouse.press(button);
    }
    else if (event->isReleased()) {
        Mouse.release(button);
    }
    return;
}
