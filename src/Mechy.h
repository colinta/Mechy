#pragma once

#include "priv/Event.h"

class Mechy;
class Plugin;

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
    PluginPtr firstPlugin;
    PluginPtr *lastPtr;

    void runPlugin(bool isDown, bool isUp, KBD *currentKey);
};


class Plugin {
public:
    virtual void begin();
    virtual void tick();
    virtual bool override(uint8_t name, Event *event);
    virtual void run(Event *event) = 0;
    Mechy *mechy;
};
