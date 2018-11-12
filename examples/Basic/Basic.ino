#include <Mechy.h>
#include <Mechy/Scanner.h>
#include <Mechy/KeyPress.h>

#define ROWS 5
#define COLS 15
const uint8_t pinRows[] = {_D0, _D1, _D2, _D3, _D5};
const uint8_t pinCols[] = {_F0, _F1, _E6, _C7, _C6, _B6, _D4, _B1, _B7, _B5, _B4, _D7, _D6, _B3, _B0};


KBD mainKeys[] = {
    KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_MINS, KC_GRV,  KC_EQL,  KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSPC  ,
    KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_LBRC, KC_BSLS, KC_RBRC, KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_QUOT  ,
    KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_HOME, KC_DEL,  KC_PGUP, KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_ENT   ,
    KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_END,  KC_UP,   KC_PGDN, KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT  ,
    KC_LCTL, KC_LGUI, KC_LALT, ____,    KC_SPC,  KC_SPC,  KC_LEFT, KC_DOWN, KC_RGHT, KC_SPC,  KC_SPC,  ____,    KC_RALT, KC_RGUI, KC_RCTL  ,
};


const char* macros[7] = {"", "", "", "", "", "", ""};

Scanner scanner = Scanner(mainKeys, pinRows, pinCols, ROWS, COLS);

Mechy mechy = Mechy();
Keypress keypress = Keypress();

void setup() {
    mechy.add(FN_KEYPRESS, &keypress);

    scanner.begin(&mechy);
}

void loop() {
    scanner.scan();
}
