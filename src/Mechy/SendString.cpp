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

uint16_t* sendKeys(uint16_t key0) {
    uint16_t* keys = (uint16_t*)malloc(sizeof(uint16_t) * 2);
    keys[0] = 1;
    keys[1] = key0;
    return keys;
}

uint16_t* sendKeys(uint16_t key0, uint16_t key1) {
    uint16_t* keys = (uint16_t*)malloc(sizeof(uint16_t) * 3);
    keys[0] = 2;
    keys[1] = key0;
    keys[2] = key1;
    return keys;
}

uint16_t* sendKeys(uint16_t key0, uint16_t key1, uint16_t key2) {
    uint16_t* keys = (uint16_t*)malloc(sizeof(uint16_t) * 4);
    keys[0] = 3;
    keys[1] = key0;
    keys[2] = key1;
    keys[3] = key2;
    return keys;
}

uint16_t* sendKeys(uint16_t key0, uint16_t key1, uint16_t key2, uint16_t key3) {
    uint16_t* keys = (uint16_t*)malloc(sizeof(uint16_t) * 5);
    keys[0] = 4;
    keys[1] = key0;
    keys[2] = key1;
    keys[3] = key2;
    keys[4] = key3;
    return keys;
}

uint16_t* sendKeys(uint16_t key0, uint16_t key1, uint16_t key2, uint16_t key3, uint16_t key4) {
    uint16_t* keys = (uint16_t*)malloc(sizeof(uint16_t) * 6);
    keys[0] = 5;
    keys[1] = key0;
    keys[2] = key1;
    keys[3] = key2;
    keys[4] = key3;
    keys[5] = key4;
    return keys;
}

uint16_t* sendKeys(uint16_t key0, uint16_t key1, uint16_t key2, uint16_t key3, uint16_t key4, uint16_t key5) {
    uint16_t* keys = (uint16_t*)malloc(sizeof(uint16_t) * 7);
    keys[0] = 6;
    keys[1] = key0;
    keys[2] = key1;
    keys[3] = key2;
    keys[4] = key3;
    keys[5] = key4;
    keys[6] = key5;
    return keys;
}

uint16_t* sendKeys(uint16_t key0, uint16_t key1, uint16_t key2, uint16_t key3, uint16_t key4, uint16_t key5, uint16_t key6) {
    uint16_t* keys = (uint16_t*)malloc(sizeof(uint16_t) * 8);
    keys[0] = 7;
    keys[1] = key0;
    keys[2] = key1;
    keys[3] = key2;
    keys[4] = key3;
    keys[5] = key4;
    keys[6] = key5;
    keys[7] = key6;
    return keys;
}

uint16_t* sendKeys(uint16_t key0, uint16_t key1, uint16_t key2, uint16_t key3, uint16_t key4, uint16_t key5, uint16_t key6, uint16_t key7) {
    uint16_t* keys = (uint16_t*)malloc(sizeof(uint16_t) * 9);
    keys[0] = 8;
    keys[1] = key0;
    keys[2] = key1;
    keys[3] = key2;
    keys[4] = key3;
    keys[5] = key4;
    keys[6] = key5;
    keys[7] = key6;
    keys[7] = key7;
    return keys;
}

uint16_t* sendKeys(uint16_t key0, uint16_t key1, uint16_t key2, uint16_t key3, uint16_t key4, uint16_t key5, uint16_t key6, uint16_t key7, uint16_t key8) {
    uint16_t* keys = (uint16_t*)malloc(sizeof(uint16_t) * 10);
    keys[0] = 9;
    keys[1] = key0;
    keys[2] = key1;
    keys[3] = key2;
    keys[4] = key3;
    keys[5] = key4;
    keys[6] = key5;
    keys[7] = key6;
    keys[8] = key7;
    keys[9] = key8;
    return keys;
}

uint16_t* sendKeys(uint16_t key0, uint16_t key1, uint16_t key2, uint16_t key3, uint16_t key4, uint16_t key5, uint16_t key6, uint16_t key7, uint16_t key8, uint16_t key9) {
    uint16_t* keys = (uint16_t*)malloc(sizeof(uint16_t) * 11);
    keys[0] = 10;
    keys[1] = key0;
    keys[2] = key1;
    keys[3] = key2;
    keys[4] = key3;
    keys[5] = key4;
    keys[6] = key5;
    keys[7] = key6;
    keys[8] = key7;
    keys[9] = key8;
    keys[10] = key9;
    return keys;
}
