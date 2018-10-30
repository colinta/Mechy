#pragma once

#include "Mechy.h"

class BetterHalf: public Plugin {
public:
    BetterHalf(KBD *workerKeys, uint8_t rows, uint8_t cols, uint8_t sdaPin, uint8_t sclPin);
    void process_begin();
    void process_tick();
    void process_event(Event *event);
protected:
    uint8_t SDA;
    uint8_t SCL;
    uint8_t ROWS;
    uint8_t COLS;
    KBD *currentKey;
    KBD *workerKeys;
    void listen();
    void hold_check();

    void await_ack();
    bool get_one_worker_bit();
    void debounce();
    void delay_for_worker();
    bool worker_did_ack();
    bool worker_has_data();
    void send_ready_state();
    void send_reading_state();
};
