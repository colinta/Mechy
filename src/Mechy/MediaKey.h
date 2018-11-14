#pragma once

#include <Media.h>
#include "Mechy.h"

class MediaKey: public Plugin {
public:
    MediaKey();
    uint8_t defaultName();
    void run(Event* event);

protected:
};

#define MD(k) { .name = FN_MEDIA, .key = k }
#define MD_VOLU  MD(MEDIA_VOL_UP)
#define MD_VOLD  MD(MEDIA_VOL_DOWN)
#define MD_PLAY  MD(MEDIA_PLAY)
#define MD_MUTE  MD(MEDIA_MUTE)
#define MD_PAUSE MD(MEDIA_PAUSE)
#define MD_STOP  MD(MEDIA_STOP)
#define MD_NEXT  MD(MEDIA_NEXT)
#define MD_PREV  MD(MEDIA_PREV)
#define MD_FFD   MD(MEDIA_FFD)
#define MD_RWD   MD(MEDIA_RWD)

// QMK compat:
#define KC_VOLU  MD_VOLU
#define KC_VOLD  MD_VOLD
#define KC_MPLY  MD_PLAY
#define KC_MUTE  MD_MUTE
#define KC_PAUSE MD_PAUSE
#define KC_STOP  MD_STOP
#define KC_MNXT  MD_NEXT
#define KC_PREV  MD_PREV
#define KC_FFD   MD_FFD
#define KC_RWD   MD_RWD
