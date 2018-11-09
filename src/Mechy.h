#pragma once

#include "priv/Constants.h"
#include "priv/Event.h"
#include "Mechy/Plugin.h"


struct PluginPtr {
    uint8_t name;
    Plugin *plugin;
    PluginPtr *next;
};


struct KBDDataPtr {
    KBD *kbd;
    bool isPressed;
    unsigned long started;
    KBDDataPtr *next;
};


class Mechy {
public:
    Mechy();
    void add(uint8_t name, Plugin *plugin);
    void _begin();
    void _tick();
    void processKeyEvent(bool isPressed, KBD *currentKey);

    void sendKeyboardPress(uint8_t k);
    void sendKeyboardRelease(uint8_t k);
protected:
    Event event;
    PluginPtr *firstPluginPtr;
    KBDDataPtr *firstKBDPtr;
    uint16_t modifiers;

    void runPlugin(uint8_t keyState, KBDDataPtr *kbdData, uint16_t duration);

private:
    inline void appendPluginPtr(PluginPtr *ptr);
    inline void appendKBDPtr(KBDDataPtr *ptr);
    inline KBDDataPtr *removeKBDPtr(KBDDataPtr *ptr);
};
