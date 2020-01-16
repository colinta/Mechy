#include <Mechy/Wiring.h>

/* template:
KEYS(keys_left) = LAYOUT_left(
     k00 , k01 , k02 , k03 , k04 , k05 ,
     k10 , k11 , k12 , k13 , k14 , k15 ,
     k20 , k21 , k22 , k23 , k24 , k25 ,
     k30 , k31 , k32 , k33 , k34 , k35 , k45 ,
                             k42 , k43 , k44
);

KEYS(keys_right) = LAYOUT_right(
     k00 , k01 , k02 , k03 , k04 , k05 ,
     k10 , k11 , k12 , k13 , k14 , k15 ,
     k20 , k21 , k22 , k23 , k24 , k25 ,
k45, k30 , k31 , k32 , k33 , k34 , k35 ,
k44, k43 , k42
);
*/

#define ROWS 5
#define COLS 6
const uint8_t pinRows[] = { _D7, _E6, _B4, _D2, _D4 };
const uint8_t pinCols[] = { _F6, _F7, _B1, _B3, _B2, _B6 };

#define LAYOUT_left( \
     k00 , k01 , k02 , k03 , k04 , k05 , \
     k10 , k11 , k12 , k13 , k14 , k15 , \
     k20 , k21 , k22 , k23 , k24 , k25 , \
     k30 , k31 , k32 , k33 , k34 , k35 , k42 ,\
                             k43 , k44 , k45 \
) { \
     k00 , k01 , k02 , k03 , k04 , k05 , \
     k10 , k11 , k12 , k13 , k14 , k15 , \
     k20 , k21 , k22 , k23 , k24 , k25 , \
     k30 , k31 , k32 , k33 , k34 , k35 , \
     ____, ____, k42 , k43 , k44 , k45   \
}

#define LAYOUT_right( \
     k05 , k04 , k03 , k02 , k01 , k00 , \
     k15 , k14 , k13 , k12 , k11 , k10 , \
     k25 , k24 , k23 , k22 , k21 , k20 , \
k42, k35 , k34 , k33 , k32 , k31 , k30 ,\
k45, k44 , k43 \
) { \
     k00 , k01 , k02 , k03 , k04 , k05 , \
     k10 , k11 , k12 , k13 , k14 , k15 , \
     k20 , k21 , k22 , k23 , k24 , k25 , \
     k30 , k31 , k32 , k33 , k34 , k35 , \
     ____, ____, k42 , k43 , k44 , k45   \
}

#define DATA _D1
#define CLK _D0
#define KEYCAPS_LED_PIN _B5


class Hardware {
public:
    Hardware(Mechy* UNUSED(_mechy)) {
    }
    void begin() {
        Wiring::pinMode(KEYCAPS_LED_PIN, OUTPUT);
        Wiring::digitalWrite(KEYCAPS_LED_PIN, HIGH);
    }
    void tick() {
    }
    void keycapsLedsWrite(bool turnOn) {
        Wiring::digitalWrite(KEYCAPS_LED_PIN, turnOn);
    }
    bool keycapsLedsRead() {
        return Wiring::digitalRead(KEYCAPS_LED_PIN);
    }

private:
};
