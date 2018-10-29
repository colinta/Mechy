#include <Keyboard.h>
#include <String.h>
#include "Event.h"

bool Event::isPressed() { return keyState == KEY_PRESSED; }
bool Event::isHeld() { return keyState == KEY_HELD; }
bool Event::isDown() { return keyState == KEY_PRESSED || keyState == KEY_HELD; }
bool Event::isReleased() { return keyState == KEY_RELEASED; }
