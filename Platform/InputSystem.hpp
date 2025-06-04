#pragma once
#include <vector>
#include <cstdint>
#include <memory>
#include "Platform/Window.hpp"
#include "Math/Vector2.hpp"
namespace RealSix
{
    enum KeyCode
    {
        KEYCODE_A = 4,
        KEYCODE_B = 5,
        KEYCODE_C = 6,
        KEYCODE_D = 7,
        KEYCODE_E = 8,
        KEYCODE_F = 9,
        KEYCODE_G = 10,
        KEYCODE_H = 11,
        KEYCODE_I = 12,
        KEYCODE_J = 13,
        KEYCODE_K = 14,
        KEYCODE_L = 15,
        KEYCODE_M = 16,
        KEYCODE_N = 17,
        KEYCODE_O = 18,
        KEYCODE_P = 19,
        KEYCODE_Q = 20,
        KEYCODE_R = 21,
        KEYCODE_S = 22,
        KEYCODE_T = 23,
        KEYCODE_U = 24,
        KEYCODE_V = 25,
        KEYCODE_W = 26,
        KEYCODE_X = 27,
        KEYCODE_Y = 28,
        KEYCODE_Z = 29,

        KEYCODE_1 = 30,
        KEYCODE_2 = 31,
        KEYCODE_3 = 32,
        KEYCODE_4 = 33,
        KEYCODE_5 = 34,
        KEYCODE_6 = 35,
        KEYCODE_7 = 36,
        KEYCODE_8 = 37,
        KEYCODE_9 = 38,
        KEYCODE_0 = 39,

        KEYCODE_RETURN = 40,
        KEYCODE_ESCAPE = 41,
        KEYCODE_BACKSPACE = 42,
        KEYCODE_TAB = 43,
        KEYCODE_SPACE = 44,

        KEYCODE_MINUS = 45,
        KEYCODE_EQUALS = 46,
        KEYCODE_LEFTBRACKET = 47,
        KEYCODE_RIGHTBRACKET = 48,
        KEYCODE_BACKSLASH = 49,
        KEYCODE_NONUSHASH = 50,
        KEYCODE_SEMICOLON = 51,
        KEYCODE_APOSTROPHE = 52,
        KEYCODE_GRAVE = 53,
        KEYCODE_COMMA = 54,
        KEYCODE_PERIOD = 55,
        KEYCODE_SLASH = 56,

        KEYCODE_CAPSLOCK = 57,

        KEYCODE_F1 = 58,
        KEYCODE_F2 = 59,
        KEYCODE_F3 = 60,
        KEYCODE_F4 = 61,
        KEYCODE_F5 = 62,
        KEYCODE_F6 = 63,
        KEYCODE_F7 = 64,
        KEYCODE_F8 = 65,
        KEYCODE_F9 = 66,
        KEYCODE_F10 = 67,
        KEYCODE_F11 = 68,
        KEYCODE_F12 = 69,

        KEYCODE_PRINTSCREEN = 70,
        KEYCODE_SCROLLLOCK = 71,
        KEYCODE_PAUSE = 72,
        KEYCODE_INSERT = 73, /**< insert on PC, help on some Mac keyboards (but
                                   does send code 73, not 117) */
        KEYCODE_HOME = 74,
        KEYCODE_PAGEUP = 75,
        KEYCODE_DELETE = 76,
        KEYCODE_END = 77,
        KEYCODE_PAGEDOWN = 78,
        KEYCODE_RIGHT = 79,
        KEYCODE_LEFT = 80,
        KEYCODE_DOWN = 81,
        KEYCODE_UP = 82,

        KEYCODE_NUMLOCKCLEAR = 83, /**< num lock on PC, clear on Mac keyboards
                                    */
        KEYCODE_KP_DIVIDE = 84,
        KEYCODE_KP_MULTIPLY = 85,
        KEYCODE_KP_MINUS = 86,
        KEYCODE_KP_PLUS = 87,
        KEYCODE_KP_ENTER = 88,
        KEYCODE_KP_1 = 89,
        KEYCODE_KP_2 = 90,
        KEYCODE_KP_3 = 91,
        KEYCODE_KP_4 = 92,
        KEYCODE_KP_5 = 93,
        KEYCODE_KP_6 = 94,
        KEYCODE_KP_7 = 95,
        KEYCODE_KP_8 = 96,
        KEYCODE_KP_9 = 97,
        KEYCODE_KP_0 = 98,
        KEYCODE_KP_PERIOD = 99,

