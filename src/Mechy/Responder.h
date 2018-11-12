#pragma once

#include <Arduino.h>
#include "priv/Event.h"

class Mechy;

class Responder {
public:
    virtual void begin();
    virtual void scan();
    virtual void gotoLayer(uint8_t layer);

    Mechy* mechy;
};
