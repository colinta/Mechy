// we don't actually need this header, but the arduino IDE won't compile without it.
#include <Mechy.h>
#include <Mechy/Transmitter.h>
#include <Mechy/Hardware/BFO9000.h>

Transmitter transmitter = Transmitter(DATA, CLK, pinRows, pinCols, ROWS, COLS);

void setup() {
  transmitter.begin();
}

void loop() {
  transmitter.scan();
}
