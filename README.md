Mechy
-----

This is a mechanical keyboard library, in the same spirit as QMK but it doesn't share any code with that project.  Only supports the Atmega32u4 for now, but support for other microcontrollers was in mind.  As other boards become supported I'll make sure to list them here and include examples for them.

Mechy is designed around C++ `Plugin` classes, and included in the main bundle are a bunch of plugins.  I tried to make Mechy focused on being well designed, but it's also compact and, I think, more intuitive for new programmers than TMK/QMK.

Btw I'll make lots of comparisons to QMK here and I'll try not to disparage it.  It's a fantastic tool, and is responsible for the rise in interest in mechanical keyboards.  I have to make the comparisons because it's the only other keyboard firmware tool that I'm familiar with.  I found it a bit clunky and I couldn't get the split code to work on my BFO-9000, so I started this project.

Mechy is built using Arduino but this *might* change.  Arduino does unfortunate things in their USB/HID code (the TX/RXLED on/off "feature") that prevents it from working with some boards, like the XD75.  So I'd like to transition away from Arduino and rely instead on a Makefile/CLI.  I could use some help on that!  I've already ported all the pinMode/digitalRead/digitalWrite code (see `Wiring.h/cpp`) so that ALL the I/O ports/pins can be utilized, and I have a work-in-progress folder where I copied all the relevant Arduino libaries, I just need to get them to compile and linked.  I don't really know how to use `avr-gcc/avr-g++`.

See the examples first to see if this library appeals to you:

