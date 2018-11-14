#include <Mechy.h>
#include <Mechy/KeyPress.h>
#include <Mechy/MediaKey.h>
#include <Mechy/GotoLayer.h>
#include <Mechy/Macro.h>

#define ROWS 5
#define COLS 15
const uint8_t pinRows[] = { _D0, _D1, _D2, _D3, _D5};
const uint8_t pinCols[] = { _F0, _F1, _E6, _C7, _C6, _B6, _D4, _B1, _B7, _B5, _B4, _D7, _D6, _B3, _B0};


KBD mainKeys[] = {
     KC_ESC , KC_GRV ,  KC_1  ,  KC_2  ,  KC_3  ,  KC_4  ,  KC_5  , KC_DEL ,  KC_6  ,  KC_7  ,  KC_8  ,  KC_9  , KC_0   , KC_MINS, KC_EQL ,
      ____  , KC_TAB ,  KC_Q  ,  KC_W  ,  KC_F  ,  KC_P  ,  KC_G  , KC_BSPC,  KC_J  ,  KC_L  ,  KC_U  ,  KC_Y  , KC_SCLN, KC_LBRC, KC_RBRC,
      ____  , KC_LSFT,  KC_A  ,  KC_R  ,  KC_S  ,  KC_T  ,  KC_D  , KC_ENT ,  KC_H  ,  KC_N  ,  KC_E  ,  KC_I  , KC_O   , KC_QUOT, KC_BSLS,
      ____  , KC_LCTL,  KC_Z  ,  KC_X  ,  KC_C  ,  KC_V  ,  KC_B  , GOTO_1 ,  KC_K  ,  KC_M  , KC_COMM, KC_DOT , KC_SLSH, KC_UP  ,  ____  ,
      ____  , MD_PLAY,  ____  , KC_LALT,  ____  , KC_LGUI,  ____  ,  ____  , KC_SPC ,  ____  , KC_PGUP, KC_PGDN, KC_LEFT, KC_DOWN, KC_RGHT
};

KBD fnKeys[] = {
      ____  ,  ____  , KC_F1  , KC_F2  , KC_F3  , KC_F4  ,  KC_F5 ,  ____  , KC_F6  , KC_F7  , KC_F8  , KC_F9  , KC_F10 , KC_F11 , KC_F12 ,
      ____  ,  ____  ,  ____  ,  ____  ,  ____  ,  ____  ,  ____  ,  ____  ,  ____  ,  ____  ,  ____  ,  ____  ,  ____  ,  ____  ,  ____  ,
      ____  ,  vvvv  ,  ____  ,  MM_0  ,  MM_1  ,  MM_2  ,  ____  ,  ____  ,  ____  ,  MM_3  ,  MM_4  ,  MM_5  ,  ____  ,  ____  ,  ____  ,
      ____  ,  vvvv  ,  ____  ,  ____  ,  ____  ,  ____  , MD_VOLD,  ____  , MD_VOLU,  ____  ,  ____  ,  ____  ,  ____  ,  ____  ,  ____  ,
      ____  ,  ____  ,  ____  ,  vvvv  ,  ____  ,  vvvv  ,  ____  ,  ____  ,  ____  ,  ____  ,  ____  ,  ____  ,  ____  ,  ____  ,  ____
};


// make sure to exclude secrets.h from source-control for this trick to work well:
#if (__has_include("secrets.h"))
#include "secrets.h"
const char* macros[7] = {M0, M1, M2, M3, M4, M5, M6};
#else
const char* macros[7] = {"", "", "", "", "", "", ""};
#endif

Layout layout = Layout(ROWS, COLS, mainKeys, fnKeys);
Scanner scanner = Scanner(&layout, pinRows, pinCols, ROWS, COLS);

Mechy mechy = Mechy();
Keypress keypress = Keypress();
MediaKey mediaKey = MediaKey();
GotoLayer gotoLayer = GotoLayer();
Macro macro = Macro(7, macros);

void setup() {
    mechy.add(&keypress);
    mechy.add(&mediaKey);
    mechy.add(&gotoLayer);
    mechy.add(&macro);

    mechy.attach(&scanner);

    mechy.begin();
}

void loop() {
    mechy.tick();
}
