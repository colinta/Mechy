#pragma once

#include "priv/Constants.h"
#include "priv/Event.h"
#include "Mechy/Responder.h"
#include "Mechy/Plugin.h"
#include "Mechy/Layout.h"
#include "Mechy/Scanner.h"


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
    KBD* kbd;
    uint8_t row;
    uint8_t col;
    bool isPressed;
    unsigned long started;
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

    void processKeyEvent(Layout* layout, uint8_t row, uint8_t col, bool isPressed);
    void runPlugin(uint8_t keyState, KBD* kbd, uint16_t duration);

    bool isCapsOn();

    void pressKey(uint8_t k);
    void pressModifier(uint16_t k);
    void releaseKey(uint8_t k);
    void releaseModifier(uint16_t k);

    uint16_t currentModifiers();
    void updateModifiers(uint16_t mchyModifiers);
    void clearModifiers();

    void pushLayer(uint8_t layer);
    void popLayer();
    uint8_t currentLayer();

    EventPtr* events();

protected:
    LayerStackPtr* layerStackPtr;
    ResponderPtr* firstResponderPtr;
    PluginPtr* firstPluginPtr;
    EventPtr* firstEventPtr;
    uint16_t modifiers;

    bool capsIsOn;

    void updateLayer(uint8_t layer);

private:
    inline void pushPluginPtr(PluginPtr* ptr);
    inline void pushResponderPtr(ResponderPtr* ptr);
    inline void pushKBDPtr(EventPtr* ptr);
    inline EventPtr* removeKBDPtr(EventPtr* ptr);
};