        KEYCODE_NONUSBACKSLASH = 100, /**< This is the additional key that ISO
                                       *   keyboards have over ANSI ones,
                                       *   located between left shift and Y.
                                       *   Produces GRAVE ACCENT and TILDE in a
                                       *   US or UK Mac layout, REVERSE SOLIDUS
                                       *   (backslash) and VERTICAL LINE in a
                                       *   US or UK Windows layout, and
                                       *   LESS-THAN SIGN and GREATER-THAN SIGN
                                       *   in a Swiss German, German, or French
                                       *   layout. */
        KEYCODE_APPLICATION = 101,    /**< windows contextual menu, compose */
        KEYCODE_POWER = 102,          /**< The USB document says this is a status flag,
                                       *   not a physical key - but some Mac keyboards
                                       *   do have a power key. */
        KEYCODE_KP_EQUALS = 103,
        KEYCODE_F13 = 104,
        KEYCODE_F14 = 105,
        KEYCODE_F15 = 106,
        KEYCODE_F16 = 107,
        KEYCODE_F17 = 108,
        KEYCODE_F18 = 109,
        KEYCODE_F19 = 110,
        KEYCODE_F20 = 111,
        KEYCODE_F21 = 112,
        KEYCODE_F22 = 113,
        KEYCODE_F23 = 114,
        KEYCODE_F24 = 115,
        KEYCODE_EXECUTE = 116,
        KEYCODE_HELP = 117,
        KEYCODE_MENU = 118,
        KEYCODE_SELECT = 119,
        KEYCODE_STOP = 120,
        KEYCODE_AGAIN = 121, /**< redo */
        KEYCODE_UNDO = 122,
        KEYCODE_CUT = 123,
        KEYCODE_COPY = 124,
        KEYCODE_PASTE = 125,
        KEYCODE_FIND = 126,
        KEYCODE_MUTE = 127,
        KEYCODE_VOLUMEUP = 128,
        KEYCODE_VOLUMEDOWN = 129,
        /* not sure whether there's a reason to enable these */
        /*     KEYCODE_LOCKINGCAPSLOCK = 130,  */
        /*     KEYCODE_LOCKINGNUMLOCK = 131, */
        /*     KEYCODE_LOCKINGSCROLLLOCK = 132, */
        KEYCODE_KP_COMMA = 133,
        KEYCODE_KP_EQUALSAS400 = 134,

        KEYCODE_INTERNATIONAL1 = 135, /**< used on Asian keyboards, see
                                            footnotes in USB doc */
        KEYCODE_INTERNATIONAL2 = 136,
        KEYCODE_INTERNATIONAL3 = 137, /**< Yen */
        KEYCODE_INTERNATIONAL4 = 138,
        KEYCODE_INTERNATIONAL5 = 139,
        KEYCODE_INTERNATIONAL6 = 140,
        KEYCODE_INTERNATIONAL7 = 141,
        KEYCODE_INTERNATIONAL8 = 142,
        KEYCODE_INTERNATIONAL9 = 143,
        KEYCODE_LANG1 = 144, /**< Hangul/English toggle */
        KEYCODE_LANG2 = 145, /**< Hanja conversion */
        KEYCODE_LANG3 = 146, /**< Katakana */
        KEYCODE_LANG4 = 147, /**< Hiragana */
        KEYCODE_LANG5 = 148, /**< Zenkaku/Hankaku */
        KEYCODE_LANG6 = 149, /**< reserved */
        KEYCODE_LANG7 = 150, /**< reserved */
        KEYCODE_LANG8 = 151, /**< reserved */
        KEYCODE_LANG9 = 152, /**< reserved */

