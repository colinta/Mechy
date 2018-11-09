#include "Plugin.h"

void Plugin::begin() {
}

void Plugin::tick() {
}

bool Plugin::override(uint8_t UNUSED(name), Event *UNUSED(event)) {
    return KBD_CONTINUE;
}

void Plugin::run(Event *UNUSED(event)) {
}
