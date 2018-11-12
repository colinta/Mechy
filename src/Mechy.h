#pragma once

#include "priv/Constants.h"
#include "priv/Event.h"
#include "Mechy/Responder.h"
#include "Mechy/Plugin.h"
#include "Mechy/Layout.h"


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


struct KBDDataPtr {
    Layout* layout;
    KBD* kbd;
    uint8_t row;
    uint8_t col;
    bool isPressed;
    unsigned long started;
    KBDDataPtr* next;

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

    void sendKeyboardPress(uint8_t k);
    void sendKeyboardRelease(uint8_t k);

    void pushLayer(uint8_t layer);
    void popLayer();

protected:
    Event event;
    LayerStackPtr* layerStackPtr;
    ResponderPtr* firstResponderPtr;
    PluginPtr* firstPluginPtr;
    KBDDataPtr* firstKBDPtr;
    uint16_t modifiers;

    void runPlugin(uint8_t keyState, KBDDataPtr* kbdData, uint16_t duration);
    void updateLayer(uint8_t layer);

private:
    inline void appendPluginPtr(PluginPtr* ptr);
    inline void pushResponderPtr(ResponderPtr* ptr);
    inline void pushKBDPtr(KBDDataPtr* ptr);
    inline KBDDataPtr* removeKBDPtr(KBDDataPtr* ptr);
};
