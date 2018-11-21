/* template:
KEYS(keys) = LAYOUT_standard(
    _k00_  , _k01_  , _k02_  , _k03_  , _k04_  , _k05_  , _k06_  , _k07_  , _k08_  ,
    _k10_  , _k11_  , _k12_  , _k13_  , _k14_  , _k15_  , _k16_  , _k17_  , _k18_  ,
    _k20_  , _k21_  , _k22_  , _k23_  , _k24_  , _k25_  , _k26_  , _k27_  , _k28_  ,
    _k30_  , _k31_  , _k32_  , _k33_  , _k34_  , _k35_  , _k36_  , _k37_  , _k38_  ,
    _k40_  , _k41_  , _k42_  , _k43_  , _k44_  , _k45_  , _k46_  , _k47_  , _k48_  ,
    _k50_  , _k51_  , _k52_  , _k53_  , _k54_  , _k55_  , _k56_  , _k57_  , _k58_
);
*/

#define ROWS 6
#define COLS 9
const uint8_t pinRows[] = { _D3, _D2, _D4, _C6, _D7, _E6};
const uint8_t pinCols[] = { _B5, _B6, _B2, _B3, _B1, _F7, _F6, _F5, _F4};

#define LAYOUT( \
    k00 , k01 , k02 , k03 , k04 , k05 , k06 , k07 , k08 , \
    k10 , k11 , k12 , k13 , k14 , k15 , k16 , k17 , k18 , \
    k20 , k21 , k22 , k23 , k24 , k25 , k26 , k27 , k28 , \
    k30 , k31 , k32 , k33 , k34 , k35 , k36 , k37 , k38 , \
    k40 , k41 , k42 , k43 , k44 , k45 , k46 , k47 , k48 , \
    k50 , k51 , k52 , k53 , k54 , k55 , k56 , k57 , k58   \
) { \
    k00 , k01 , k02 , k03 , k04 , k05 , k06 , k07 , k08 , \
    k10 , k11 , k12 , k13 , k14 , k15 , k16 , k17 , k18 , \
    k20 , k21 , k22 , k23 , k24 , k25 , k26 , k27 , k28 , \
    k30 , k31 , k32 , k33 , k34 , k35 , k36 , k37 , k38 , \
    k40 , k41 , k42 , k43 , k44 , k45 , k46 , k47 , k48 , \
    k50 , k51 , k52 , k53 , k54 , k55 , k56 , k57 , k58   \
}

#define DATA _D1
#define CLK _D0
#define RGB_PIN _B4

class Hardware {
public:
    Hardware(Mechy* UNUSED(_mechy)) {
    }
    void begin() {
    }
    void tick() {
    }

private:
};
