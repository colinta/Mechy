#pragma once

#define _B0 11
#define _B1 15
#define _B2 16
#define _B3 14
#define _B4  8
#define _B5  9
#define _B6 10
#define _B7 12

#define _C6  5
#define _C7 13

#define _D0  3
#define _D1  2
#define _D2  0
#define _D3  1
#define _D4  4
#define _D5 17
#define _D6 22
#define _D7  6

#define _E2 23
#define _E6  7

#define _F0 24
#define _F1 25
#define _F4 21
#define _F5 20
#define _F6 19
#define _F7 18


#define bit_get(p, m) ((p) & (m))
#define bit_on(p, m) ((p) |= (m))
#define bit_off(p, m) ((p) &= ~(m))

#define UNUSED(x) x __attribute__ ((unused))

#define KEY_STATE_NONE 0
#define KEY_STATE_PRESSED 1
#define KEY_STATE_HELD 2
#define KEY_STATE_RELEASED 3

#define KBD_HALT false
#define KBD_CONTINUE true

#define FN_NONE       0
#define FN_KEYPRESS   1
#define FN_MEDIA      2
#define FN_STICKY     3
#define FN_LOCK       4
#define FN_MACRO      5
#define FN_GOTO_LAYER 6
#define FN_TAP_HOLD   7
#define FN_USER_0    128

#define MCHY_NONE     0
#define MCHY_TRANS    1

/* EVENT TYPES */
// typical keys (F1, A, ESC, DELETE), except modifiers
#define EVENT_KEYPRESS 1
// Shift, Control, Alt, Gui
#define EVENT_MODIFIER 2
// Mechy internals: Lock, etc
#define EVENT_META 3

/* EVENT key masks */
// in general, events have 8bits of "data" and 8bits of "key space"
#define EVENT_KEY_MASK  0b0000000011111111
#define EVENT_PLUG_MASK 0b1111111100000000
// to encourage "best practices" here are some bit masks:
#define EVENT_IS_ACTIVE 0b0010000000000000
#define isEventActive(e) (e->key & EVENT_IS_ACTIVE)
#define onEventActive(e) (e->key |= EVENT_IS_ACTIVE)
#define offEventActive(e) (e->key &= ~(EVENT_IS_ACTIVE))

/* modifier masks that are used by Mechy */
#define MCHY_MASK_L_SHIFT 0b0000000000000011
#define MCHY_BITL_L_SHIFT                  0
#define MCHY_MASK_R_SHIFT 0b0000000000001100
#define MCHY_BITL_R_SHIFT                2
#define MCHY_MASK_SHIFT   0b0000000000001111

#define MCHY_MASK_L_CTRL  0b0000000000110000
#define MCHY_BITL_L_CTRL               4
#define MCHY_MASK_R_CTRL  0b0000000011000000
#define MCHY_BITL_R_CTRL             6
#define MCHY_MASK_CTRL    0b0000000011110000

#define MCHY_MASK_L_ALT   0b0000001100000000
#define MCHY_BITL_L_ALT            8
#define MCHY_MASK_R_ALT   0b0000110000000000
#define MCHY_BITL_R_ALT         10
#define MCHY_MASK_ALT     0b0000111100000000

#define MCHY_MASK_L_GUI   0b0011000000000000
#define MCHY_BITL_L_GUI       12
#define MCHY_MASK_R_GUI   0b1100000000000000
#define MCHY_BITL_R_GUI     14
#define MCHY_MASK_GUI     0b1111000000000000

/* user defined key macros */
#define FN_USER(n)  (FN_USER_0 + n)
#define USER_KEY(m) m
#define USER(n, m) { .name = FN_USER(n), .key = USER_KEY(m) }
