#pragma once

#include "priv/Constants.h"
#include "priv/Event.h"
#include "Mechy/Responder.h"
#include "Mechy/Plugin.h"
#include "Mechy/Layout.h"
#include "Mechy/Scanner.h"

#define DEBOUNCE 10


struct PluginPtr {
    uint8_t name;
    Plugin* plugin;
    PluginPtr* next;
};


struct ResponderPtr {
    Responder* responder;
    ResponderPtr* next;
};


struct LayerStackPtr {
    uint8_t value;
    LayerStackPtr* prev;
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
    void runPlugin(Event* event);
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
    void pushLayer(uint8_t layer);
    void popLayer();
    void removeLayer(uint8_t layer);
    uint8_t currentLayer();

    EventPtr* events();

protected:
    uint8_t _defaultLayer;
    LayerStackPtr* layerStackPtr;
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
