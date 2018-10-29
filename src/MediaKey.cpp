#include "MediaKey.h"


MediaKey::MediaKey() {
}

void MediaKey::process_event(Event *event) {
  if (event->isPressed()) {
    Media.send(event->key);
  }
  else if (event->isReleased()) {
    Media.clear();
  }
}
