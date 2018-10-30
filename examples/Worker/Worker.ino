#include "Worker.h"

#define DATA 2
#define CLK 3

#define ROWS 6
#define COLS 9
const uint8_t pinRows[ROWS] = {1, 0, 4, 5, 6, 7};
const uint8_t pinCols[COLS] = {9, 10, 16, 14, 15, 18, 19, 20, 21};

Worker worker = Worker(DATA, CLK, pinRows, pinCols, ROWS, COLS);

void setup() {
  worker.begin();
}

void loop() {
  worker.scan();
}
