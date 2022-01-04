#include <stdarg.h>
#include <Keyboard.h>
#include "SendString.h"


SendString::SendString(uint8_t _count, uint16_t** _macros) {
    count = _count;
    macros = (uint16_t**)_macros;
}

uint8_t SendString::defaultName() {
    return FN_SENDSTRING;
}

bool SendString::is(uint8_t event_type, Event* event) {
    return event_type == EVENT_KEYPRESS && event->isReleased();
}

void SendString::run(Event* event) {
    uint8_t macro_index = event->key();

    if (macro_index >= count) {
        return;
    }

    if (event->isPressed()) {
        uint16_t keyCount = macros[macro_index][0];
        uint16_t delayBy, mods, key;
        for (uint8_t i = 0; i < keyCount ; ++i) {
            key = macros[macro_index][i + 1];

            if (key & SS_DELAY) {
                delayBy = key & (~(uint16_t)SS_DELAY);
                Serial.print("delayBy:");
                Serial.println(delayBy);
            } else {
                delayBy = 10;
                if (key & SS_IGNOREMODS) {
                    mods = mechy->currentModifiers();
                    mechy->clearModifiers();
                }

                if (key & SS_DOWN) {
                    sendKey(key, true);
                }

                if (key & (SS_DOWN | SS_UP)) {
                    delay(10);
                }

                if (key & SS_UP) {
                    sendKey(key, false);
                }

                if (key & SS_IGNOREMODS) {
                    mechy->updateModifiers(mods);
                }
            }

            delay(delayBy);
        }
    }
}

void SendString::sendKey(uint16_t key, bool keyDown) {
    uint8_t ascii = key & SS_CHAR;
    uint8_t mods = ((uint8_t)((uint16_t)(key & SS_MODS) >> 8));

    if (keyDown) {
        if (mods & DATA_MOD_LSFT) {
            mechy->pressKey(KEY_LEFT_SHIFT);
        }
        else if (mods & DATA_MOD_LGUI) {
            mechy->pressKey(KEY_LEFT_GUI);
        }
        else if (mods & DATA_MOD_LCTL) {
            mechy->pressKey(KEY_LEFT_CTRL);
        }
        else if (mods & DATA_MOD_LALT) {
            mechy->pressKey(KEY_LEFT_ALT);
        }

        mechy->pressKey(ascii);
    }
    else {
        mechy->releaseKey(ascii);

        if (mods & DATA_MOD_LSFT) {
            mechy->releaseKey(KEY_LEFT_SHIFT);
        }
        else if (mods & DATA_MOD_LGUI) {
            mechy->releaseKey(KEY_LEFT_GUI);
        }
        else if (mods & DATA_MOD_LCTL) {
            mechy->releaseKey(KEY_LEFT_CTRL);
        }
        else if (mods & DATA_MOD_LALT) {
            mechy->releaseKey(KEY_LEFT_ALT);
        }
    }
}

uint16_t down(uint16_t key) {
    return SS_DOWN | (key & SS_KEY);
}

uint16_t up(uint16_t key) {
    return SS_UP | (key & SS_KEY);
}

uint16_t downUp(uint16_t key) {
    return SS_DOWN | SS_UP | (key & SS_KEY);
}

uint16_t ignoreModifiers(uint16_t key) {
    return SS_IGNOREMODS | (key & SS_KEY);
}

uint16_t delayBy(uint16_t delayBy) {
    return SS_DELAY | delayBy;
}

uint16_t* sendMacro(uint16_t count, ...) {
    uint16_t* keys = (uint16_t*)malloc(sizeof(uint16_t) * (count + 1));
    keys[0] = count;
    va_list args;
    va_start(args, count);
    for (uint16_t i = 0; i < count; ++i) {
        uint16_t key = va_arg(args, uint16_t);
        keys[i + 1] = key;
    }
    return keys;
}

uint16_t* sendString(const char *string) {
    uint16_t count = strlen(string);
    uint16_t* keys = (uint16_t*)malloc(sizeof(uint16_t) * (count + 1));
    keys[0] = count;
    for (uint16_t i = 0; i < count; ++i) {
        uint16_t key = string[i];
        keys[i + 1] = (key & SS_CHAR) | SS_DOWN | SS_UP;
    }
    return keys;
}
