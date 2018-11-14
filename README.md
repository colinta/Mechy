Mechy
-----

This is a mechanical keyboard library, in the same spirit as QMK but it doesn't share any code with that project.  Only supports the Atmega32u4 for now, but support for other microcontrollers was in mind.  As other boards become supported I'll make sure to list them here and include examples for them.

Mechy is designed around C++ `Plugin` classes, and included in the main bundle are a bunch of plugins.  I tried to make Mechy focused on being well designed, but it's also compact and, I think, more intuitive for new programmers than TMK/QMK.

Btw I'll make lots of comparisons to QMK here and I'll try not to disparage it.  It's a fantastic tool, and is responsible for the rise in interest in mechanical keyboards.  I have to make the comparisons because it's the only other keyboard firmware tool that I'm familiar with.  I found it a bit clunky and I couldn't get the split code to work on my BFO-9000, so I started this project.

Mechy is built using Arduino but this *might* change.  Arduino does unfortunate things in their USB/HID code (the TX/RXLED on/off "feature") that prevents it from working with some boards, like the XD75.  So I'd like to transition away from Arduino and rely instead on a Makefile/CLI.  I could use some help on that!  I've already ported all the pinMode/digitalRead/digitalWrite code (see `Wiring.h/cpp`) so that ALL the I/O ports/pins can be utilized, and I have a work-in-progress folder where I copied all the relevant Arduino libaries, I just need to get them to compile and linked.  I don't really know how to use `avr-gcc/avr-g++`.

See the examples first to see if this library appeals to you:

