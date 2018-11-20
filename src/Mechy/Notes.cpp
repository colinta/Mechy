#include "Notes.h"
#include "Tone/Tone.h"

static const unsigned int  notes[]     PROGMEM = { NOTE_E7 , NOTE_E7 , 0       , NOTE_E7 , 0       , NOTE_C7 , NOTE_E7 , 0       , NOTE_G7 , 0       , NOTE_G6 , };
static const unsigned long durations[] PROGMEM = { LEN_1_16, LEN_1_16, LEN_1_16, LEN_1_16, LEN_1_16, LEN_1_16, LEN_1_16, LEN_1_16, LEN_1_16, LEN_3_16, LEN_1_16, };

Notes::Notes(uint8_t _pin) {
    pin = _pin;
}

uint8_t Notes::defaultName() { return FN_NOTES; }

void Notes::run(Event* event) {
    if (event->data() & DATA_SONG) {
        goto runSong;
    }

    if (event->isPressed()) {
        Tone::play(pin, event->rawKey());
    }
    else if (event->isReleased()) {
        EventPtr* eventPtr = mechy->events();
        while (eventPtr) {
            if (eventPtr->event->name == event->name && eventPtr->event->isDown()) {
                Tone::play(pin, eventPtr->event->rawKey());
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
    if (event->isPressed()) {
        Tone::song(pin, 11, notes, durations);
    }
    else if (event->isReleased()) {
        Tone::stop(pin);
    }
}
