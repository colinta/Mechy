#include "Lock.h"


Lock::Lock(uint8_t _need) {
    need = _need;
    lock_count = 0;
    is_locked = false;
}

uint8_t Lock::defaultName() {
    return FN_LOCK;
}

bool Lock::is(uint8_t event_type, Event* UNUSED(event)) {
    return event_type == EVENT_LOCK;
}

bool Lock::override(Event* event, Plugin* plugin) {
    if (plugin->is(EVENT_LOCK, event) || plugin->is(EVENT_LAYER, event))  return KBD_CONTINUE;
    return !is_locked;
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

            EventPtr* eventPtr = mechy->events();
            EventPtr* nextPtr = NULL;
            while (eventPtr) {
                nextPtr = eventPtr->next;
                if (eventPtr->event->name != FN_LOCK) {
                    mechy->finishEvent(eventPtr->event);
                }
                eventPtr = nextPtr;
            }
        }
    }
}
