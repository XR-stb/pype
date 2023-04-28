#pragma once

#include <map>

inline static const char* kVirtualKeyCodes[] = {
    "Undefined",    // 0
    "Alpha0",       // 1
    "Alpha1",       // 2
    "Alpha2",       // 3
    "Alpha3",       // 4
    "Alpha4",       // 5
    "Alpha5",       // 6
    "Alpha6",       // 7
    "Alpha7",       // 8
    "Alpha8",       // 9
    "Alpha9",       // 10

    "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", // 11 - 20
    "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", // 21 - 30
    "U", "V", "W", "X", "Y", "Z",                    // 31 - 36

    "F1", "F2", "F3", "F4", "F5", "F6",               // 37 - 42
    "F7", "F8", "F9", "F10", "F11", "F12",            // 43 - 48
    "F13", "F14", "F15", "F16", "F17", "F18",         // 49 - 54
    "F19", "F20", "F21", "F22", "F23", "F24",         // 55 - 60

    "Keypad0",      // 61
    "Keypad1",      // 62
    "Keypad2",      // 63
    "Keypad3",      // 64
    "Keypad4",      // 65
    "Keypad5",      // 66
    "Keypad6",      // 67
    "Keypad7",      // 68
    "Keypad8",      // 69
    "Keypad9",      // 70

    "KeypadClear",  // 71
    "KeypadDecimal",// 72
    "KeypadDivide", // 73
    "KeypadEnter",  // 74
    "KeypadEquals", // 75
    "KeypadMinus",  // 76
    "KeypadMultiply",//77
    "KeypadPlus",   // 78

    "Backslash",    // 79
    "Comma",        // 80
    "Equals",       // 81
    "Grave",        // 82
    "LeftBracket",  // 83
    "Minus",        // 84
    "Period",       // 85
    "Quote",        // 86
    "RightBracket", // 87
    "Semicolon",    // 88
    "Slash",        // 89

    "Meta",         // 90
    "Ctrl",         // 91
    "Function",     // 92
    "Alt",          // 93
    "Shift",        // 94
    "RightMeta",    // 95
    "RightCtrl",    // 96
    "RightAlt",     // 97
    "RightShift",   // 98
    "CapsLock",     // 99
    "NumLock",      // 100
    "ScrollLock",   // 101

    "DownArrow",    // 102
    "LeftArrow",    // 103
    "RightArrow",   // 104
    "UpArrow",      // 105

    "End",          // 106
    "Home",         // 107
    "PageDown",     // 108
    "PageUp",       // 109

    "ContextMenu",  // 110
    "Backspace",    // 111
    "Escape",       // 112
    "Delete",       // 113
    "Help",         // 114
    "Mute",         // 115
    "PrintScreen",  // 116
    "Enter",        // 117
    "Space",        // 118
    "Tab",          // 119
    "VolumeDown",   // 120
    "VolumeUp",     // 121
    "Pause",        // 122
    "Insert",       // 123
    "Sleep",        // 124
};

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

