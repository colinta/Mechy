#pragma once

#include "priv/Event.h"

class Mechy;

class Responder {
public:
    virtual void begin();
    virtual void scan();

    Mechy* mechy;
};
