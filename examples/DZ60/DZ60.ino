#include <Mechy.h>
#include <Mechy/KeyPress.h>
#include <Mechy/GotoLayer.h>
#include <Mechy/Hardware/DZ60.h>

KBD mainKeys[] = LAYOUT_60(
    KC_ESC , KC_1   , KC_2   , KC_3   , KC_4   , KC_5   , KC_6   , KC_7   , KC_8   , KC_9   , KC_0   , KC_MINS, KC_EQL ,     KC_BSPC     ,
     KC_TAB  ,  KC_Q  , KC_W   , KC_E   , KC_R   , KC_T   , KC_Y   , KC_U   , KC_I   , KC_O   , KC_P   , KC_LBRC, KC_RBRC,    KC_BSLS    ,
      KC_CAPS  ,  KC_A   , KC_S  , KC_D   , KC_F   , KC_G   , KC_H   , KC_J   , KC_K   , KC_L   , KC_SCLN, KC_QUOT,        KC_ENT        ,
       KC_LSFT   ,  KC_Z   , KC_X  , KC_C   , KC_V   , KC_B   , KC_N   , KC_M   , KC_COMM, KC_DOT , KC_SLSH,           KC_RSFT           ,
     KC_LCTL ,  KC_LGUI  ,  KC_LALT  ,                    KC_SPC                        ,  KC_RALT  ,   RAISE   ,   LOWER   ,    KC_RCTL
);

KBD lowerKeys[] = LAYOUT_60(
     ____  ,  KC_F1 ,  KC_F2 ,  KC_F3 ,  KC_F4 ,  KC_F5 ,  KC_F6 ,  KC_F7 ,  KC_F8 ,  KC_F9 , KC_F10 , KC_F11 , KC_F12 ,      KC_DEL     ,
      ____   ,   ____ ,  ____  ,  ____  ,  ____  ,  ____  ,  ____  ,  ____  ,  ____  ,  ____  ,  ____  ,  ____  ,  ____  ,      ____     ,
       ____    ,   ____  ,  ____ ,  ____  ,  ____  ,  ____  ,  ____  ,  ____  ,  ____  ,  ____  ,  ____  ,  ____  ,         ____         ,
        ____     ,   ____  ,  ____ ,  ____  ,  ____  ,  ____  ,  ____  ,  ____  ,  ____  ,  ____  ,  ____  ,             ____            ,
      ____   ,    ____   ,    ____   ,                     ____                         ,    ____   ,    ____   ,    ____   ,    ____
);

KBD upperKeys[] = LAYOUT_60(
     ____  ,  ____  ,  ____  ,  ____  ,  ____  ,  ____  ,  ____  ,  ____  ,  ____  ,  ____  ,  ____  ,  ____  ,  ____  ,       ____      ,
      ____   ,   ____ ,  ____  ,  ____  ,  ____  ,  ____  ,  ____  ,  ____  ,  ____  ,  ____  ,  ____  ,  ____  ,  ____  ,      ____     ,
       ____    ,   ____  ,  ____ ,  ____  ,  ____  ,  ____  ,  ____  ,  ____  ,  ____  ,  ____  ,  ____  ,  ____  ,         ____         ,
        ____     ,   ____  ,  ____ ,  ____  ,  ____  ,  ____  ,  ____  ,  ____  ,  ____  ,  ____  ,  ____  ,             ____            ,
      ____   ,    ____   ,    ____   ,                     ____                         ,    ____   ,    ____   ,    ____   ,    ____
);


Layout layout = Layout(ROWS, COLS, mainKeys, lowerKeys, upperKeys);
Scanner scanner = Scanner(&layout, pinRows, pinCols, ROWS, COLS);

Mechy mechy = Mechy();
Hardware hardware = Hardware(&mechy);

void setup() {
    mechy.add(new KeyPress());
    mechy.add(new GotoLayer());

    mechy.attach(&scanner);

    mechy.begin();
    hardware.begin();
}

void loop() {
    mechy.tick();
    hardware.tick();
}
