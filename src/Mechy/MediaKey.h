#pragma once

#include <Media.h>
#include "Mechy.h"

class MediaKey: public Plugin {
public:
    MediaKey();
    void process_event(Event *event);

protected:
};

#define MD(k) { .name = FN_MEDIA, .key = k, .isPressed = false, .started = 0 }
#define MD_VOLU MD(MEDIA_VOL_UP)
#define MD_VOLD MD(MEDIA_VOL_DOWN)
#define MD_PLAY MD(MEDIA_PLAY)
