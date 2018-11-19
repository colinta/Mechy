#pragma once

#define LISTEN_TIME 20
#define TRANSMIT_DETECT 50
#define TRANSMIT_TIME 60
#define TX_LISTEN_TIME 400
#define NUM_BITS 11

inline void debounce() { delayMicroseconds(20); }
