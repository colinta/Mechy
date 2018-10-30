#include "Lock.h"


Lock::Lock() {
    lock_count = 0;
    is_locked = false;
}

bool Lock::override(uint8_t name, Event *event) {
    if (!is_locked || name == FN_LOCK)  return KBD_CONTINUE;
    return KBD_HALT;
}

void Lock::run(Event *event) {
    if (event->isPressed() || event->isReleased()) {
        switch (event->key) {
            case LOCK_1:
            case LOCK_2:
                if (event->isPressed()) {
                    lock_count += 1;
                }
                else if (event->isReleased()) {
                    lock_count -= 1;
                }

                if (lock_count == 2) {
                    is_locked = !is_locked;
                }
        }
    }
}