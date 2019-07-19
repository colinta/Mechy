/* templates:

KEYS(keys) = LAYOUT(
    _k00_  , _k01_  , _k02_  , _k03_  , _k04_  , _k05_  , _k06_  , _k07_  , _k08_  , _k09_  , _k0a_  , _k0b_  , _k0c_  , _k0d_  , _k0e_  ,
     _k10_   ,  _k12_ , _k13_  , _k14_  , _k15_  , _k16_  , _k17_  , _k18_  , _k19_  , _k1a_  , _k1b_  , _k1c_  , _k1d_  ,     _k1e_     ,
      _k20_    ,  _k22_  , _k23_ , _k24_  , _k25_  , _k26_  , _k27_  , _k28_  , _k29_  , _k2a_  , _k2b_  , _k2c_  ,        _k2d_         ,
     _k30_ , _k31_  , _k32_  , _k33_  , _k34_  , _k35_  , _k36_  , _k37_  , _k38_  , _k39_  , _k3a_  , _k3b_  , _k3c_  ,  _k3d_ , _k3e_  ,
     _k40_   ,   _k41_   ,   _k43_   ,   _k44_   ,           _k46_          ,    _k48_      , _k4a_  , _k4b_  , _k4c_  ,  _k4d_ , _k4e_
);

KEYS(keys) = LAYOUT_standard(
    _k00_  , _k01_  , _k02_  , _k03_  , _k04_  , _k05_  , _k06_  , _k07_  , _k08_  , _k09_  , _k0a_  , _k0b_  , _k0c_  , _k0d_  , _k0e_  ,
     _k10_   ,  _k12_ , _k13_  , _k14_  , _k15_  , _k16_  , _k17_  , _k18_  , _k19_  , _k1a_  , _k1b_  , _k1c_  , _k1d_  ,     _k1e_     ,
      _k20_    ,  _k22_  , _k23_ , _k24_  , _k25_  , _k26_  , _k27_  , _k28_  , _k29_  , _k2a_  , _k2b_  , _k2c_  ,        _k2d_         ,
     _k30_ , _k31_  , _k32_  , _k33_  , _k34_  , _k35_  , _k36_  , _k37_  , _k38_  , _k39_  , _k3a_  , _k3b_  ,     _k3d_       , _k3e_  ,
     _k40_   ,   _k41_   ,   _k43_   ,   _k44_   ,           _k46_          ,    _k48_      , _k4a_  , _k4b_  , _k4c_  ,  _k4d_ , _k4e_
);

KEYS(keys) = LAYOUT_60(
    _k00_  , _k01_  , _k02_  , _k03_  , _k04_  , _k05_  , _k06_  , _k07_  , _k08_  , _k09_  , _k0a_  , _k0b_  , _k0c_  ,      _k0e_      ,
     _k10_   ,  _k12_ , _k13_  , _k14_  , _k15_  , _k16_  , _k17_  , _k18_  , _k19_  , _k1a_  , _k1b_  , _k1c_  , _k1d_  ,     _k1e_     ,
      _k20_    ,  _k22_  , _k23_ , _k24_  , _k25_  , _k26_  , _k27_  , _k28_  , _k29_  , _k2a_  , _k2b_  , _k2c_  ,        _k2d_         ,
       _k30_     ,  _k32_  , _k33_ , _k34_  , _k35_  , _k36_  , _k37_  , _k38_  , _k39_  , _k3a_  , _k3b_  ,            _k3d_            ,
     _k40_   ,   _k41_   ,   _k43_   ,                    _k46_                         ,   _k4a_   ,   _k4b_   ,   _k4d_   ,   _k4e_
);

*/

#define LAYOUT( \
    k00,  k01,  k02,  k03,  k04,  k05,  k06,  k07,  k08,  k09,  k0a,  k0b,  k0c,  k0d,  k0e, \
    k10,        k12,  k13,  k14,  k15,  k16,  k17,  k18,  k19,  k1a,  k1b,  k1c,  k1d,  k1e, \
    k20,        k22,  k23,  k24,  k25,  k26,  k27,  k28,  k29,  k2a,  k2b,  k2c,  k2d,       \
    k30,  k31,  k32,  k33,  k34,  k35,  k36,  k37,  k38,  k39,  k3a,  k3b,  k3c,  k3d,  k3e, \
    k40,  k41,        k43,  k44,        k46,        k48,        k4a,  k4b,  k4c,  k4d,  k4e  \
) { \
    k00,  k01,  k02,  k03,  k04,  k05,  k06,  k07,  k08,  k09,  k0a,  k0b,  k0c,  k0d,  k0e, \
    k10, ____,  k12,  k13,  k14,  k15,  k16,  k17,  k18,  k19,  k1a,  k1b,  k1c,  k1d,  k1e, \
    k20, ____,  k22,  k23,  k24,  k25,  k26,  k27,  k28,  k29,  k2a,  k2b,  k2c,  k2d, ____, \
    k30,  k31,  k32,  k33,  k34,  k35,  k36,  k37,  k38,  k39,  k3a,  k3b,  k3c,  k3d,  k3e, \
    k40,  k41, ____,  k43,  k44, ____,  k46, ____,  k48, ____,  k4a,  k4b,  k4c,  k4d,  k4e  \
}

