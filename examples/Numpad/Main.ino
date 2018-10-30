#include <Mechy.h>
#include <Mechy/Scanner.h>
#include <Mechy/KeyPress.h>
#include <Mechy/MediaKey.h>
#include <Mechy/Sticky.h>
#include <Mechy/Lock.h>
#include <Mechy/Macro.h>
#include <Mechy/Receiver.h>

#define DATA 2
#define CLK 3

#define ROWS 6
#define COLS 9
const uint8_t pinRows[] = {1, 0, 4, 5, 6, 7};
const uint8_t pinCols[] = {9, 10, 16, 14, 15, 18, 19, 20, 21};


KBD mainKeys[ROWS][COLS] = {
    {  MM_0 ,   ____,   KC_ESC, KC_F1 , KC_F2 , KC_F3 , KC_F4 , KC_F5 , KC_F6 },
    {  MM_1 ,   ____,   KC_GRV, KC_1  , KC_2  , KC_3  , KC_4  , KC_5  , KC_DEL },
    {  MM_2 ,   ____,   KC_TAB, KC_Q  , KC_W  , KC_F  , KC_P  , KC_G  , KC_BSPC },
    {  MM_3 ,   ____,   ST_SFT, KC_A  , KC_R  , KC_S  , KC_T  , KC_D  , KC_ENT },
    {  MM_4 ,   ____,   ST_CTL, KC_Z  , KC_X  , KC_C  , KC_V  , KC_B  , LK_1 },
    {  ____ ,   ____,   ____  , MM_6  , ____  , ST_ALT, ____  , ST_GUI, ____ }
};

KBD receiverKeys[ROWS][COLS] = {
    { KC_F7,  KC_F8 , KC_F9 , KC_F10, KC_F11, KC_F12, MD_VOLD,MD_PLAY, MD_VOLU },
    { KC_DEL, KC_6  , KC_7  , KC_8  , KC_9  ,  KC_0 , KC_MINS,KC_EQL , MM_5    },
    { KC_BSPC,KC_J  , KC_L  , KC_U  , KC_Y  ,KC_SCLN, KC_LBRK,KC_RBRK, KC_PGUP },
    { KC_ENT, KC_H  , KC_N  , KC_E  , KC_I  ,  KC_O , KC_APOS, ST_SFT, KC_PGDN },
    { LK_2,   KC_K  , KC_M  ,KC_COMM,KC_PERD,KC_FSLH, KC_BSLH, KC_UP , ST_HYP  },
    { ____,   KC_SPC, ____  ,  ____ , ____  ,  ____ , KC_LT  , KC_DN , KC_RT   }
};


const char *macros[7] = {"", "", "", "", "", "", ""};

Scanner scanner = Scanner(&mainKeys[0][0], pinRows, pinCols, ROWS, COLS);

Mechy mechy = Mechy();
Keypress keypress = Keypress();
MediaKey mediakey = MediaKey();
Sticky sticky = Sticky();
Lock lock = Lock();
Macro macro = Macro(7, macros);
Receiver receiver = Receiver(&receiverKeys[0][0], ROWS, COLS, DATA, CLK);

void setup() {
    mechy.add(FN_KEYPRESS, &keypress);
    mechy.add(FN_MEDIA, &mediakey);
    mechy.add(FN_STICKY, &sticky);
    mechy.add(FN_LOCK, &lock);
    mechy.add(FN_MACRO, &macro);
    mechy.add(FN_NONE, &receiver);

    scanner.begin(&mechy);
}

void loop() {
    scanner.scan();
}
