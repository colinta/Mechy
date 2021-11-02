#pragma once

#include "Mechy.h"

class SendString: public Plugin {
public:
    SendString(uint8_t count, uint16_t** macros);
    uint8_t defaultName();
    bool is(uint8_t event_type, Event* event);
    void run(Event* event);

protected:
    uint8_t count;
    uint16_t** macros;
    void sendKey(uint16_t key, bool keyDown);
};

#define SS(n) { .name = FN_SENDSTRING, .key = (n) }
#define SS_0 SS(0)
#define SS_1 SS(1)
#define SS_2 SS(2)
#define SS_3 SS(3)
#define SS_4 SS(4)
#define SS_5 SS(5)
#define SS_6 SS(6)
#define SS_7 SS(7)
#define SS_8 SS(8)
#define SS_9 SS(9)

//                     0x0000000000000000
//                       ^                delay
//                        ^               ignore modifiers
//                        .^^             down, up, or both
//                        .  ^^^^         modifier    \__ key mask
//                        .      ^^^^^^^^ ascii code  /
#define SS_DELAY      (0b1000000000000000)
#define SS_IGNOREMODS (0b0100000000000000)
#define SS_DOWN       (0b0010000000000000)
#define SS_UP         (0b0001000000000000)

uint16_t down(uint16_t key);
uint16_t up(uint16_t key);
uint16_t downUp(uint16_t key);
uint16_t ignoreModifiers(uint16_t key);
uint16_t delayBy(uint16_t delayBy);

/**
 * helper to create macro, returns a pointer to a macro.
 * Usage:
 * uint16_t* macros[] = {
 *     // sends \, ctrl+v, '
 *     sendString(3, ignoreModifiers(downUp('\\')), ignoreModifiers(downUp(LCTL('v'))), downUp('\'')),
 * }
 */
uint16_t* sendString(uint16_t count, ...);
