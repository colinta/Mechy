#pragma once

#include "Plugin.h"
#include "../priv/Event.h"

enum THBehavior {
    TH_PRESS,
    TH_MODIFIER,
};

struct TapHoldKey {
    uint8_t keyIndex;
    KBD tapKey;
    KBD holdKey;
    THBehavior behavior;
    TapHoldKey* next;
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
    static TapHoldKey* keyPtrStack;
};

#define TH(tap, hold) { .name = FN_TAP_HOLD, .key = TapHold::add(tap, hold, TH_PRESS) }
#define LT(hold, tap) { .name = FN_TAP_HOLD, .key = TapHold::add(tap, hold, TH_MODIFIER) }
