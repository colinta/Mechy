#include "DebugKey.h"


DebugKey::DebugKey() {
}

void DebugKey::begin() {
    Serial.begin(9600);
}

uint8_t DebugKey::defaultName() {
    return FN_DEBUG;
}

bool DebugKey::is(uint8_t event_type, Event* event) {
    return event->key() && event_type == EVENT_KEYPRESS;
}

void DebugKey::run(Event* event) {
    if (event->isPressed()) {
        Serial.print("\\dn  ");
    }
    else if (event->isReleased()) {
        Serial.print("/up  ");
    }
    else {
        return;
    }

    switch ((char)event->key()) {
    case '`':
        Serial.println("`~");
        break;
    case '-':
        Serial.println("-_");
        break;
    case '=':
        Serial.println("=+");
        break;
    case ';':
        Serial.println(";:");
        break;
    case '[':
        Serial.println("[{");
        break;
    case ']':
        Serial.println("]}");
        break;
    case '\'':
        Serial.println("\'\"");
        break;
    case '\\':
        Serial.println("\\|");
        break;
    case ',':
        Serial.println(",<");
        break;
    case '.':
        Serial.println(".>");
        break;
    case '/':
        Serial.println("/?");
        break;
    case ' ':
        Serial.println("Space");
        break;
    case 'A':
        Serial.println("F1");
        break;
    case 'B':
        Serial.println("F2");
        break;
    case 'C':
        Serial.println("F3");
        break;
    case 'D':
        Serial.println("F4");
        break;
    case 'E':
        Serial.println("F5");
        break;
    case 'F':
        Serial.println("F6");
        break;
    case 'G':
        Serial.println("F7");
        break;
    case 'H':
        Serial.println("F8");
        break;
    case 'I':
        Serial.println("F9");
        break;
    case 'J':
        Serial.println("F10");
        break;
    case 'K':
        Serial.println("F11");
        break;
    case 'L':
        Serial.println("F12");
        break;
    case 'M':
        Serial.println("Left");
        break;
    case 'N':
        Serial.println("Right");
        break;
    case 'O':
        Serial.println("Up");
        break;
    case 'P':
        Serial.println("Down");
        break;
    case 'Q':
        Serial.println("Pg Up");
        break;
    case 'R':
        Serial.println("Pg Dn");
        break;
    case 'S':
        Serial.println("Home");
        break;
    case 'T':
        Serial.println("End");
        break;
    case 'U':
        Serial.println("Shift");
        break;
    case 'V':
        Serial.println("Ctrl");
        break;
    case 'W':
        Serial.println("Alt");
        break;
    case 'X':
        Serial.println("Gui");
        break;
    case '!':
        Serial.println("Esc");
        break;
    case '?':
        Serial.println("Insert");
        break;
    case '>':
        Serial.println("Delete");
        break;
    case '<':
        Serial.println("Backspace");
        break;
    case '|':
        Serial.println("Enter");
        break;
    case ':':
        Serial.println("Tab");
        break;
    case '@':
        Serial.println("Caps Lock");
        break;
    case '#':
        Serial.println("FN 1");
        break;
    case '$':
        Serial.println("FN 2");
        break;
    case '%':
        Serial.println("FN 3");
        break;
    case '^':
        Serial.println("FN 4");
        break;
    case '&':
        Serial.println("FN 5");
        break;
    case '*':
        Serial.println("FN 6");
        break;
    case '(':
        Serial.println("FN 7");
        break;
    case ')':
        Serial.println("FN 8");
        break;
    case '_':
        Serial.println("C-A-G");
        break;
    case '+':
        Serial.println("PLAY");
        break;
    default:
        Serial.println((char)event->key());
        break;
    }
}
