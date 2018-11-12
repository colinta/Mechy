#include "MediaKey.h"


MediaKey::MediaKey() {
}

uint8_t MediaKey::defaultName() {
    return FN_MEDIA;
}

void MediaKey::run(Event* event) {
    if (event->isPressed()) {
        Media.send(event->key);
    }
    else if (event->isReleased()) {
        Media.clear();
    }
}
