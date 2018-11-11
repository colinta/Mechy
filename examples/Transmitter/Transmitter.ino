// we don't actually need this header, but the arduino IDE won't compile without it.
#include <Mechy/Transmitter.h>

#define DATA 2
#define CLK 3

#define ROWS 6
#define COLS 9
const uint8_t pinRows[ROWS] = {1, 0, 4, 5, 6, 7};
const uint8_t pinCols[COLS] = {9, 10, 16, 14, 15, 18, 19, 20, 21};
// const uint8_t pinRows[] = {_D3, _D2, _D4, _C6, _D7, _E6};
// const uint8_t pinCols[] = {_B5, _B6, _B2, _B3, _B1, _F7, _F6, _F5, _F4};

Transmitter transmitter = Transmitter(DATA, CLK, pinRows, pinCols, ROWS, COLS);

void setup() {
    transmitter.begin();
}

void loop() {
    transmitter.scan();
}