        KEYCODE_ALTERASE = 153, /**< Erase-Eaze */
        KEYCODE_SYSREQ = 154,
        KEYCODE_CANCEL = 155,
        KEYCODE_CLEAR = 156,
        KEYCODE_PRIOR = 157,
        KEYCODE_RETURN2 = 158,
        KEYCODE_SEPARATOR = 159,
        KEYCODE_OUT = 160,
        KEYCODE_OPER = 161,
        KEYCODE_CLEARAGAIN = 162,
        KEYCODE_CRSEL = 163,
        KEYCODE_EXSEL = 164,

        KEYCODE_KP_00 = 176,
        KEYCODE_KP_000 = 177,
        KEYCODE_THOUSANDSSEPARATOR = 178,
        KEYCODE_DECIMALSEPARATOR = 179,
        KEYCODE_CURRENCYUNIT = 180,
        KEYCODE_CURRENCYSUBUNIT = 181,
        KEYCODE_KP_LEFTPAREN = 182,
        KEYCODE_KP_RIGHTPAREN = 183,
        KEYCODE_KP_LEFTBRACE = 184,
        KEYCODE_KP_RIGHTBRACE = 185,
        KEYCODE_KP_TAB = 186,
        KEYCODE_KP_BACKSPACE = 187,
        KEYCODE_KP_A = 188,
        KEYCODE_KP_B = 189,
        KEYCODE_KP_C = 190,
        KEYCODE_KP_D = 191,
        KEYCODE_KP_E = 192,
        KEYCODE_KP_F = 193,
        KEYCODE_KP_XOR = 194,
        KEYCODE_KP_POWER = 195,
        KEYCODE_KP_PERCENT = 196,
        KEYCODE_KP_LESS = 197,
        KEYCODE_KP_GREATER = 198,
        KEYCODE_KP_AMPERSAND = 199,
        KEYCODE_KP_DBLAMPERSAND = 200,
        KEYCODE_KP_VERTICALBAR = 201,
        KEYCODE_KP_DBLVERTICALBAR = 202,
        KEYCODE_KP_COLON = 203,
        KEYCODE_KP_HASH = 204,
        KEYCODE_KP_SPACE = 205,
        KEYCODE_KP_AT = 206,
        KEYCODE_KP_EXCLAM = 207,
        KEYCODE_KP_MEMSTORE = 208,
        KEYCODE_KP_MEMRECALL = 209,
        KEYCODE_KP_MEMCLEAR = 210,
        KEYCODE_KP_MEMADD = 211,
        KEYCODE_KP_MEMSUBTRACT = 212,
        KEYCODE_KP_MEMMULTIPLY = 213,
        KEYCODE_KP_MEMDIVIDE = 214,
        KEYCODE_KP_PLUSMINUS = 215,
        KEYCODE_KP_CLEAR = 216,
        KEYCODE_KP_CLEARENTRY = 217,
        KEYCODE_KP_BINARY = 218,
        KEYCODE_KP_OCTAL = 219,
        KEYCODE_KP_DECIMAL = 220,
        KEYCODE_KP_HEXADECIMAL = 221,

        KEYCODE_LCTRL = 224,
        KEYCODE_LSHIFT = 225,
        KEYCODE_LALT = 226, /**< alt, option */
        KEYCODE_LGUI = 227, /**< windows, command (apple), meta */
        KEYCODE_RCTRL = 228,
        KEYCODE_RSHIFT = 229,
        KEYCODE_RALT = 230, /**< alt gr, option */
        KEYCODE_RGUI = 231, /**< windows, command (apple), meta */

        KEYCODE_MODE = 257, /**< I'm not sure if this is really not covered
                             *   by any of the above, but since there's a
                             *   special KMOD_MODE for it I'm adding it here
                             */

        /* @} */ /* Usage page 0x07 */

        /**
         *  \name Usage page 0x0C
         *
         *  These values are mapped from usage page 0x0C (USB consumer page).
         */
        /* @{ */

