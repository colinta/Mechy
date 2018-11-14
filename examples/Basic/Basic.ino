#include <Mechy.h>
#include <Mechy/KeyPress.h>

#define ROWS 5
#define COLS 15
const uint8_t pinRows[] = { _D0, _D5, _B5, _B6 }
const uint8_t pinCols[] = { _F1, _F0, _B0, _C7, _F4, _F5, _F6, _F7, _D4, _D6, _B4, _D7 }


KBD keys[] = {
    KC_TAB , KC_Q   , KC_W   , KC_E   , KC_R   , KC_T   , KC_Y   , KC_U   , KC_I   , KC_O   , KC_P   , KC_BSPC,
    KC_ESC , KC_A   , KC_S   , KC_D   , KC_F   , KC_G   , KC_H   , KC_J   , KC_K   , KC_L   , KC_SCLN, KC_QUOT,
    KC_LSFT, KC_Z   , KC_X   , KC_C   , KC_V   , KC_B   , KC_N   , KC_M   , KC_COMM, KC_DOT , KC_SLSH, KC_ENT ,
      ____ , KC_LCTL, KC_LALT, KC_LGUI,  ____  , KC_SPC , KC_SPC ,  ____  , KC_LEFT, KC_DOWN, KC_UP  , KC_RGHT
};

Scanner scanner = Scanner(keys, pinRows, pinCols, ROWS, COLS);

Mechy mechy = Mechy();

void setup() {
    mechy.add(new Keypress());
    mechy.attach(&scanner);
    mechy.begin();
}

void loop() {
    mechy.tick();
}
