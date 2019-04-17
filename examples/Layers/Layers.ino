#include <Mechy.h>
#include <Mechy/KeyPress.h>
#include <Mechy/MediaKey.h>
#include <Mechy/GotoLayer.h>
#include <Mechy/Password.h>
#include <Mechy/TapHold.h>

#define ROWS 5
#define COLS 15
const uint8_t pinRows[] = { _D0, _D1, _D2, _D3, _D5};
const uint8_t pinCols[] = { _F0, _F1, _E6, _C7, _C6, _B6, _D4, _B1, _B7, _B5, _B4, _D7, _D6, _B3, _B0};

// press and hold 1/2/3 for F1/F2/F3
#define TH_PGUP TH(KC_PGUP, KC_HOME)
#define TH_PGDN TH(KC_PGDN, KC_END)
#define TH_PLAY TH(MD_PLAY, MD_MUTE)


KEYS(mainKeys) = {
     KC_ESC , KC_GRV ,  TH_1  ,  TH_2  ,  TH_3  ,  TH_4  ,  TH_5  , TH_DEL ,  TH_6  ,  TH_7  ,  TH_8  ,  TH_9  , TH_0   , KC_MINS, KC_EQL ,
      ____  , KC_TAB ,  KC_Q  ,  KC_W  ,  KC_F  ,  KC_P  ,  KC_G  , KC_BSPC,  KC_J  ,  KC_L  ,  KC_U  ,  KC_Y  , KC_SCLN, KC_LBRC, KC_RBRC,
      ____  , KC_LSFT,  KC_A  ,  KC_R  ,  KC_S  ,  KC_T  ,  KC_D  , KC_ENT ,  KC_H  ,  KC_N  ,  KC_E  ,  KC_I  , KC_O   , KC_QUOT, KC_BSLS,
      ____  , KC_LCTL,  KC_Z  ,  KC_X  ,  KC_C  ,  KC_V  ,  KC_B  , GOTO_1 ,  KC_K  ,  KC_M  , KC_COMM, KC_DOT , KC_SLSH, KC_UP  ,  ____  ,
      ____  , TH_PLAY,  ____  , KC_LALT,  ____  , KC_LGUI,  ____  ,  ____  , KC_SPC ,  ____  , TH_PGUP, TH_PGDN, KC_LEFT, KC_DOWN, KC_RGHT
};

KEYS(fnKeys) = {
      ____  ,  ____  , KC_F1  , KC_F2  , KC_F3  , KC_F4  ,  KC_F5 ,  ____  , KC_F6  , KC_F7  , KC_F8  , KC_F9  , KC_F10 , KC_F11 , KC_F12 ,
      ____  ,  ____  ,  ____  ,  ____  ,  ____  ,  ____  ,  ____  ,  ____  ,  ____  ,  ____  ,  ____  ,  ____  ,  ____  ,  ____  ,  ____  ,
      ____  ,  vvvv  ,  ____  ,  PW_0  ,  PW_1  ,  PW_2  ,  ____  ,  ____  ,  ____  ,  PW_3  ,  PW_4  ,  PW_5  ,  ____  ,  ____  ,  ____  ,
      ____  ,  vvvv  ,  ____  ,  ____  ,  ____  ,  ____  , MD_VOLD,  ____  , MD_VOLU,  ____  ,  ____  ,  ____  ,  ____  ,  ____  ,  ____  ,
      ____  ,  ____  ,  ____  ,  vvvv  ,  ____  ,  vvvv  ,  ____  ,  ____  ,  ____  ,  ____  ,  ____  ,  ____  ,  ____  ,  ____  ,  ____
};


// make sure to exclude secrets.h from source-control for this trick to work well:
#if (__has_include("secrets.h"))
#include "secrets.h"
#define PASSWORDS 7
const char* passwords[PASSWORDS] = {M0, M1, M2, M3, M4, M5, M6};
#else
#define PASSWORDS 0
const char* passwords[] = {};
#endif

Layout layout = Layout(ROWS, COLS, mainKeys, fnKeys);
Scanner scanner = Scanner(&layout, pinRows, pinCols, ROWS, COLS);

Mechy mechy = Mechy();

void setup() {
    TapHold* tapHold = new TapHold();
    tapHold->add(KC_1, KC_F1);
    tapHold->add(KC_2, KC_F2);
    tapHold->add(KC_3, KC_F3);
    tapHold->add(KC_4, KC_F4);
    tapHold->add(KC_5, KC_F5);
    tapHold->add(KC_6, KC_F6);
    tapHold->add(KC_7, KC_F7);
    tapHold->add(KC_8, KC_F8);
    tapHold->add(KC_9, KC_F9);
    tapHold->add(KC_0, KC_F10);
    tapHold->add(KC_PGUP, KC_HOME);
    tapHold->add(KC_PGDN, KC_END);
    tapHold->add(MD_PLAY, MD_MUTE);

    mechy.add(new KeyPress());
    mechy.add(new MediaKey());
    mechy.add(new GotoLayer());
    mechy.add(new Password(PASSWORDS, passwords));
    mechy.add(tapHold);

    mechy.attach(&scanner);

    mechy.begin();
}

void loop() {
    mechy.tick();
}
