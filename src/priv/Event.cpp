#include "Event.h"

bool Event::isPressed() { return keyState == KEY_STATE_PRESSED; }
bool Event::isHeld() { return keyState == KEY_STATE_HELD; }
bool Event::isDown() { return keyState == KEY_STATE_PRESSED || keyState == KEY_STATE_HELD; }
bool Event::isReleased() { return keyState == KEY_STATE_RELEASED; }
