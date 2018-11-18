#pragma once

#include "Plugin.h"
#include "../priv/Event.h"

enum THBehavior {
    TH_PRESS,
    TH_MODIFIER,
};

struct TapHoldKeyList {
    uint8_t keyIndex;
    KBD tapKey;
    KBD holdKey;
    THBehavior behavior;
    TapHoldKeyList* next;
};

struct TapHoldEvent {
    KBD tapKey;
    KBD holdKey;
    THBehavior behavior;
    uint16_t modifierSnapshot;
};

class TapHold : public Plugin {
public:
    TapHold();
    uint8_t defaultName();
    void begin();
    void run(Event* event);

    static uint16_t add(KBD, KBD, THBehavior);
protected:
    static uint8_t keys;
    TapHoldEvent* eventArray;
    static TapHoldKeyList* keyPtrStack;
};

#define TH(tap, hold) { .name = FN_TAP_HOLD, .key = TapHold::add(tap, hold, TH_PRESS) }
#define MT(hold, tap) { .name = FN_TAP_HOLD, .key = TapHold::add(tap, hold, TH_MODIFIER) }
