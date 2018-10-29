#include "BetterHalf.h"

BetterHalf::BetterHalf(KBD *_workerKeys, uint8_t rows, uint8_t cols, uint8_t sdaPin, uint8_t sclPin) {
    workerKeys = _workerKeys;
    ROWS = rows;
    COLS = cols;
    SDA = sdaPin;
    SCL = sclPin;
}

void BetterHalf::process_begin() {
  pinMode(SDA, INPUT);
  pinMode(SCL, OUTPUT);
  digitalWrite(SCL, HIGH);
}

void BetterHalf::process_tick() {
    listen();
    hold_check();
}

void BetterHalf::process_event(Event *event) {
}

void BetterHalf::listen() {
    if (!worker_has_data()) { return; }

    await_ack();

listenBody:
    byte input = 0;
    for (uint8_t i = 0; i < 8; i++) {
        bool oneBit = get_one_worker_bit();
        if (oneBit) {
            input |= 1 << i;
        }
    }

    uint8_t row = input & 0b111;
    uint8_t col = (input >> 3) & 0b1111;
    bool isPressed = !!(input >> 7);
    currentKey = workerKeys + (COLS * row) + col;
    mechy->process_key_event(isPressed, currentKey);

    bool done = get_one_worker_bit();
    if (!done) {
        goto listenBody;
    }
}

void BetterHalf::hold_check() {
    for (uint8_t row = 0; row < ROWS; row++) {
        for (uint8_t col = 0; col < COLS; col++) {
            currentKey = workerKeys + (COLS * row) + col;
            if (currentKey->isPressed) {
                mechy->process_key_event(true, currentKey);
            }
        }
    }
}

void BetterHalf::await_ack() {
    send_ready_state();
    if (!worker_did_ack())  while (!worker_did_ack()) {};
    delay_for_worker();
    send_reading_state();
    debounce();
}

bool BetterHalf::get_one_worker_bit() {
    send_ready_state();
    delay_for_worker();
    send_reading_state();
    debounce();
    return digitalRead(SDA);
}

void BetterHalf::debounce() { delayMicroseconds(10); }
void BetterHalf::delay_for_worker() { delayMicroseconds(1000); }
bool BetterHalf::worker_did_ack() { return digitalRead(SDA); }
bool BetterHalf::worker_has_data() { return !digitalRead(SDA); }
void BetterHalf::send_ready_state() { digitalWrite(SCL, LOW); }
void BetterHalf::send_reading_state() { digitalWrite(SCL, HIGH); }
