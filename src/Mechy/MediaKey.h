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
#define MD_PAUSE MD(MEDIA_PAUSE)
#define MD_STOP  MD(MEDIA_STOP)
#define MD_NEXT  MD(MEDIA_NEXT)
#define MD_PREV  MD(MEDIA_PREV)
#define MD_FFD   MD(MEDIA_FFD)
#define MD_RWD   MD(MEDIA_RWD)
