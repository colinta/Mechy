#include "MediaKey.h"


MediaKey::MediaKey() {
}

void MediaKey::run(Event *event) {
    if (event->isPressed()) {
        Media.send(event->key);
    }
    else if (event->isReleased()) {
        Media.clear();
    }
}
