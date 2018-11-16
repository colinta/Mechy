#include <Arduino.h>

class Tone {
public:
    static void play(uint8_t _pin, unsigned int frequency, unsigned long duration = 0);
    static void song(uint8_t _pin, unsigned int len, const unsigned int* notes, const unsigned long* durations);
    static bool isPlaying();
    static void stop(uint8_t _pin);
};
