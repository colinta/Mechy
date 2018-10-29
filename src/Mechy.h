#pragma once

#include "Event.h"

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
    void process_key_event(bool isPressed, KBD *currentKey);
protected:
    Event event;
    PluginPtr firstPlugin;
    PluginPtr *lastPtr;

    void process_event(bool isDown, bool isUp, KBD *currentKey);
};


class Plugin {
public:
    virtual void process_begin();
    virtual void process_tick();
    virtual bool process_override(uint8_t name, Event *event);
    virtual void process_event(Event *event) = 0;
    Mechy *mechy;
};
