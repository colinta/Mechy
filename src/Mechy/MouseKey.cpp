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

void MouseKey::run(Event* event) {
    char dx = 0, dy = 0;
    char button;

    switch (event->key) {
    case MOUSEKEY_UP:
        dy = -5;
        goto mouseMove;
    case MOUSEKEY_DOWN:
        dy = 5;
        goto mouseMove;
    case MOUSEKEY_LEFT:
        dx = -5;
        goto mouseMove;
    case MOUSEKEY_RIGHT:
        dx = 5;
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
    if (mechy->currentModifiers() & MCHY_MASK_CTRL) {
        dy /= 5;
        dx /= 5;
    }
    else if (mechy->currentModifiers() & MCHY_MASK_SHIFT) {
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
