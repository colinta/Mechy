#pragma once

#define _B0  8
#define _B1  9
#define _B2 10
#define _B3 11
#define _B4 28
#define _B5 29
#define _B6 30
#define _B7 12

#define _C6 31
#define _C7 32

#define _D0 18
#define _D1 19
#define _D2 20
#define _D3 21
#define _D4 25
#define _D5 22
#define _D6 26
#define _D7 27

#define _E2 33
#define _E6  1

#define _F0 41
#define _F1 40
#define _F4 39
#define _F5 38
#define _F6 37
#define _F7 36


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

#define FN_NONE     0
#define FN_KEYPRESS 1
#define FN_MEDIA    2
#define FN_STICKY   3
#define FN_LOCK     4
#define FN_MACRO    5
#define FN_USER_0    128

#define MCHY_NONE     0
#define MCHY_TRANS     0

#define MCHY_MASK_L_SHIFT 0b0000000000000011
#define MCHY_BITL_L_SHIFT                  0
#define MCHY_MASK_R_SHIFT 0b0000000000001100
#define MCHY_BITL_R_SHIFT                2
#define MCHY_MASK_L_CTRL  0b0000000000110000
#define MCHY_BITL_L_CTRL               4
#define MCHY_MASK_R_CTRL  0b0000000011000000
#define MCHY_BITL_R_CTRL             6
#define MCHY_MASK_L_ALT   0b0000001100000000
#define MCHY_BITL_L_ALT            8
#define MCHY_MASK_R_ALT   0b0000110000000000
#define MCHY_BITL_R_ALT         10
#define MCHY_MASK_L_GUI   0b0011000000000000
#define MCHY_BITL_L_GUI       12
#define MCHY_MASK_R_GUI   0b1100000000000000
#define MCHY_BITL_R_GUI     14

#define FN_USER(n)  (FN_USER_0 + n)
#define USER_KEY(m) m
#define USER(n, m) { .name = FN_USER(n), .key = USER_KEY(m) }
