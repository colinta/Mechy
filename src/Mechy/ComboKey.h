#pragma once

#include "Mechy.h"

class ComboKey : public Plugin {
public:
    ComboKey(uint8_t count, const KBD keys[], KBD comboKey);
    uint8_t defaultName();
    bool is(uint8_t event_type, Event* event);
    void run(Event* event);

protected:
    uint8_t count;
    KBD comboKey;
    KBD* keys;
};

#define CK(n)  { .name = FN_COMBO_KEY, .key = n }
