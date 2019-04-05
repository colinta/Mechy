#pragma once

#include <Arduino.h>
#include "Constants.h"

// in general, events have 8bits of "data" and 8bits of "key space"
#define EVENT_KEY_MASK  0b0000000011111111
#define EVENT_DATA_MASK 0b1111111100000000
#define EVENT_USER_MASK 0b00011111
#define EVENT_DATA_SHIFT 8
#define EVENT_KEY_STATE_MASK  0b00001111
#define EVENT_INTERNAL_MASK 0b11110000

// to encourage "best practices" here are some bit masks:
#define EVENT_IS_ACTIVE_BIT 5

/* EVENT TYPES */
// typical keys (F1, A, ESC, DELETE), but not modifiers
#define EVENT_KEYPRESS 1
// Shift, Control, Alt, Gui
#define EVENT_MODIFIER 2
#define EVENT_MOUSE 3
#define EVENT_NOTES 4
#define EVENT_LOCK 5
#define EVENT_LAYER 6

struct KBD {
    uint8_t name;
    uint16_t key;

    inline uint8_t getProgmemName() const {
        return pgm_read_byte(&name);
    }
    inline uint16_t getProgmemKey() const {
        return pgm_read_word(&key);
    }
};

typedef const KBD* KBDPROG;
#define KEYS(varName) const KBD varName[] PROGMEM

struct Event {
    uint8_t name;
    uint16_t keyAndData;
    uint8_t internalState;
    unsigned long started;

    inline unsigned long duration() { return millis() - started; }
    inline bool isPressed() { return (internalState & EVENT_KEY_STATE_MASK) == KEY_STATE_PRESSED; }
    inline bool isHeld() { return (internalState & EVENT_KEY_STATE_MASK) == KEY_STATE_HELD; }
    inline bool isDown() { return (internalState & EVENT_KEY_STATE_MASK) == KEY_STATE_PRESSED || (internalState & EVENT_KEY_STATE_MASK) == KEY_STATE_HELD; }
    inline bool isReleased() { return (internalState & EVENT_KEY_STATE_MASK) == KEY_STATE_RELEASED; }

    inline uint16_t rawKey() {
        return keyAndData;
    }

    inline uint8_t key() {
        return keyAndData & EVENT_KEY_MASK;
    }

    inline uint16_t rawData() {
        return keyAndData & EVENT_DATA_MASK;
    }

    inline uint8_t data() {
        return ((uint8_t)((uint16_t)keyAndData >> EVENT_DATA_SHIFT));
    }

    inline uint8_t userData() {
        return data() & EVENT_USER_MASK;
    }

    inline bool dataBit(uint8_t bit) {
        if (bit >= 16)  return false;
        return bit_get(keyAndData, bit(EVENT_DATA_SHIFT + bit));
    }

    inline void setDataBit(uint8_t bit, bool value) {
        if (bit >= 16)  return;
        if (value) {
            bit_on(keyAndData, bit(EVENT_DATA_SHIFT + bit));
        }
        else {
            bit_off(keyAndData, bit(EVENT_DATA_SHIFT + bit));
        }
    }
    inline bool isActive() {
        return dataBit(EVENT_IS_ACTIVE_BIT);
    }
    inline void setIsActive(bool value) {
        setDataBit(EVENT_IS_ACTIVE_BIT, value);
    }

    inline void setKeyState(uint8_t keyState) {
        internalState = (internalState & EVENT_INTERNAL_MASK) | (keyState & EVENT_KEY_STATE_MASK);
    }

    inline uint8_t keyState() {
        return internalState & EVENT_KEY_STATE_MASK;
    }

    inline void setShouldIgnore(bool ignore) {
        internalState = (internalState & EVENT_KEY_STATE_MASK) | (ignore ? 0b00010000 : 0b00000000);
    }

    inline bool shouldIgnore() {
        return internalState & EVENT_INTERNAL_MASK;
    }
};

#define ____ { .name = FN_NONE, .key = MCHY_NONE }
#define vvvv { .name = FN_NONE, .key = MCHY_TRANS }

/* user defined key macros */
#define FN_USER(n)  (FN_USER_0 + n)
#define USER_KEY(m) m
#define USER(n, m) { .name = FN_USER(n), .key = USER_KEY(m) }
