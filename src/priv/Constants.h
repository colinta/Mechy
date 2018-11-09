#pragma once

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
#define FN_USER    128

#define MCHY_NONE     0

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
