#pragma once

namespace Wiring {
    void pinMode(uint8_t pin, uint8_t);
    bool digitalRead(uint8_t pin);
    void digitalWrite(uint8_t pin, bool);
};