- [Basic.ino](https://github.com/colinta/Mechy/blob/master/examples/Basic/Basic.ino) - A basic Planck (rev1-5, using the Atmega32u4) with just one layer.  Doesn't use the `Hardware` class, to keep things simple.
- [Numpad.ino](https://github.com/colinta/Mechy/blob/master/examples/Numpad/Numpad.ino) - simple 4x5 layout, uses a bunch of plugins
- [Layers.ino](https://github.com/colinta/Mechy/blob/master/examples/Layers/Layers.ino) - I copied and modified my XD75 layout, uses layers and all my favorite plugins
- [Planck.ino](https://github.com/colinta/Mechy/blob/master/examples/Planck/Planck.ino) - A port of the Planck default layout, with layers and `Hardware` object.
- [DZ60.ino](https://github.com/colinta/Mechy/blob/master/examples/DZ60/DZ60.ino) - A port of the DZ60 default layout, but with unsupported features and macros removed - a good starting point for a new layout.
- [LeftSplit.ino](https://github.com/colinta/Mechy/blob/master/examples/LeftSplit/LeftSplit.ino) - left side of a BFO9000 keyboard, uses split feature
- [RightSplit.ino](https://github.com/colinta/Mechy/blob/master/examples/RightSplit/RightSplit.ino) - right side of a BFO9000 keyboard

Usage
=====

Include the main library (`Mechy.h`), include your plugins (`Mechy/KeyPress.h`, `Mechy/MediaKey.h`), define the pins for your keyboard (better yet use a `Hardware` header to do this).  Then define your layout (you should use `PROGMEM`), create a few objects, and add `mechy.begin()` and `mechy.tick()` to `setup()` and `loop()`.  Easy! (I think.)

If a keyboard is available (in [`Keyboards.h`](https://github.com/colinta/Mechy/blob/master/src/Mechy/Keyboards)) the process is even slightly easier: the pins will be defined for you, and you can create a `Hardware` instance.  This will make it easy to use keyboard features like LEDs and sound.  The `Hardware` author needs to write this class to support what that keyboard is capable of, or you can use it as a starting point and do your own thing!

The keyboard header will also define handy `LAYOUT` macros and a keyboard template that you can use.  The macro is nice because it can take into account staggering and keys that span multiple rows & columns.

API
===

### Mechy

The heart of it all!  Create a Mechy instance, `add()` your plugins, `attach()` your responders, and that should do it!

```cpp
Mechy mechy = Mechy();

// Layout and Mechy expect your keys to be in PROGMEM, use this macro to have
// your keys array defined correctly.
KEYS(keys) = LAYOUT(KC_A, KC_B, ..., KC_RSFT);

// Create a scanner with your keys array (or Layout).  This class will generate events and send them to Mechy.
Scanner scanner = Scanner(keys, pinRows, pinCols, ROWS, COLS);

void setup() {
    // add all your plugins
    mechy.add(new KeyPress());

    mechy.attach(scanner);  // and attach(receiver) if you have a split keyboard

    mechy.begin();
    mechy.setUpdateFunc(update)
}

void loop() {
    mechy.tick();
}

void update(Event* event) {
    // listen for all events here - press, hold, and release
}
```

### Plugins

In QMK you might be used to activating features in your `rules.mk` and `config.h` files.  In Mechy a lot of these same features are accomplished instead by using the appropriate plugin.  For instance, "AutoShift" is a plugin, not a feature you activate.  Once you include it you need to use *its* key commands, e.g. `AS_A` instead of `KC_A`.

###### KeyPress

The most basic plugin, sends any printable key press and modifiers, also supports HYPER (⌘⌥⌃⇧), MEH (⌥⌃⇧), and CAG (⌘⌥⌃) keys, and keyboard chords.

```cpp
#include <Mechy/KeyPress.h>

// define a custom "cmd+tab" key:
#define CMD_TAB KC(LGUI(KEY_TAB))
KEYS(mainKeys) = { KC_ESC, KC_A, CMD_TAB };
```

See [KeyPress.h](https://github.com/colinta/Mechy/blob/master/src/Mechy/KeyPress.h#L17) for defined keys.

###### MediaKey

Send play, pause, volume, track change keys.  I've only tested on macOS.

Btw this plugin uses the "ArduinoMedia" Library at https://github.com/colinta/ArduinoMedia. I maintain this library but it's just a copy of code I found in https://github.com/Nicohood/HID

```cpp
#include <Mechy/MediaKey.h>

KEYS(mainKeys) = { MD_PLAY, MD_VOLU, MD_FFD };
```

See [MediaKey.h](https://github.com/colinta/Mechy/blob/master/src/Mechy/MediaKey.h#L15) for defined keys.

###### MouseKey

A simple implementation of mouse keys.  Hold shift to move the mouse faster.

```cpp
#include <Mechy/MouseKey.h>

KEYS(mainKeys) = { MK_LEFT, MK_LCLK, MK_RGHT };
```

See [MouseKey.h](https://github.com/colinta/Mechy/blob/master/src/Mechy/MouseKey.h#L27) for defined keys.

###### AutoShift

A port of QMK's auto shift feature.  Tap a letter for lower case, hold it for the shifted key.  Also works for numbers and symbol keys.

This is an interesting plugin because it is very careful about when it reports that it is a keypress.  The mechanism for this is the function `bool Plugin::is(event_type, Event*)`.  This plugin function returns `true` when it is considered a match for the `event_type` (`EVENT_KEYPRESS, EVENT_MOUSE, EVENT_MODIFIER, EVENT_LOCK, EVENT_LAYER` are possible values).

The AutoShift key only matches at two times: after being held for 1/4 second, or at release when it is below that time.  At all other times this key is not doing anything; it doesn't send key repeat events.

See [AutoShift.h](https://github.com/colinta/Mechy/blob/master/src/Mechy/AutoShift.h#L15) for defined keys.

###### TapHold

Tap a key for one key, press and hold for another.  Much like `AutoShift` but works with any key commands.  Getting it setup is just a little more work, but it's great for limited-size keyboards to have keys like PageUp double as the Home key!

The default behavior is to send a "key tap", i.e. a down event followed immediately by an up event.  If you want to use the "hold" feature to activate modifiers or switch layers, you'll probably want to use the `TH_HOLD` behavior (though not necessarily - the GotoLayer `PUSH` and `LSET` behaviors work just fine with the "tap" behavior).

```cpp
#include <Mechy/TapHold.h>

// define human readable shortcuts for the layout
#define TH_PGUP TH_0
#define TH_PGDN TH_1
#define TH_SPC  TH(2)  // TH_0...TH_9 are defined, if you need more use TH(n)
KEYS(mainKeys) = { TH_PGUP, TH_PGDN, TH_SPC };

void setup() {
    TapHold* tapHold = new TapHold();
    // we need to tell TapHold what keys should be associated with these events.
    // Order matters here - the first call to `add` will be associated with
    // TH_0, and so on.
    tapHold->add(KC_PGUP, KC_HOME);
    tapHold->add(KC_PGDN, KC_END);
    // check this out, you can use this command to switch layers. tap for space,
    // or hold to activate layer 1.  Uses the `TH_HOLD` behavior to keep the
    // layer active as long as the key is held
    tapHold->add(KC_SPC, GOTO_1, TH_HOLD);
    // these must be called before mechy.begin()!  They get copied over as an
    // array in TapHold::begin()

    mechy.add(tapHold);
    mechy.begin();
}

```

###### GotoLayer

Obviously it changes layers!  This is only the key listener part, to define multiple layers you'll need to study the `Layout` class.

There are a few layer changing "modes".  The simplest is `GOTO`, which momentarily activates another layer while the key is held.  When you release the key you'll bounce back to the previous layer.

Another option is `LSET` or layer set.  This is used to activate another layer and keep it active.  A good use case for this is to activate Colemak or Dvorak or Qwerty layouts.  The "set layout" key could be on your function layer, and using `LSET` there you would in effect swap out the layer *below* the function layer.  See below for this trick.  Btw this does not exactly "push" the layer on, it activates the layer while the key is held and assigns it as the default, when no other layers are active.

My favorite, though, is `PUSH`, which takes the best of both these behaviors.  It activates another layer while the key is held, and then you have an option: release the `PUSH` key and that layer will remain the active layer (which makes it similar to `LSET`).  Or if you keep it held and press some keys, when you release the `PUSH` key you'll return to the previous layer (which makes it similar to `GOTO`).  This is called "one shot layer" in QMK speak, though I don't know whether QMK's `OSL()` macro works the same way or not.  If you use this macro be sure to include the `BACK` key (or other layer changing keys) because otherwise you won't be able to return to the base layer.

Lastly there is the `BACK` behavior, this just pops the top-most layer off the stack of activated layers.  Don't expect miracles if you have a dozen layers defined and want this feature to "just work".

There is limited support for "transparent" keys.  This is deliberate, it keeps the "what keys are active?" logic simple.  If you use the `vvvv` macro you can cascade up *to the zero'th layer*, but that's all.  If you use `LSET` to activate another layer, it will still cascade up to the zero'th layer, not the "previous" layer.  In the future this might change to support cascading to multiple layers, but that will probably be an option and not the default behavior.

```cpp
#include <Mechy/GotoLayer.h>

KEYS(mainKeys) = { GOTO_1, PUSH_2, LSET_3 };
KEYS(fnKeys) = { LOWER , RAISE , BACK };  // LOWER and RAISE use the PUSH behavior, btw.

// here's an example of supporting colemak/qwerty/dvorak layers
#define LSET_QW LSET_0
#define LSET_CM LSET_1
#define LSET_DK LSET_2
#define GOTO_FN GOTO_3
// remember that this first layer is always used for cascading, regardless of which layer is active.
KEYS(mainKeys)    = { KC_LCTL,  KC_Q  ,  KC_W  ,  KC_E  ,  KC_R  ,  KC_T  ,  KC_Y  , GOTO_FN };
KEYS(colemakKeys) = {  vvvv  ,  KC_Q  ,  KC_W  ,  KC_F  ,  KC_P  ,  KC_G  ,  KC_J  ,  vvvv   };
KEYS(dvorakKeys)  = {  vvvv  , KC_QUOT, KC_COMM, KC_DOT ,  KC_P  ,  KC_Y  ,  KC_F  ,  vvvv   };
KEYS(fnKeys)      = { KC_LCTL, LSET_QW, LSET_CM, LSET_DK,  ____  ,  ____  ,  ____  , GOTO_FN };
Layout(ROWS, COLS, mainKeys, colemakKeys, dvorakKeys, fnKeys);
```

See [GotoLayer.h](https://github.com/colinta/Mechy/blob/master/src/Mechy/GotoLayer.h#L23) for defined keys.

###### Macro

This is only for password macros at the moment. For keyboard chords like `CMD+SHIFT+4` you can use KeyPress (`LGUI(LSFT('4'))`).

```cpp
#include <Mechy/Macro.h>

KEYS(mainKeys) = { MM_0, MM_1, MM_2 };

// If this is a "private" keyboard you can hardcode them, otherwise look at
// Layers.ino for how I put them in a secrets file.  They'll still be visible to
// someone who gets your keyboard and knows what to look for.
const char* macros[7] = {"", "", "", "", "", "", ""};

Mechy mechy = Mechy();
mechy.add(new Macro(7, macros));
```

See [Macro.h](https://github.com/colinta/Mechy/blob/master/src/Mechy/Macro.h#L17) for defined keys.

###### Sticky

This is my own very opinionated version of how "sticky modifier keys" should work.  The same spirit as "One shot" keys in QMK and "Sticky keys" on Windows and macOS.  The difference is that my version of sticky keys deactivates the "sticky" key if you hold the modifier key.

1. Press the modifier key once to activate it, then press any key to send "mod+key".  The modifier will be deactivated afterwards.
2. Press and hold the modifier (for 1/4 sec or more) and it will behave like a normal modifier.  This makes it easy to Command+click or to decide "nevermind I *don't* want to activate the modifier".
3. Double tap the modifier to lock it, press it again to unlock it.

Also supports HYPER (⌘⌥⌃⇧), MEH (⌥⌃⇧), and CAG (⌘⌥⌃) keys.

```cpp
#include <Mechy/Sticky.h>

KEYS(mainKeys) = { ST_CTL, ST_ALT, ST_SFT, ST_GUI, ST_HYP, ST_MEH, ST_CAG };
```

The example above shows all the defined keys.

###### ComboKey

This plugin listens for multiple key down events and sends the 'combo' key when they are all pressed.  Currently only supports one combination, in the future this could be refactored to support multiple combinations.

The key events that it listens to are still activated, so these can either perform double duty (like modifier keys), or just assign `____` to have them ignored.

```cpp
#include <Mechy/ComboKey.h>

#define CK_LCTL CK(0)
#define CK_LALT CK(1)
#define CK_BSPC CK(2)

KEYS(mainKeys) = { CK_LALT, CK_LALT, CK_BSPC };

// map 'ctrl+alt+backspace' to 'goto layer 1'
KBD comboKeys[3] = { KC_LCTL, KC_LALT, KC_BSPC };
ComboKey comboKey = ComboKey(3, comboKeys, GOTO_1);
mechy.add(&comboKey);
```

###### Lock

A super simple Plugin, I wrote it as an early exercise.  This plugin listens for two `LK` key presses (or more - the `Lock()` constructor accepts the number of keys needed), and after both are pressed it locks all keys except internal "meta" keys like `GOTO_x` and `LK`.  You can prevent a small child from using your keyboard - which is the actual reason I wrote this, because I have a precosious daughter who loves to play with my keyboard.  Actually now that I added the `Notes` I let her play with that feature.

```cpp
#include <Mechy/Lock.h>

KEYS(mainKeys) = { LK, LK };
mechy.add(new Lock());
// to require 3 lock keys:
mechy.add(new Lock(3));
```

Only the `LK` key is defined.

###### Notes

For funsies!  If you have a piezo speaker attached to a pin you can use this plugin to play music.

```cpp
#include <Mechy/Notes.h>

KEYS(mainKeys) = { NT_C3, NT_D3, NT_E3, NT_F3, NT_G3, NT_A3, NT_B3, NT_C4 }
```

The header file is big - it includes keys for every note and an easter egg song key. [Notes.h](https://github.com/colinta/Mechy/blob/master/src/Mechy/Notes.h#L121)

### Support classes

###### Layout

This is the class you need to use if you want layers.  It's easy:

```cpp
KEYS(mainKeys) = { KC_A, GOTO_1 };
KEYS(fnKeys) = { KC_B , ____ };

Layout layout = Layout(ROWS, COLS, mainKeys, fnKeys);
```

The `layout` can then be assigned to a `Scanner` or `Receiver` (for split keyboards).  A cool feature that QMK doesn't have - when you press a key in Mechy that key is stored until it is released, based on the layout, row, and column position.  So the `GOTO_1` key will send a key down AND key up even though it's not defined in `fnKeys`.  Also, if you pressed `KC_A` before pressing `GOTO_1`, then released `KC_A` while `GOTO_1` is still held, you won't have any bugs, and `KC_B` would become "available".  I can't explain it well, but this feature avoids a lot of the "layer code hell" that QMK can run into sometimes.


###### Scanner

This class handles the hardware reading/writing, and sends updates to `Mechy`, which is responsible for debouncing and storing those key presses.  You assign a `Layout` or a keyboard array directly (a `Layout` will be created for you, useful for one layer keyboards).

Implementation detail: `Scanner` is a `Responder`, which means it responds to key presses and emits `KBD*` events to `Mechy`, which stores the events and translates them to `Event*` objects, which are consumed by `Plugin` instances.

```cpp
KEYS(keys) = { ... };
Scanner scanner = Scanner(keys, pinRows, pinCols, ROWS, COLS);
mechy.attach(scanner);

// multiple layers
KEYS(mainKeys) = { ... };
KEYS(fnKeys) = { ... };
Layout layout = Layout(ROWS, COLS, mainKeys, fnKeys);
Scanner scanner = Scanner(&layout, pinRows, pinCols, ROWS, COLS);

mechy.attach(scanner);
```

###### Receiver

`Receiver` is similar in usage to `Scanner` (it is also a "Responder" subclass, and so can emit key events).  It accepts a `Layout` object and emits events that it receives from the "right half" of a split keyboard.

This class listens uses a custom-written 2-wire serial protocol (I tried to use I2C but it didn't work?), with support for up to 32 rows and 32 columns.  The other half uses `Transmitter` and a very basic `.ino` program.

Fun history fact, this split keyboard code is the original reason I wrote Mechy.  I couldn't get the split keyboard code to perform well using QMK, the resulting keyboard was buggy and unresponsive.

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

###### Hardware

This class is defined per-keyboard, but in general it will have:

- `LAYOUT` macro and maybe alternates like `LAYOUT_standard` and `LAYOUT_60`.
- `begin()` to initialize LED or other pins
- `tick()` to check `Mechy` for Caps lock status and update that LED automatically (if you assign to `capsLockLedWrite()` your setting will override the default).
- Definitions for ROWS, COLS, pinRows and pinCols.
- LED controlling methods like `capsLockLedWrite()` and `capsLockLedRead()` and defines like `CAPS_LOCK_LED_PIN`
- RGB and backlight support, if available.

# FAQ

Mechy is pronounced "*Mech Key*" (or I suppose it sounds like "Meh-key", too).

I would've called the `Hardware` class `Keyboard`, but that name was taken.

Technical documentation: yeah probably some day, but only if someone asks for it and wants to contribute.
