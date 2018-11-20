#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "Arduino.h"
#include "pins_arduino.h"
#include "Tone.h"

// timerx_toggle_count:
//  > 0 - duration specified
//  = 0 - stopped
//  < 0 - infinitely (until stop() method called, or new play() called)

#define TIMER_OPT_AUDIBLE 0b00000001
#define TIMER_OPT_SONG    0b00000010
#define TIMER_DID_PAUSE   0b00000100

static unsigned int* songNotes = NULL;
static unsigned long* songDurations = NULL;
static unsigned int songLen = 0;

volatile long timer_toggle_count;
volatile uint8_t timer_pin;
volatile uint8_t timer_options;
volatile uint8_t* timer_pin_port;
volatile uint8_t timer_pin_mask;


static void toneBegin(uint8_t _pin)
{
    // Set the pinMode as OUTPUT
    pinMode(_pin, OUTPUT);

    // Set timer specific stuff
    // All timers in CTC mode
    // 8 bit timers will require changing prescalar values,
    // whereas 16 bit timers are set to either ck/1 or ck/64 prescalar
    // 16 bit timer
    TCCR3A = 0;
    TCCR3B = 0;
    bitWrite(TCCR3B, WGM32, 1);
    bitWrite(TCCR3B, CS30, 1);
    timer_pin = _pin;
    timer_pin_port = portOutputRegister(digitalPinToPort(_pin));
    timer_pin_mask = digitalPinToBitMask(_pin);
}



// XXX: this function only works properly for timer 2 (the only one we use
// currently).  for the others, it should end the tone, but won't restore
// proper PWM functionality for the timer.
void disableTimer()
{
    bitWrite(TIMSK3, OCIE3A, 0);
}


void Tone::play(uint8_t _pin, unsigned int frequency, unsigned long duration)
{
    uint8_t prescalarbits = 0b001;
    long toggle_count = 0;
    uint32_t ocr = 0;
    uint8_t audible = frequency > 0;
    if (frequency == 0)
    {
        frequency = 1000;
    }

    toneBegin(_pin);

    // two choices for the 16 bit timers: ck/1 or ck/64
    ocr = F_CPU / frequency / 2 - 1;

    prescalarbits = 0b001;
    if (ocr > 0xffff)
    {
        ocr = F_CPU / frequency / 2 / 64 - 1;
        prescalarbits = 0b011;
    }

    TCCR3B = (TCCR3B & 0b11111000) | prescalarbits;

    // Calculate the toggle count
    if (duration > 0)
    {
        toggle_count = 2 * frequency * duration / 1000;
    }
    else
    {
        toggle_count = -1;
    }

    // Set the OCR for the given timer,
    // set the toggle count,
    // then turn on the interrupts
    OCR3A = ocr;
    timer_toggle_count = toggle_count;
    timer_options = 0;
    timer_options |= audible ? TIMER_OPT_AUDIBLE : 0;
    timer_options |= songLen > 0 ? TIMER_OPT_SONG : 0;
    bitWrite(TIMSK3, OCIE3A, 1);
}

bool Tone::isPlaying()
{
        return (songLen > 0) || (timer_toggle_count != 0);
}

void Tone::song(uint8_t _pin, unsigned int len, const unsigned int* notes, const unsigned long* durations)
{
    if (len == 0)  return;

    songNotes = (unsigned int*)notes;
    songDurations = (unsigned long*)durations;
    songLen = len;
    Tone::play(_pin, 0, 10);
}

void Tone::stop(uint8_t _pin)
{
    songNotes = NULL;
    songDurations = NULL;
    songLen = 0;
    disableTimer();
    digitalWrite(_pin, 0);
}


ISR(TIMER3_COMPA_vect)
{
    if (timer_toggle_count != 0)
    {
        // toggle the pin or turn it off
        if (timer_options & TIMER_OPT_AUDIBLE)
        {
            *timer_pin_port ^= timer_pin_mask;
        }
        else {
            *timer_pin_port &= ~(timer_pin_mask);
        }

        if (timer_toggle_count > 0)
            timer_toggle_count--;
    }
    else if ((timer_options & TIMER_OPT_SONG) && songLen > 0)
    {
        if (timer_options & TIMER_DID_PAUSE) {
            unsigned int note = pgm_read_word(songNotes++);
            unsigned long duration = pgm_read_dword(songDurations++);
            Tone::play(timer_pin, note, duration);

            songLen -= 1;
            if (songLen == 0)
            {
                songNotes = NULL;
                songDurations = NULL;
            }
        }
        else {
            Tone::play(timer_pin, 0, 50);
            timer_options |= TIMER_DID_PAUSE;
        }
    }
    else
    {
        disableTimer();
        *timer_pin_port &= ~(timer_pin_mask);  // keep pin low after stop
    }
}
