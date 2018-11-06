#pragma once

#include "priv/Event.h"
#include "priv/Plugin.h"


struct PluginPtr {
    uint8_t name;
    Plugin *plugin;
    PluginPtr *next;
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

    void runPlugin(uint8_t keyState, KBD *currentKey);

private:
    inline void appendPluginPtr(PluginPtr *ptr);
};
