#pragma once

#include "priv/Constants.h"
#include "priv/Event.h"
#include "Mechy/Responder.h"
#include "Mechy/Plugin.h"
#include "Mechy/Layout.h"
#include "Mechy/Scanner.h"

#define DEBOUNCE 15

// Fixed capacities for runtime state.  These replace runtime heap use: once
// the keyboard boots, no allocation can fail.  Override with a build flag
// (e.g. -DMECHY_MAX_EVENTS=16) if a configuration needs more.

// maximum simultaneously active key events (pressed keys, plus keys released
// within the last DEBOUNCE ms)
#ifndef MECHY_MAX_EVENTS
#define MECHY_MAX_EVENTS 12
#endif

// maximum simultaneously pushed layers
#ifndef MECHY_MAX_LAYER_STACK
#define MECHY_MAX_LAYER_STACK 8
#endif


struct PluginPtr {
    uint8_t name;
    Plugin* plugin;
    PluginPtr* next;
};


struct ResponderPtr {
    Responder* responder;
    ResponderPtr* next;
};



struct EventPtr {
    Layout* layout;
    uint8_t row;
    uint8_t col;
    Event* event;
    EventPtr* next;

    bool matches(Layout* layout, uint8_t row, uint8_t col);
};


class Mechy {
public:
    Mechy();
    void add(Plugin* plugin);
    void add(uint8_t name, Plugin* plugin);
    void attach(Responder* responder);
    void begin();
    void tick();
    void setListenFunc(void (*fnPtr)(Event*));

    bool processKeyEvent(Layout* layout, uint8_t row, uint8_t col, bool isPressed);
    Plugin* pluginFor(uint8_t name);
    void runEvent(Event* event);
    void finishEvent(Event* event);

    bool isCapsOn();

    void pressKey(uint8_t k);
    void pressModifier(uint16_t k);
    void releaseKey(uint8_t k);
    void releaseModifier(uint16_t k);

    uint16_t currentModifiers();
    void updateModifiers(uint16_t mchyModifiers);
    void clearModifiers();

    uint8_t defaultLayer();
    void setDefaultLayer(uint8_t layer);
    // returns false if the layer-stack allocation failed; the active layer
    // is unchanged in that case
    bool pushLayer(uint8_t layer);
    void popLayer();
    void removeLayer(uint8_t layer);
    void clearLayers();
    uint8_t currentLayer();

    EventPtr* events();

protected:
    uint8_t _defaultLayer;
    uint8_t layerStack[MECHY_MAX_LAYER_STACK];
    uint8_t layerStackSize;
    ResponderPtr* firstResponderPtr;
    PluginPtr* firstPluginPtr;
    EventPtr* firstEventPtr;
    uint16_t modifiers;
    void (*listenFnPtr)(Event*);

    bool capsIsOn;

    void updateLayer(uint8_t layer);

private:
    inline void pushPluginPtr(PluginPtr* ptr);
    inline void pushResponderPtr(ResponderPtr* ptr);
    inline void pushEventPtr(EventPtr* ptr);
    inline EventPtr* removeEventPtr(EventPtr* ptr);
};
