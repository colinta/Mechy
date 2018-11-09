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

#define USER(n, m) { .name = FN_USER + n, .key = m }

#define USER_0(m) USER(0, m)
#define USER_1(m) USER(1, m)
#define USER_2(m) USER(2, m)
#define USER_3(m) USER(3, m)

#define USER_KEY(m) m
#define USER_KEY_0 USER_KEY(0)
#define USER_KEY_1 USER_KEY(1)
#define USER_KEY_2 USER_KEY(2)
#define USER_KEY_3 USER_KEY(3)
#define USER_KEY_4 USER_KEY(4)
#define USER_KEY_5 USER_KEY(5)
#define USER_KEY_6 USER_KEY(6)
#define USER_KEY_7 USER_KEY(7)
#define USER_KEY_8 USER_KEY(8)
#define USER_KEY_9 USER_KEY(9)

#define USER_0_0   USER(0, USER_KEY_0)
#define USER_0_1   USER(0, USER_KEY_1)
#define USER_0_2   USER(0, USER_KEY_2)
#define USER_0_3   USER(0, USER_KEY_3)
#define USER_0_4   USER(0, USER_KEY_4)
#define USER_0_5   USER(0, USER_KEY_5)
#define USER_0_6   USER(0, USER_KEY_6)
#define USER_0_7   USER(0, USER_KEY_7)
#define USER_0_8   USER(0, USER_KEY_8)
#define USER_0_9   USER(0, USER_KEY_9)

#define USER_1_0   USER(1, USER_KEY_0)
#define USER_1_1   USER(1, USER_KEY_1)
#define USER_1_2   USER(1, USER_KEY_2)
#define USER_1_3   USER(1, USER_KEY_3)
#define USER_1_4   USER(1, USER_KEY_4)
#define USER_1_5   USER(1, USER_KEY_5)
#define USER_1_6   USER(1, USER_KEY_6)
#define USER_1_7   USER(1, USER_KEY_7)
#define USER_1_8   USER(1, USER_KEY_8)
#define USER_1_9   USER(1, USER_KEY_9)

#define USER_2_0   USER(2, USER_KEY_0)
#define USER_2_1   USER(2, USER_KEY_1)
#define USER_2_2   USER(2, USER_KEY_2)
#define USER_2_3   USER(2, USER_KEY_3)
#define USER_2_4   USER(2, USER_KEY_4)
#define USER_2_5   USER(2, USER_KEY_5)
#define USER_2_6   USER(2, USER_KEY_6)
#define USER_2_7   USER(2, USER_KEY_7)
#define USER_2_8   USER(2, USER_KEY_8)
#define USER_2_9   USER(2, USER_KEY_9)

#define USER_3_0   USER(3, USER_KEY_0)
#define USER_3_1   USER(3, USER_KEY_1)
#define USER_3_2   USER(3, USER_KEY_2)
#define USER_3_3   USER(3, USER_KEY_3)
#define USER_3_4   USER(3, USER_KEY_4)
#define USER_3_5   USER(3, USER_KEY_5)
#define USER_3_6   USER(3, USER_KEY_6)
#define USER_3_7   USER(3, USER_KEY_7)
#define USER_3_8   USER(3, USER_KEY_8)
#define USER_3_9   USER(3, USER_KEY_9)
