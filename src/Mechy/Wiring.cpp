#include "../priv/Constants.h"
#include "Wiring.h"

uint8_t* _directionRegisterPtr(uint8_t pin);
uint8_t* _portRegisterPtr(uint8_t pin);
uint8_t _pinRegister(uint8_t pin);
uint8_t _pinMask(uint8_t pin);
void _ddrModeOutput(uint8_t pin);
void _ddrModePullup(uint8_t pin);
void _ddrModeInput(uint8_t pin);
void _ddrInput(uint8_t pin);
void _pinOutputLow(uint8_t pin);
void _pinOutputHigh(uint8_t pin);

namespace Wiring {
    void pinMode(uint8_t pin, uint8_t mode) {
        uint8_t oldSREG = SREG;
        cli();
        switch (mode) {
        case INPUT:
            _ddrModeInput(pin);
            break;
        case INPUT_PULLUP:
            _ddrModePullup(pin);
            break;
        case OUTPUT:
            _ddrModeOutput(pin);
            break;
        }
        SREG = oldSREG;
    }

    void digitalWrite(uint8_t pin, bool val) {
        uint8_t oldSREG = SREG;
        cli();
        if (val) {
            _pinOutputHigh(pin);
        }
        else {
            _pinOutputLow(pin);
        }
        SREG = oldSREG;
    }

    bool digitalRead(uint8_t pin) {
        uint8_t pinRegister = _pinRegister(pin);
        uint8_t pinMask = _pinMask(pin);
        return bit_get(pinRegister, pinMask) != 0;
    }
}

void _ddrModeOutput(uint8_t pin) {
    uint8_t* ddr = _directionRegisterPtr(pin);
    uint8_t pinMask = _pinMask(pin);
    bit_on(*ddr, pinMask);
}

void _ddrModeInput(uint8_t pin) {
    _ddrInput(pin);
    _pinOutputLow(pin);
}

void _ddrModePullup(uint8_t pin) {
    _ddrInput(pin);
    _pinOutputHigh(pin);
}

void _ddrInput(uint8_t pin) {
    uint8_t* ddr = _directionRegisterPtr(pin);
    uint8_t pinMask = _pinMask(pin);
    bit_off(*ddr, pinMask);
}

// output LOW or floating input
void _pinOutputLow(uint8_t pin) {
    uint8_t* port = _portRegisterPtr(pin);
    uint8_t pinMask = _pinMask(pin);
    bit_off(*port, pinMask);
}

// output HIGH or INPUT_PULLUP
void _pinOutputHigh(uint8_t pin) {
    uint8_t* port = _portRegisterPtr(pin);
    uint8_t pinMask = _pinMask(pin);
    bit_on(*port, pinMask);
}

uint8_t* _directionRegisterPtr(uint8_t pin) {
    switch (pin) {
    case _B0: case _B1: case _B2: case _B3: case _B4: case _B5: case _B6: case _B7:
        return &DDRB;
    case _C6: case _C7:
        return &DDRC;
    case _D0: case _D1: case _D2: case _D3: case _D4: case _D5: case _D6: case _D7:
        return &DDRD;
    case _E2: case _E6:
        return &DDRE;
    case _F0: case _F1: case _F4: case _F5: case _F6: case _F7:
        return &DDRF;
    }
    return NULL;
}

uint8_t _pinMask(uint8_t pin) {
    switch (pin) {
        case _B0: return bit(0);
        case _B1: return bit(1);
        case _B2: return bit(2);
        case _B3: return bit(3);
        case _B4: return bit(4);
        case _B5: return bit(5);
        case _B6: return bit(6);
        case _B7: return bit(7);
        case _C6: return bit(6);
        case _C7: return bit(7);
        case _D0: return bit(0);
        case _D1: return bit(1);
        case _D2: return bit(2);
        case _D3: return bit(3);
        case _D4: return bit(4);
        case _D5: return bit(5);
        case _D6: return bit(6);
        case _D7: return bit(7);
        case _E2: return bit(2);
        case _E6: return bit(6);
        case _F0: return bit(0);
        case _F1: return bit(1);
        case _F4: return bit(4);
        case _F5: return bit(5);
        case _F6: return bit(6);
        case _F7: return bit(7);
    }
    return 0;
}

uint8_t* _portRegisterPtr(uint8_t pin) {
    switch (pin) {
    case _B0: case _B1: case _B2: case _B3: case _B4: case _B5: case _B6: case _B7:
        return &PORTB;
    case _C6: case _C7:
        return &PORTC;
    case _D0: case _D1: case _D2: case _D3: case _D4: case _D5: case _D6: case _D7:
        return &PORTD;
    case _E2: case _E6:
        return &PORTE;
    case _F0: case _F1: case _F4: case _F5: case _F6: case _F7:
        return &PORTF;
    }
    return NULL;
}

uint8_t _pinRegister(uint8_t pin) {
    switch (pin) {
    case _B0: case _B1: case _B2: case _B3: case _B4: case _B5: case _B6: case _B7:
        return PINB;
    case _C6: case _C7:
        return PINC;
    case _D0: case _D1: case _D2: case _D3: case _D4: case _D5: case _D6: case _D7:
        return PIND;
    case _E2: case _E6:
        return PINE;
    case _F0: case _F1: case _F4: case _F5: case _F6: case _F7:
        return PINF;
    }
    return 0;
}
