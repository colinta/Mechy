#pragma once

#include <Arduino.h>
#include "priv/Event.h"

class Mechy;

class Plugin {
public:
    virtual uint8_t defaultName();
    virtual void begin();
    virtual void tick();
    virtual bool override(uint8_t name, Event* event);
    virtual void run(Event* event);

    Mechy* mechy;
};