        AUDIONEXT = 258,
        AUDIOPREV = 259,
        AUDIOSTOP = 260,
        AUDIOPLAY = 261,
        AUDIOMUTE = 262,
        MEDIASELECT = 263,
        WWW = 264,
        MAIL = 265,
        CALCULATOR = 266,
        COMPUTER = 267,
        AC_SEARCH = 268,
        AC_HOME = 269,
        AC_BACK = 270,
        AC_FORWARD = 271,
        AC_STOP = 272,
        AC_REFRESH = 273,
        AC_BOOKMARKS = 274,
        BRIGHTNESSDOWN = 275,
        BRIGHTNESSUP = 276,
        DISPLAYSWITCH = 277, /**< display mirroring/dual display
                                   switch, video mode switch */
        KBDILLUMTOGGLE = 278,
        KBDILLUMDOWN = 279,
        KBDILLUMUP = 280,
        EJECT = 281,
        SLEEP = 282,

        APP1 = 283,
        APP2 = 284,

        AUDIOREWIND = 285,
        AUDIOFASTFORWARD = 286,
    };

    enum ControllerButton
    {
        A = 0,
        B,
        X,
        Y,
        BACK,
        GUIDE,
        START,
        LEFTSTICK,
        RIGHTSTICK,
        LEFTSHOULDER,
        RIGHTSHOULDER,
        DPAD_UP,
        DPAD_DOWN,
        DPAD_LEFT,
        DPAD_RIGHT,
        MISC1,    /* Xbox Series X share button, PS5 microphone button, Nintendo Switch Pro capture button */
        PADDLE1,  /* Xbox Elite paddle P1 */
        PADDLE2,  /* Xbox Elite paddle P3 */
        PADDLE3,  /* Xbox Elite paddle P2 */
        PADDLE4,  /* Xbox Elite paddle P4 */
        TOUCHPAD, /* PS4/PS5 touchpad button */
    };

    enum class ButtonState
    {
        NONE,
        PRESS,
        RELEASE,
        HOLD
    };

    class Keyboard
    {
    public:
        Keyboard() = default;
        virtual ~Keyboard() = default;
        virtual bool GetKeyValue(KeyCode keyCode) const = 0;
        virtual ButtonState GetKeyState(KeyCode keyCode) const = 0;
    };

    class Mouse
    {
    public:
        Mouse() = default;
        virtual ~Mouse() = default;
        virtual bool GetButtonValue(int32_t button) const = 0;
        virtual ButtonState GetButtonState(int32_t button) const = 0;
        virtual Vector2f GetMousePos() const = 0;
        virtual Vector2f GetReleativeMove() const = 0;
        virtual Vector2f GetMouseScrollWheel() const = 0;

        virtual void SetReleativeMode(Window *pWindow, bool isActive) = 0;
        virtual bool IsReleativeMode(Window *pWindow) const = 0;
    };

    class Controller
    {
    public:
        Controller() = default;
        virtual ~Controller() = default;

        virtual bool GetButtonValue(ControllerButton button) const = 0;

        virtual enum ButtonState GetButtonState(ControllerButton button) const = 0;

        virtual float GetLeftTriggerValue() const = 0;
        virtual float GetRightTriggerValue() const = 0;

        virtual const Vector2f &GetLeftStickValue() const = 0;
        virtual const Vector2f &GetRightStickValue() const = 0;

        virtual bool IsConnected() const = 0;
    };

    class InputSystem
    {
    public:
        InputSystem() = default;
        virtual ~InputSystem() = default;

        static InputSystem *Create();

        const Keyboard *GetKeyboard() const
        {
            return mKeyboard.get();
        }

        const Mouse *GetMouse() const
        {
            return mMouse.get();
        }

        size_t GetControllerCount() const
        {
            return mControllers.size();
        }

        const Controller *GetController(size_t index) const
        {
            if (index >= mControllers.size())
                return nullptr;
            return mControllers[index].get();
        }

        virtual void Init() = 0;
        virtual void PreTick(Window *pWindow) = 0;
        virtual void PostTick() = 0;

    protected:
        std::unique_ptr<Keyboard> mKeyboard;
        std::unique_ptr<Mouse> mMouse;
        std::vector<std::unique_ptr<Controller>> mControllers;
    };
}