#define LAYOUT_standard( \
    k00,  k01,  k02,  k03,  k04,  k05,  k06,  k07,  k08,  k09,  k0a,  k0b,  k0c,  k0d,  k0e, \
    k10,        k12,  k13,  k14,  k15,  k16,  k17,  k18,  k19,  k1a,  k1b,  k1c,  k1d,  k1e, \
    k20,        k22,  k23,  k24,  k25,  k26,  k27,  k28,  k29,  k2a,  k2b,  k2c,  k2d,       \
    k30,  k31,  k32,  k33,  k34,  k35,  k36,  k37,  k38,  k39,  k3a,  k3b,        k3d,  k3e, \
    k40,  k41,        k43,  k44,        k46,        k48,        k4a,  k4b,  k4c,  k4d,  k4e  \
) { \
    k00,  k01,  k02,  k03,  k04,  k05,  k06,  k07,  k08,  k09,  k0a,  k0b,  k0c, ____,  k0e, \
    k10, ____,  k12,  k13,  k14,  k15,  k16,  k17,  k18,  k19,  k1a,  k1b,  k1c,  k1d,  k1e, \
    k20, ____,  k22,  k23,  k24,  k25,  k26,  k27,  k28,  k29,  k2a,  k2b,  k2c,  k2d, ____, \
    k30,  k31,  k32,  k33,  k34,  k35,  k36,  k37,  k38,  k39,  k3a,  k3b, ____,  k3d,  k3e, \
    k40,  k41, ____,  k43,  k44, ____,  k46, ____,  k48, ____,  k4a,  k4b,  k4c,  k4d,  k4e  \
}

#define LAYOUT_60( \
    k00,  k01,  k02,  k03,  k04,  k05,  k06,  k07,  k08,  k09,  k0a,  k0b,  k0c,        k0e, \
    k10,        k12,  k13,  k14,  k15,  k16,  k17,  k18,  k19,  k1a,  k1b,  k1c,  k1d,  k1e, \
    k20,        k22,  k23,  k24,  k25,  k26,  k27,  k28,  k29,  k2a,  k2b,  k2c,  k2d,       \
    k30,        k32,  k33,  k34,  k35,  k36,  k37,  k38,  k39,  k3a,  k3b,        k3d,       \
    k40,  k41,        k43,              k46,                    k4a,  k4b,        k4d,  k4e  \
) { \
    k00,  k01,  k02,  k03,  k04,  k05,  k06,  k07,  k08,  k09,  k0a,  k0b,  k0c, ____,  k0e, \
    k10, ____,  k12,  k13,  k14,  k15,  k16,  k17,  k18,  k19,  k1a,  k1b,  k1c,  k1d,  k1e, \
    k20, ____,  k22,  k23,  k24,  k25,  k26,  k27,  k28,  k29,  k2a,  k2b,  k2c,  k2d, ____, \
    k30, ____,  k32,  k33,  k34,  k35,  k36,  k37,  k38,  k39,  k3a,  k3b, ____,  k3d, ____, \
    k40,  k41, ____,  k43, ____, ____,  k46, ____, ____, ____,  k4a,  k4b, ____,  k4d,  k4e  \
}

#include "../Wiring.h"

#define CAPS_LOCK_LED_PIN _B2
#define RGB_PIN _E2

#define ROWS 5
#define COLS 15
const uint8_t pinRows[] = { _D0, _D1, _D2, _D3, _D5 };
const uint8_t pinCols[] = { _F0, _F1, _E6, _C7, _C6, _B7, _D4, _B1, _B0, _B5, _B4, _D7, _D6, _B3, _F4 };

class Hardware {
public:
    Hardware(Mechy* _mechy) {
        mechy = _mechy;
        didSetCaps = false;
    }
    void begin() {
        Wiring::pinMode(CAPS_LOCK_LED_PIN, OUTPUT);
        Wiring::digitalWrite(CAPS_LOCK_LED_PIN, LOW);
    }
    void tick() {
        if (!didSetCaps) {
            Wiring::digitalWrite(CAPS_LOCK_LED_PIN, mechy->isCapsOn());
        }
    }
    void capsLockLedWrite(bool turnOn) {
        didSetCaps = true;
        Wiring::digitalWrite(CAPS_LOCK_LED_PIN, turnOn);
    }
    bool capsLockLedRead() {
        return Wiring::digitalRead(CAPS_LOCK_LED_PIN);
    }

private:
    Mechy* mechy;
    bool didSetCaps;
};
