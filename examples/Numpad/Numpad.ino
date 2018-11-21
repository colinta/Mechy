#include <Mechy.h>
#include <Mechy/KeyPress.h>
#include <Mechy/MediaKey.h>
#include <Mechy/Sticky.h>
#include <Mechy/Lock.h>
#include <Mechy/Macro.h>

#define ROWS 6
#define COLS 4
const uint8_t pinRows[] = { _D3, _D2, _D4, _C6, _D7, _E6};
const uint8_t pinCols[] = { _B5, _B6, _B2, _B3};

KEYS(keys) = {
    // some regular keys, using the shorthands and the macros
    {  KC_A, KC_BSPC, KC('b'), KC(KEY_F12)  },
    // macros
    {  MM_0 , MM_1, MM_2, MM(3) },
    // media keys: vol+, play, vol-
    {  MD_VOLU, MD_PLAY, MD_VOLD, MD(MEDIA_PLAY)  },
    // lock keys,     simple modifier keys
    {  LK, LK,        KC_SFT, KC_GUI  },
    // sticky keys - hyper is ctrl+alt+shift+gui
    {  ST_CTL , ST_ALT, ST_GUI, ST_HYP  },
    // no-op and transparent keys (for when using layers)
    { ____, vvvv, vvvv, ____}
};


Scanner scanner = Scanner(keys, pinRows, pinCols, ROWS, COLS);

Mechy mechy = Mechy();
const char* macros[4] = {"abcdefg", "1234509876", "my password is password", "potato"};
Macro macro = Macro(4, macros);

void setup() {
    mechy.add(new KeyPress());
    mechy.add(new MediaKey());
    mechy.add(new Sticky());
    mechy.add(new Lock());
    mechy.add(&macro);

    mechy.attach(&scanner);

    mechy.begin();
}

void loop() {
    mechy.tick();
}
