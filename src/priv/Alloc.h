#pragma once

#include <Arduino.h>
#include <stdlib.h>

// Halt codes, one per subsystem.  mechyHalt() reports the code over serial
// and as an LED blink count.
#define MECHY_HALT_RESPONDER   1
#define MECHY_HALT_PLUGIN      2
#define MECHY_HALT_LAYOUT      3
#define MECHY_HALT_TAPHOLD     4
#define MECHY_HALT_COMBOKEY    5
#define MECHY_HALT_TRANSMITTER 6
#define MECHY_HALT_SENDSTRING  7

// Runtime allocations (key events, layer-stack entries, receiver key
// tracking) may return NULL; callers must degrade gracefully.
void* mechyAlloc(size_t size);
void mechyFree(void* ptr);

// Startup allocations (layouts, registration, plugin configuration) either
// succeed or halt.  Returns non-null or never returns.
void* mechyAllocOrHalt(size_t size, uint8_t haltCode);

// Deliberate fail-fast halt.  On AVR a null-pointer write does not trap - it
// silently corrupts the register file - so "crashing" must be explicit.  The
// halt loop initializes Serial itself (startup allocations can fail in global
// constructors, before setup() runs) and leaves interrupts enabled so the USB
// stack stays alive: the serial monitor can attach after the fact and the
// board can still be re-flashed with the normal upload flow.
void mechyHalt(uint8_t haltCode) __attribute__((noreturn));

// Tagged placement-new, used to construct a Layout in memory obtained from
// mechyAllocOrHalt.  The global `new` on the AVR core is an unchecked malloc
// wrapper: the constructor runs even when malloc returns NULL.  A distinct
// tag type avoids any collision with a core-provided placement new.
struct MechyPlacement {};
inline void* operator new(size_t, MechyPlacement, void* ptr) { return ptr; }
