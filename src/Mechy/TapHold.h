#pragma once

#include "Plugin.h"
#include "../priv/Event.h"

enum THBehavior {
    TH_TAP,
    TH_HOLD,
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

    static void add(KBD, KBD, THBehavior behavior = TH_TAP);
protected:
    TapHoldEvent* eventArray;
};

#define TH(n) { .name = FN_TAP_HOLD, .key = n }
#define TH_0 TH(0)
#define TH_1 TH(1)
#define TH_2 TH(2)
#define TH_3 TH(3)
#define TH_4 TH(4)
#define TH_5 TH(5)
#define TH_6 TH(6)
#define TH_7 TH(7)
#define TH_8 TH(8)
#define TH_9 TH(9)
