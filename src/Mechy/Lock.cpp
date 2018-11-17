#include "Lock.h"


Lock::Lock(uint8_t _need = 2) {
    need = _need;
    lock_count = 0;
    is_locked = false;
}

uint8_t Lock::defaultName() {
    return FN_LOCK;
}

bool Lock::is(uint8_t event_type, Event* UNUSED(event)) {
    return event_type == EVENT_META;
}

bool Lock::override(uint8_t name, Event* event, Plugin* plugin) {
    if (!is_locked || (plugin->is(EVENT_META, event)))  return KBD_CONTINUE;
    return KBD_HALT;
}

void Lock::run(Event* event) {
    if (event->isPressed() || event->isReleased()) {
        if (event->isPressed()) {
            lock_count += 1;
        }
        else if (event->isReleased()) {
            lock_count -= 1;
        }

        if (lock_count == need) {
            is_locked = !is_locked;
        }
    }
}
