#pragma once

#define LISTEN_TIME 20
#define TRANSMIT_DETECT 50
#define TRANSMIT_TIME 60
#define TX_LISTEN_TIME 400
#define NUM_LISTEN_BITS 11
#define NUM_TRANSMIT_BITS 8

static inline void debounce() { delayMicroseconds(20); }
static inline void delayForTransmit() { delayMicroseconds(TX_LISTEN_TIME); }
