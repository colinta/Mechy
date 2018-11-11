#include <Keyboard.h>
// we don't actually need this header, but the arduino IDE won't compile without it.
#include <Mechy.h>
#include <Mechy/Wiring.h>
#include <Mechy/Transmitter.h>

#define DATA _D1
#define CLK _D0

#define ROWS 6
#define COLS 9
const uint8_t pinRows[] = {_D3, _D2, _D4, _C6, _D7, _E6};
const uint8_t pinCols[] = {_B5, _B6, _B2, _B3, _B1, _F7, _F6, _F5, _F4};

Transmitter transmitter = Transmitter(DATA, CLK, pinRows, pinCols, ROWS, COLS);

void setup() {
    // turn off TXLED and RXLED by making them input pins
    Wiring::pinMode(_D5, INPUT);
    Wiring::pinMode(_B0, INPUT);
    transmitter.begin();
}

void loop() {
    transmitter.scan();
}