- [Numpad.ino](https://github.com/colinta/Mechy/blob/master/examples/Numpad/Numpad.ino) - simple 4x5 layout, uses a bunch of plugins
- [Layers.ino](https://github.com/colinta/Mechy/blob/master/examples/Layers/Layers.ino) - I copied and modified my XD75 layout, uses layers
- [LeftSplit.ino](https://github.com/colinta/Mechy/blob/master/examples/LeftSplit/LeftSplit.ino) - left side of a BFO9000 keyboard, uses split feature
- [RightSplit.ino](https://github.com/colinta/Mechy/blob/master/examples/RightSplit/RightSplit.ino) - right side of a BFO9000 keyboard

Usage
=====

Include the main library (`Mechy.h`) and scanner (`Mechy/Scanner.h`), include your plugins (`Mechy/Keypress.h`, `Mechy/MediaKey.h`), define the pins (you can copy the examples, they are all written for the Atmega32u4).  Then define your layout, create a few objects, and add `mechy.begin()` and `mechy.tick()` to `setup()` and `loop()`.  Easy! (I think.)

API
===

###### Mechy

The heart of it all!  Create a Mechy instance, `add()` your plugins, `attach()` your responders, and that should do it!

```c++
Mechy mechy = Mechy();
// create scanner
// create plugins

void setup() {
  mechy.add(plugin);

  mechy.attach(scanner);  // and attach(receiver) if you have a split keyboard

  mechy.begin();
}

void loop() {
  mechy.tick();
}
```

###### Keypress

The most basic plugin, sends any printable key press and basic modifiers.

```c++
#include <Mechy/Keypress.h>

KBD mainKeys[] = { KC_ESC, KC_A, KC_LSFT };

Mechy mechy = Mechy();
Keypress keypress = Keypress();
mechy.add(&keypress)

// or if you prefer:
Mechy mechy = Mechy();
mechy.add(new Keypress());
```

See [Keypress.h](https://github.com/colinta/Mechy/blob/master/src/Mechy/Keypress.h#L17) for defined keys.

###### MediaKey

Send play, pause, volume, track change keys.  I've only tested on macOS.

Btw this plugin uses the "ArduinoMedia" Library at https://github.com/colinta/ArduinoMedia, I maintain this library but it's just a copy of some code I found online.  I'm not entirely sure where I found it actually...

```c++
#include <Mechy/MediaKey.h>

KBD mainKeys[] = { MD_PLAY, MD_VOLU, MD_FFD };

Mechy mechy = Mechy();
mechy.add(new MediaKey());
```

See [Media.h](https://github.com/colinta/Mechy/blob/master/src/Mechy/Media.h#L15) for defined keys.

###### GotoLayer

Obviously it changes layers!  This is only the key listener part, to define multiple layers you'll need to study the `Layout` class.

I've only just finished this one, right now it only supports "hold for another layer", i.e. while the key is pressed it will activate another layer, when you release the key you'll bounce back to the previous layer.

Also I don't have much support for "transitive" keys.  If you use the `vvvv` macro you can cascade up to the main/first layer, but that's all.  In the future I'd like to change this to support lots of layer pushing/popping features.

```c++
#include <Mechy/GotoLayer.h>

KBD mainKeys[] = { GOTO_1, GOTO_2, GOTO_3 };

Mechy mechy = Mechy();
mechy.add(new GotoLayer());
```

See [GotoLayer.h](https://github.com/colinta/Mechy/blob/master/src/Mechy/GotoLayer.h#16) for defined keys.

###### Lock

A super simple Plugin, I wrote it as an early exercise.  This plugin listens for two key presses (`LK_1` and `LK_2`), and after both are pressed it locks all keys except `LK_1` and `LK_2`.  You can prevent a small child from using your keyboard - which is the actual reason I wrote this, because I have a precosious daughter who loves to play with my keyboard.

```c++
#include <Mechy/Lock.h>

KBD mainKeys[] = { LK_1, LK_2 };

Mechy mechy = Mechy();
mechy.add(new Lock());
```

See [Lock.h](https://github.com/colinta/Mechy/blob/master/src/Mechy/Lock.h#L15) for defined keys (it's only those two, so don't get excited).

###### Macro

This is only for password macros at the moment, but I'd like to add support for key macros like `GUI+SHIFT+4` (screenshot on macOS), etc.

```c++
#include <Mechy/Macro.h>

KBD mainKeys[] = { MM_0, MM_1, MM_2 };

// If this is a "private" keyboard you can hardcode them, otherwise look at
// Layers.ino for how I put them in a secrets file.  They'll still be visible to
// someone who gets your keyboard and knows what to look for.
const char* macros[7] = {"", "", "", "", "", "", ""};

Mechy mechy = Mechy();
mechy.add(new Macro(7, macros));
```

See [Macro.h](https://github.com/colinta/Mechy/blob/master/src/Mechy/Macro.h#L21) for defined keys.

###### Sticky

Good luck with this one, it's my own very opinionated version of how "sticky modifier keys" should work.

1. Press the modifier key once to activate it, then press any key to send "mod+key" and the modifier will be deactivated afterwards.
2. Press and hold the modifier and it will just work like a normal modifier.  This makes it easy to Command+click or to decide "nevermind I *don't* want to activate the modifier".
3. Double tap the modifier to lock it, press it again to unlock it.

###### Layout

This is the class you need to use if you want layers.  It's easy:

```c++
KBD mainKeys[] = { KC_A, GOTO_1 };
KBD fnKeys[] = { KC_B , ____ };

Layout layout = Layout(ROWS, COLS, mainKeys, fnKeys);
```

The `layout` can then be assigned to a `Scanner` or `Receiver` (for split keyboards).  A cool feature that QMK doesn't have - when you press a key in Mechy that key is stored until it is released, based on the layout, row, and column position.  So the `GOTO_1` key will send a key down AND key up even though it's not defined in `fnKeys`.  Also, if you pressed `KC_A` before pressing `GOTO_1`, then released `KC_A` while `GOTO_1` is still held, you won't have any bugs, and `KC_B` would become "available".  I can't explain it well, but this feature avoids a lot of the "layer code hell" that QMK can run into sometimes.


###### Scanner

This class handles the hardware reading/writing, and sends updates to `Mechy`, which is responsible for debouncing and storing those key presses.  You assign a `Layout` or a keyboard array directly (a `Layout` will be created for you, useful for one layer keyboards).

Implementation detail: `Scanner` is a `Responder`, which means it responds to key presses and emits `KBD*` events to `Mechy`, which stores the events and translates them to `Event*` objects, which are consumed by `Plugin` instances.

```c++
KBD keys[] = { ... };
Scanner scanner = Scanner(keys, pinRows, pinCols, ROWS, COLS);
mechy.attach(scanner);

// multiple layers
KBD mainKeys[] = { ... };
KBD fnKeys[] = { ... };
Layout layout = Layout(ROWS, COLS, mainKeys, fnKeys);
Scanner scanner = Scanner(&layout, pinRows, pinCols, ROWS, COLS);

mechy.attach(scanner);
```

###### Receiver

`Receiver` is similar in usage to `Scanner` (it is also a "Responder" subclass, and so can emit key events).  It accepts a `Layout` object and emits events.

Fun history fact, this split keyboard code is the original reason I wrote Mechy.  I couldn't get the split keyboard code to work using QMK, the resulting keyboard was buggy and unresponsive.

This class listens uses a custom-written 2-wire serial protocol (I tried to use I2C but it didn't work?), with support for up to 32 rows and 32 columns.  The other half uses `Transmitter` and a very basic `.ino` program.


###### Transmitter

This is the "right half" of the split code.  It implements the sending side of the 2-wire protocol.

###### Wiring

This is a re-implementation of Arduino's `wiring.c` code, but with support for all the pins on the Atmega32u4.  It's used internally, and I recommend you use the pins as they are defined in `Constants.h`, but they are compatible with the "digital pin numbers" that Arduino uses.  I.e. Arduino refers to PB0 as "11" in `pinMode` and `digitalRead/Write`, so the value for _B0 is also 11.  The values don't have any other meaning, they just need to be unique.

```c
#define _B0 11
#define _B1 15
#define _B2 16
#define _B3 14
#define _B4  8
#define _B5  9
#define _B6 10
#define _B7 12

#define _C6  5
#define _C7 13

#define _D0  3
#define _D1  2
#define _D2  0
#define _D3  1
#define _D4  4
#define _D5 17
#define _D6 22
#define _D7  6

#define _E2 23
#define _E6  7

#define _F0 24
#define _F1 25
#define _F4 21
#define _F5 20
#define _F6 19
#define _F7 18
```

(the numbers refer to the pin location on the Atmega32u4 chip, but the values don't matter as long as they're unique).

###### FYI

Mechy: pronounced "*Mech Key*" (or I suppose it sounds like "Meh-key", too), if that wasn't obvious.