const int _native_key_mapping[] = {
    0,

    // numbers
    0x30,
    0x31,
    0x32,
    0x33,
    0x34,
    0x35,
    0x36,
    0x37,
    0x38,
    0x39,

    // letters
    0x41,
    0x42,
    0x43,
    0x44,
    0x45,
    0x46,
    0x47,
    0x48,
    0x49,
    0x4A,
    0x4B,
    0x4C,
    0x4D,
    0x4E,
    0x4F,
    0x50,
    0x51,
    0x52,
    0x53,
    0x54,
    0x55,
    0x56,
    0x57,
    0x58,
    0x59,
    0x5A,

    // function
    VK_F1,
    VK_F2,
    VK_F3,
    VK_F4,
    VK_F5,
    VK_F6,
    VK_F7,
    VK_F8,
    VK_F9,
    VK_F10,
    VK_F11,
    VK_F12,
    VK_F13,
    VK_F14,
    VK_F15,
    VK_F16,
    VK_F17,
    VK_F18,
    VK_F19,
    VK_F20,
    VK_F21,
    VK_F22,
    VK_F23,
    VK_F24,

    // keypad numbers
    VK_NUMPAD0,
    VK_NUMPAD1,
    VK_NUMPAD2,
    VK_NUMPAD3,
    VK_NUMPAD4,
    VK_NUMPAD5,
    VK_NUMPAD6,
    VK_NUMPAD7,
    VK_NUMPAD8,
    VK_NUMPAD9,

    // keypad other
    VK_CLEAR,
    VK_DECIMAL,
    VK_DIVIDE,
    VK_RETURN, // KeypadEnter
    0,         // KeypadEquals
    VK_SUBTRACT,
    VK_MULTIPLY,
    VK_ADD,

    // others
    VK_OEM_5, // Backslash
    VK_OEM_COMMA,
    VK_OEM_PLUS, // Equal
    VK_OEM_3,    // Grave
    VK_OEM_4,    // LeftBracket
    VK_OEM_MINUS,
    VK_OEM_PERIOD,
    VK_OEM_7, // Quote
    VK_OEM_6, // RightBracket
    VK_OEM_1, // Semicolon
    VK_OEM_2, // Slash

    // modifiers
    VK_LWIN,
    VK_CONTROL,
    0, // Function
    VK_MENU,
    VK_SHIFT,
    VK_RWIN,
    VK_RCONTROL,
    VK_RMENU,
    VK_RSHIFT,
    VK_CAPITAL,
    VK_NUMLOCK,
    VK_SCROLL,

    // arrows
    VK_DOWN,
    VK_LEFT,
    VK_RIGHT,
    VK_UP,

    // navigation
    VK_END,
    VK_HOME,
    VK_PRIOR,
    VK_NEXT,

    // actions
    VK_APPS, // ContextMenu
    VK_BACK, // Delete
    VK_ESCAPE,
    VK_DELETE, // ForwardDelete
    VK_HELP,
    VK_VOLUME_MUTE,
    VK_SNAPSHOT, // PrintScreen
    VK_RETURN,
    VK_SPACE,
    VK_TAB,
    VK_VOLUME_DOWN,
    VK_VOLUME_UP,
    VK_PAUSE,
    VK_INSERT,
    VK_SLEEP,
};

#elif __linux__
#include <GL/freeglut.h>

const int _native_key_mapping[] = {
    0,
    // numbers
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
    // letters
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
    'k', 'l', 'm', 'n', 'o','p', 'q', 'r', 's', 't',
    'u', 'v', 'w', 'x', 'y', 'z',
    // function
    GLUT_KEY_F1, GLUT_KEY_F2, GLUT_KEY_F3, GLUT_KEY_F4, GLUT_KEY_F5, GLUT_KEY_F6, GLUT_KEY_F7,
    GLUT_KEY_F8, GLUT_KEY_F9, GLUT_KEY_F10, GLUT_KEY_F11, GLUT_KEY_F12,
    0,0,0,0,0,0,0,0,0,0,0,0,

    // keypad numbers (glut doesn't have these)
    0,0,0,0,0,0,0,0,0,0,

    // keypad other
    0, '.', '/', '\r', '=', '-', '*', '+',

    // others
    '\\', ',', '=', '`', '[', '-', '.', '\'', ']', ';', '/',
    // XK_backslash, XK_comma, XK_equal, XK_grave, XK_bracketleft, XK_minus, XK_period, XK_apostrophe,
    // XK_bracketright, XK_semicolon, XK_slash,

    // modifiers
    0,
    GLUT_KEY_CTRL_L,
    0, // Function
    GLUT_KEY_ALT_L, GLUT_KEY_SHIFT_L,
    0, // RightMeta
    GLUT_KEY_CTRL_R, GLUT_KEY_ALT_R, GLUT_KEY_SHIFT_R, 0, 0, 0,
    // arrows
    GLUT_KEY_DOWN, GLUT_KEY_LEFT, GLUT_KEY_RIGHT, GLUT_KEY_UP,
    // navigation
    GLUT_KEY_END, GLUT_KEY_HOME, GLUT_KEY_PAGE_DOWN, GLUT_KEY_PAGE_UP,

    // actions
    0, 0, 27, GLUT_KEY_DELETE, 0,
    0, // Mute
    0, // PrintScreen
    '\r', ' ', '\t',
    0, // VolumeDown
    0, // VolumeUp
    0, GLUT_KEY_INSERT,
    0, // Sleep
};

#elif __EMSCRIPTEN__
#include <emscripten.h>
// not implemented yet
#elif __APPLE__
// not implemented yet
#endif

const int KEY_MAPPING_SIZE = sizeof(_native_key_mapping) / sizeof(int);
static_assert(KEY_MAPPING_SIZE == sizeof(kVirtualKeyCodes)/sizeof(char*));

static const std::map<int, int> KEY_MAPPING = []() {
    std::map<int, int> map;
    for (int i=0; i<KEY_MAPPING_SIZE; i++) {
        uint8_t byte = static_cast<uint8_t>(_native_key_mapping[i]);
        if(byte == 0) continue;
        map[byte] = i;
    }
    return map;
}();