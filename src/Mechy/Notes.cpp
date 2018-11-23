#include "Notes.h"
#include "Tone/Tone.h"

static const unsigned int  notes[]     PROGMEM = { NOTE_E7 , NOTE_E7 , 0       , NOTE_E7 , 0       , NOTE_C7 , NOTE_E7 , 0       , NOTE_G7 , 0       , NOTE_G6 , };
static const unsigned long durations[] PROGMEM = { LEN_1_16, LEN_1_16, LEN_1_16, LEN_1_16, LEN_1_16, LEN_1_16, LEN_1_16, LEN_1_16, LEN_1_16, LEN_3_16, LEN_1_16, };

Notes::Notes(uint8_t _pin) {
    pin = _pin;
}

uint8_t Notes::defaultName() { return FN_NOTES; }

bool Notes::is(uint8_t event_type, Event* UNUSED(event)) {
    return event_type == EVENT_NOTES;
}

void Notes::run(Event* noteEvent) {
    if (noteEvent->data() & DATA_SONG) {
        goto runSong;
    }

    if (noteEvent->isPressed()) {
        Tone::play(pin, noteEvent->rawKey());
    }
    else if (noteEvent->isReleased()) {
        EventPtr* eventPtr = mechy->events();
        Event* otherEvent;
        while (eventPtr) {
            otherEvent = eventPtr->event;
            Plugin* plugin = mechy->pluginFor(otherEvent->name);
            if (plugin && plugin->is(EVENT_NOTES, otherEvent) && otherEvent->isDown()) {
                Tone::play(pin, otherEvent->rawKey());
                break;
            }
            eventPtr = eventPtr->next;
        }

        if (!eventPtr) {
            Tone::stop(pin);
        }
    }
    return;

runSong:
    if (noteEvent->isPressed()) {
        Tone::song(pin, 11, notes, durations);
    }
    else if (noteEvent->isReleased()) {
        Tone::stop(pin);
    }
}
