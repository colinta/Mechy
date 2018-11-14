#include <Mechy.h>
#include <Mechy/Wiring.h>
#include <Mechy/KeyPress.h>
#include <Mechy/MediaKey.h>
#include <Mechy/Sticky.h>
#include <Mechy/Lock.h>
#include <Mechy/Macro.h>
#include <Mechy/Receiver.h>

#define DATA _D1
#define CLK _D0

#define ROWS 6
#define COLS 9
const uint8_t pinRows[] = { _D3, _D2, _D4, _C6, _D7, _E6};
const uint8_t pinCols[] = { _B5, _B6, _B2, _B3, _B1, _F7, _F6, _F5, _F4};

KBD leftKeys[] = {
    MM_0  ,  ____  , KC_ESC , KC_F1  ,  KC_F2 ,  KC_F3 ,  KC_F4 ,  KC_F5 , KC_F6  ,
    MM_1  ,  ____  , KC_GRV ,  KC_1  ,  KC_2  ,  KC_3  ,  KC_4  ,  KC_5  , KC_DEL ,
    MM_2  ,  ____  , KC_TAB ,  KC_Q  ,  KC_W  ,  KC_F  ,  KC_P  ,  KC_G  , KC_BSPC,
    MM_3  ,  ____  , ST_SFT ,  KC_A  ,  KC_R  ,  KC_S  ,  KC_T  ,  KC_D  , KC_ENT ,
    MM_4  ,  ____  , ST_CTL ,  KC_Z  ,  KC_X  ,  KC_C  ,  KC_V  ,  KC_B  ,  LK_1  ,
    ____  ,  ____  ,  ____  ,  MM_6  ,  ____  ,  ST_ALT,  ____  ,  ST_GUI,  ____
};

KBD rightKeys[] = {
     KC_F7 , KC_F8  , KC_F9  ,  KC_F10, KC_F11 , KC_F12 , MD_VOLD, MD_PLAY, MD_VOLU ,
     KC_DEL,  KC_6  ,  KC_7  ,  KC_8  , KC_9   ,  KC_0  , KC_MINS, KC_EQL , MM_5    ,
    KC_BSPC,  KC_J  ,  KC_L  ,  KC_U  , KC_Y   ,KC_SCLN , KC_LBRC, KC_RBRC, KC_PGUP ,
     KC_ENT,  KC_H  ,  KC_N  ,  KC_E  , KC_I   ,  KC_O  , KC_QUOT,  ST_SFT, KC_PGDN ,
     LK_2  ,  KC_K  ,  KC_M  ,KC_COMM , KC_DOT ,KC_SLSH , KC_BSLS,  KC_UP , ST_HYP  ,
     ____  , KC_SPC ,  ____  ,  ____  , ____   ,  ____  , KC_LEFT, KC_DOWN, KC_RGHT
};


Scanner scanner = Scanner(leftKeys, pinRows, pinCols, ROWS, COLS);
Mechy mechy = Mechy();

KeyPress keypress = KeyPress();
MediaKey mediakey = MediaKey();
Sticky sticky = Sticky();
Lock lock = Lock();
const char* macros[7] = {"", "", "", "", "", "", ""};
Macro macro = Macro(7, macros);
Receiver receiver = Receiver(rightKeys, ROWS, COLS, DATA, CLK);

void setup() {
    Wiring::pinMode(_D5, INPUT);
    Wiring::pinMode(_B0, INPUT);

    mechy.add(&keypress);
    mechy.add(&mediakey);
    mechy.add(&sticky);
    mechy.add(&lock);
    mechy.add(&macro);

    mechy.attach(&scanner);
    mechy.attach(&receiver);

    mechy.begin();
}

void loop() {
    mechy.tick();
}
