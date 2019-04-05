#undef  TXLED0
#define TXLED0
#undef  TXLED1
#define TXLED1
#undef  RXLED0
#define RXLED0
#undef  RXLED1
#define RXLED1

/* template:
KEYS(keys) = LAYOUT(
    _k00_  , _k01_  , _k02_  , _k03_  , _k04_  , _k05_  , _k06_  , _k07_  , _k08_  , _k09_  , _k0a_  , _k0b_  , _k0c_  , _k0d_  , _k0e_  ,
    _k10_  , _k11_  , _k12_  , _k13_  , _k14_  , _k15_  , _k16_  , _k17_  , _k18_  , _k19_  , _k1a_  , _k1b_  , _k1c_  , _k1d_  , _k1e_  ,
    _k20_  , _k21_  , _k22_  , _k23_  , _k24_  , _k25_  , _k26_  , _k27_  , _k28_  , _k29_  , _k2a_  , _k2b_  , _k2c_  , _k2d_  , _k2e_  ,
    _k30_  , _k31_  , _k32_  , _k33_  , _k34_  , _k35_  , _k36_  , _k37_  , _k38_  , _k39_  , _k3a_  , _k3b_  , _k3c_  , _k3d_  , _k3e_  ,
    _k40_  , _k41_  , _k42_  , _k43_  , _k44_  , _k45_  , _k46_  , _k47_  , _k48_  , _k49_  , _k4a_  , _k4b_  , _k4c_  , _k4d_  , _k4e_
);
*/

#define ROWS 5
#define COLS 15
const uint8_t pinRows[] = { _D0, _D1, _D2, _D3, _D5 };
const uint8_t pinCols[] = { _F0, _F1, _E6, _C7, _C6, _B6, _D4, _B1, _B7, _B5, _B4, _D7, _D6, _B3, _B0 };

#define LAYOUT( \
    k00, k01, k02, k03, k04, k05, k06, k07, k08, k09, k0a, k0b, k0c, k0d, k0e, \
    k10, k11, k12, k13, k14, k15, k16, k17, k18, k19, k1a, k1b, k1c, k1d, k1e, \
    k20, k21, k22, k23, k24, k25, k26, k27, k28, k29, k2a, k2b, k2c, k2d, k2e, \
    k30, k31, k32, k33, k34, k35, k36, k37, k38, k39, k3a, k3b, k3c, k3d, k3e, \
    k40, k41, k42, k43, k44, k45, k46, k47, k48, k49, k4a, k4b, k4c, k4d, k4e  \
) { \
    k00, k01, k02, k03, k04, k05, k06, k07, k08, k09, k0a, k0b, k0c, k0d, k0e, \
    k10, k11, k12, k13, k14, k15, k16, k17, k18, k19, k1a, k1b, k1c, k1d, k1e, \
    k20, k21, k22, k23, k24, k25, k26, k27, k28, k29, k2a, k2b, k2c, k2d, k2e, \
    k30, k31, k32, k33, k34, k35, k36, k37, k38, k39, k3a, k3b, k3c, k3d, k3e, \
    k40, k41, k42, k43, k44, k45, k46, k47, k48, k49, k4a, k4b, k4c, k4d, k4e  \
}

#define RGB_PIN _F6
#define CAPSLOCK_LED_PIN _B2
#define GP100_PIN    _F7
#define GP103_PIN    _F4
#define KEYCAPS_LED_PIN  _F5

class Hardware {
public:
    Hardware(Mechy* _mechy, Scanner* scanner) {
        mechy = _mechy;
        scanner->usePullup(true);
        didSetCaps = false;
    }
    void begin() {
        Wiring::pinMode(CAPSLOCK_LED_PIN, OUTPUT);
        Wiring::digitalWrite(CAPSLOCK_LED_PIN, HIGH);
        Wiring::pinMode(GP100_PIN, OUTPUT);
        Wiring::digitalWrite(GP100_PIN, HIGH);
        Wiring::pinMode(GP103_PIN, OUTPUT);
        Wiring::digitalWrite(GP103_PIN, LOW);
        Wiring::pinMode(KEYCAPS_LED_PIN, OUTPUT);
        Wiring::digitalWrite(KEYCAPS_LED_PIN, LOW);
        PORTF &= ~(1 << 5);
    }
    void tick() {
        if (!didSetCaps) {
            Wiring::digitalWrite(CAPSLOCK_LED_PIN, !mechy->isCapsOn());
        }
    }
    void capsLockLedWrite(bool turnOn) {
        didSetCaps = true;
        Wiring::digitalWrite(CAPSLOCK_LED_PIN, !turnOn);
    }
    bool capsLockLedRead() {
        return Wiring::digitalRead(CAPSLOCK_LED_PIN);
    }
    void gp100Write(bool turnOn) {
        Wiring::digitalWrite(GP100_PIN, !turnOn);
    }
    bool gp100Read() {
        return Wiring::digitalRead(GP100_PIN);
    }
    void gp103Write(bool turnOn) {
        Wiring::digitalWrite(GP103_PIN, turnOn);
    }
    bool gp103Read() {
        return Wiring::digitalRead(GP103_PIN);
    }
    void keycapsLedsWrite(bool turnOn) {
        Wiring::digitalWrite(KEYCAPS_LED_PIN, !turnOn);
    }
    bool keycapsLedsRead() {
        return Wiring::digitalRead(KEYCAPS_LED_PIN);
    }

private:
    Mechy* mechy;
    bool didSetCaps;
};
