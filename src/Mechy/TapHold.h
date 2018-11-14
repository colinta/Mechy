#pragma once

#include "Plugin.h"
#include "../priv/Event.h"

struct TapHoldKey {
    uint8_t keyIndex;
    KBD tapKey;
    KBD holdKey;
    TapHoldKey* next;
};

struct TapHoldEvent {
    bool isActive;
    KBD tapKey;
    KBD holdKey;
    uint16_t modifierSnapshot;
};

class TapHold : public Plugin {
public:
    TapHold();
    uint8_t defaultName();
    void begin();
    void run(Event* event);

    static uint16_t add(KBD, KBD);
protected:
    static uint8_t keys;
    TapHoldEvent* eventArray;
    static TapHoldKey* keyPtrStack;
};

#define TH(k1, k2) { .name = FN_TAP_HOLD, .key = TapHold::add(k1, k2) }
