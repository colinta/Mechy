#pragma once

#include "priv/Event.h"
#include "priv/Plugin.h"


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
protected:
    Event event;
    PluginPtr *firstPluginPtr;
    KBDDataPtr *firstKBDPtr;

    void runPlugin(uint8_t keyState, KBDDataPtr *kbdData, uint16_t duration);

private:
    inline void appendPluginPtr(PluginPtr *ptr);
    inline void appendKBDPtr(KBDDataPtr *ptr);
    inline KBDDataPtr *removeKBDPtr(KBDDataPtr *ptr);
};
