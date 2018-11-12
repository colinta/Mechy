#include <Mechy.h>
#include <Mechy/Scanner.h>
#include <Mechy/KeyPress.h>
#include <Mechy/MediaKey.h>
#include <Mechy/Sticky.h>
#include <Mechy/Lock.h>
#include <Mechy/Macro.h>

#define ROWS 5
#define COLS 4
const uint8_t pinRows[] = {_D3, _D2, _D4, _C6, _D7};
const uint8_t pinCols[] = {_B5, _B6, _B2, _B3};

KBD keys[ROWS][COLS] = {
    // macros
    {  MM_0 , MM_1, MM_2, MM(3) },
    // some regular keys, using the shorthands and macros
    {  KC_A,   KC('b'), KC(KEY_F12), KC_BSPC  },
    // media keys: vol+, play, vol- and the "no-op" key
    {  MD_VOLU, MD_PLAY, MD_VOLD, MD(MEDIA_PLAY)  },
    // lock keys,     simple modifier keys
    {  LK_1 , LK_2,   KC_SFT, KC_GUI  },
    // sticky keys - hyper is ctrl+alt+gui
    {  ST_CTL , ST_ALT, ST_GUI, ST_HYPER  },
};


Scanner scanner = Scanner(&keys[0][0], pinRows, pinCols, ROWS, COLS);

Mechy mechy = Mechy();
Keypress keypress = Keypress();
MediaKey mediakey = MediaKey();
Sticky sticky = Sticky();
Lock lock = Lock();
const char* macros[4] = {"a", "1", "2", "four"};
Macro macro = Macro(4, macros);

void setup() {
    mechy.add(FN_KEYPRESS, &keypress);
    mechy.add(FN_MEDIA, &mediakey);
    mechy.add(FN_STICKY, &sticky);
    mechy.add(FN_LOCK, &lock);
    mechy.add(FN_MACRO, &macro);

    scanner.begin(&mechy);
}

void loop() {
    scanner.scan();
}
