#include "Plugin.h"

uint8_t Plugin::defaultName() {
    return FN_NONE;
}

void Plugin::begin() {
}

void Plugin::tick() {
}

bool Plugin::override(uint8_t UNUSED(name), Event* UNUSED(event), Plugin* UNUSED(plugin)) {
    return KBD_CONTINUE;
}

void Plugin::run(Event* UNUSED(event)) {
}
