#include <Mechy.h>
#include <Mechy/Wiring.h>
#include <Mechy/KeyPress.h>
#include <Mechy/MediaKey.h>
#include <Mechy/Sticky.h>
#include <Mechy/Lock.h>
#include <Mechy/Password.h>
#include <Mechy/Receiver.h>
#include <Mechy/Hardware/BFO9000.h>

#define LK_1 LK
#define LK_2 LK

// clang-format off
KEYS(leftKeys) = LAYOUT(
    PW_0  ,  ____  , KC_ESC , KC_F1  ,  KC_F2 ,  KC_F3 ,  KC_F4 ,  KC_F5 , KC_F6  ,
    PW_1  ,  ____  , KC_GRV ,  KC_1  ,  KC_2  ,  KC_3  ,  KC_4  ,  KC_5  , KC_DEL ,
    PW_2  ,  ____  , KC_TAB ,  KC_Q  ,  KC_W  ,  KC_F  ,  KC_P  ,  KC_G  , KC_BSPC,
    PW_3  ,  ____  , ST_SFT ,  KC_A  ,  KC_R  ,  KC_S  ,  KC_T  ,  KC_D  , KC_ENT ,
    PW_4  ,  ____  , ST_CTL ,  KC_Z  ,  KC_X  ,  KC_C  ,  KC_V  ,  KC_B  ,  LK_1  ,
    ____  ,  ____  ,  ____  ,  PW_6  ,  ____  ,  ST_ALT,  ____  ,  ST_GUI,  ____
);

KEYS(rightKeys) = LAYOUT(
    KC_F7 , KC_F8  , KC_F9  ,  KC_F10, KC_F11 , KC_F12 , MD_VOLD, MD_PLAY, MD_VOLU ,
    KC_DEL,  KC_6  ,  KC_7  ,  KC_8  , KC_9   ,  KC_0  , KC_MINS, KC_EQL , PW_5    ,
    KC_BSPC,  KC_J  ,  KC_L  ,  KC_U  , KC_Y   , KC_SCLN , KC_LBRC, KC_RBRC, KC_PGUP ,
    KC_ENT,  KC_H  ,  KC_N  ,  KC_E  , KC_I   ,  KC_O  , KC_QUOT,  ST_SFT, KC_PGDN ,
    LK_2  ,  KC_K  ,  KC_M  , KC_COMM , KC_DOT , KC_SLSH , KC_BSLS,  KC_UP , ST_HYP  ,
    ____  , KC_SPC ,  ____  ,  ____  , ____   ,  ____  , KC_LEFT, KC_DOWN, KC_RGHT
);
// clang-format on

Scanner scanner = Scanner(leftKeys, pinRows, pinCols, ROWS, COLS);
Mechy   mechy   = Mechy();

KeyPress keypress = KeyPress();
MediaKey mediakey = MediaKey();
Sticky   sticky   = Sticky();
Lock     lock     = Lock();
#define PASSWORDS 7
const char* passwords[PASSWORDS] = {"", "", "", "", "", "", ""};
Password    password             = Password(PASSWORDS, passwords);
Receiver    receiver             = Receiver(rightKeys, ROWS, COLS, DATA, CLK);

void setup() {
    Wiring::pinMode(_D5, INPUT);
    Wiring::pinMode(_B0, INPUT);

    mechy.add(&keypress);
    mechy.add(&mediakey);
    mechy.add(&sticky);
    mechy.add(&lock);
    mechy.add(&password);

    mechy.attach(&scanner);
    mechy.attach(&receiver);

    mechy.begin();
}

void loop() {
    mechy.tick();
}
