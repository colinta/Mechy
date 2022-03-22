#include "Plugin.h"

uint8_t Plugin::defaultName() {
    return FN_NONE;
}

void Plugin::begin() {
}

void Plugin::tick() {
}

bool Plugin::override(Event* UNUSED(event), Plugin* UNUSED(plugin)) {
    return KBD_CONTINUE;
}

bool Plugin::is(uint8_t UNUSED(event_type), Event* UNUSED(event)) {
    return false;
}

void Plugin::run(Event* UNUSED(event)) {
}
