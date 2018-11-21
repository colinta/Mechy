/* template:
KEYS(keys) = LAYOUT(
    _k00_  , _k01_  , _k02_  , _k03_  , _k04_  , _k05_  , _k06_  , _k07_  , _k08_  , _k09_  , _k0a_  , _k0b_  ,
    _k10_  , _k11_  , _k12_  , _k13_  , _k14_  , _k15_  , _k16_  , _k17_  , _k18_  , _k19_  , _k1a_  , _k1b_  ,
    _k20_  , _k21_  , _k22_  , _k23_  , _k24_  , _k25_  , _k26_  , _k27_  , _k28_  , _k29_  , _k2a_  , _k2b_  ,
    _k30_  , _k31_  , _k32_  , _k33_  , _k34_  , _k35_  , _k36_  , _k37_  , _k38_  , _k39_  , _k3a_  , _k3b_
);
*/

#define ROWS 5
#define COLS 15
const uint8_t pinRows[] = { _D0, _D5, _B5, _B6 }
const uint8_t pinCols[] = { _F1, _F0, _B0, _C7, _F4, _F5, _F6, _F7, _D4, _D6, _B4, _D7 }

#define LAYOUT( \
    k00, k01, k02, k03, k04, k05, k06, k07, k08, k09, k0a, k0b, \
    k10, k11, k12, k13, k14, k15, k16, k17, k18, k19, k1a, k1b, \
    k20, k21, k22, k23, k24, k25, k26, k27, k28, k29, k2a, k2b, \
    k30, k31, k32, k33, k34, k35, k36, k37, k38, k39, k3a, k3b  \
) { \
    k00, k01, k02, k03, k04, k05, k06, k07, k08, k09, k0a, k0b, \
    k10, k11, k12, k13, k14, k15, k16, k17, k18, k19, k1a, k1b, \
    k20, k21, k22, k23, k24, k25, k26, k27, k28, k29, k2a, k2b, \
    k30, k31, k32, k33, k34, k35, k36, k37, k38, k39, k3a, k3b  \
}

class Hardware {
public:
    Hardware(Mechy* _mechy) {
    }
    void begin() {
    }
    void tick() {
    }

private:
};
