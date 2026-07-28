#include "Alloc.h"

void* mechyAlloc(size_t size) {
    return malloc(size);
}

void mechyFree(void* ptr) {
    free(ptr);
}

void* mechyAllocOrHalt(size_t size, uint8_t haltCode) {
    void* ptr = mechyAlloc(size);
    if (!ptr) {
        mechyHalt(haltCode);
    }
    return ptr;
}

void mechyHalt(uint8_t haltCode) {
    // Serial may not be initialized yet - global constructors (e.g. a global
    // Layout, or sendMacro() initializers) run before setup().  begin() is
    // safe to call even if the sketch already called it.
    Serial.begin(9600);
#ifdef LED_BUILTIN
    pinMode(LED_BUILTIN, OUTPUT);
#endif

    // interrupts stay enabled (no cli()) so USB CDC keeps running: the
    // 1200-baud-touch auto-reset still works and the board is re-flashable
    // without the physical reset button.
    for (;;) {
        Serial.print(F("MECHY HALT: allocation failed, code "));
        Serial.println(haltCode);

        // blink `haltCode` times, pause, repeat forever
        for (uint8_t i = 0; i < haltCode; i++) {
#ifdef LED_BUILTIN
            digitalWrite(LED_BUILTIN, HIGH);
            delay(150);
            digitalWrite(LED_BUILTIN, LOW);
            delay(250);
#else
            delay(400);
#endif
        }
        delay(1200);
    }
}
