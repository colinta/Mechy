#pragma once

#include "Mechy.h"

class MediaKey: public Plugin {
public:
    MediaKey();
    void process_event(Event *event);

protected:
};

#define MD_VOLU { .name = FN_MEDIA, .key = MEDIA_VOL_UP, .isPressed = false, .started = 0 }
#define MD_VOLD { .name = FN_MEDIA, .key = MEDIA_VOL_DOWN, .isPressed = false, .started = 0 }
#define MD_PLAY { .name = FN_MEDIA, .key = MEDIA_PLAY, .isPressed = false, .started = 0 }
