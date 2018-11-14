#include <Mechy.h>
#include <Mechy/KeyPress.h>
#include <Mechy/MediaKey.h>
#include <Mechy/GotoLayer.h>
#include <Mechy/Hardware/Planck.h>

KBD mainKeys[] = LAYOUT(
    KC_TAB , KC_Q   , KC_W   , KC_E   , KC_R   , KC_T   , KC_Y   , KC_U   , KC_I   , KC_O   , KC_P   , KC_BSPC,
    KC_ESC , KC_A   , KC_S   , KC_D   , KC_F   , KC_G   , KC_H   , KC_J   , KC_K   , KC_L   , KC_SCLN, KC_QUOT,
    KC_LSFT, KC_Z   , KC_X   , KC_C   , KC_V   , KC_B   , KC_N   , KC_M   , KC_COMM, KC_DOT , KC_SLSH, KC_ENT ,
      ____ , KC_LCTL, KC_LALT, KC_LGUI, LOWER  , KC_SPC , KC_SPC , RAISE  , KC_LEFT, KC_DOWN, KC_UP  , KC_RGHT
);

KBD lowerKeys[] = LAYOUT(
    KC_TILD, KC_EXLM, KC_AT  , KC_HASH, KC_DLR , KC_PERC, KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_BSPC ,
    KC_DEL , KC_F1  , KC_F2  , KC_F3  , KC_F4  , KC_F5  , KC_F6  , KC_UNDS, KC_PLUS, KC_LCBR, KC_RCBR, KC_PIPE ,
      ____ , KC_F7  , KC_F8  , KC_F9  , KC_F10 , KC_F11 , KC_F12 ,  ____  ,  ____  , KC_HOME, KC_END ,  ____   ,
      ____ ,  ____  ,  ____  ,  ____  ,  ____  ,  ____  ,  ____  ,  ____  , MD_NEXT, MD_VOLD, MD_VOLU, MD_PLAY
);

KBD upperKeys[] = LAYOUT(
    KC_GRV , KC_1   , KC_2   , KC_3   , KC_4   , KC_5   , KC_6   , KC_7   , KC_8   , KC_9   , KC_0   , KC_BSPC ,
    KC_DEL , KC_F1  , KC_F2  , KC_F3  , KC_F4  , KC_F5  , KC_F6  , KC_MINS, KC_EQL , KC_LBRC, KC_RBRC, KC_BSLS ,
      ____ , KC_F7  , KC_F8  , KC_F9  , KC_F10 , KC_F11 , KC_F12 ,  ____  ,  ____  , KC_PGUP, KC_PGDN,  ____   ,
      ____ ,  ____  ,  ____  ,  ____  ,  ____  ,  ____  ,  ____  ,  ____  , MD_NEXT, MD_VOLD, MD_VOLU, MD_PLAY
);


Layout layout = Layout(ROWS, COLS, mainKeys, lowerKeys, upperKeys);
Scanner scanner = Scanner(&layout, pinRows, pinCols, ROWS, COLS);

Mechy mechy = Mechy();
Hardware hardware = Hardware(&mechy);

void setup() {
    mechy.add(new Keypress());
    mechy.attach(&scanner);
    mechy.begin();
    hardware.begin();
}

void loop() {
    mechy.tick();
    hardware.tick();
}
