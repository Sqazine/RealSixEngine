#pragma once
#include <vector>
#include <cstdint>
#include <memory>
#include "Platform/Window.hpp"
#include "Math/Vector2.hpp"
namespace RealSix
{
    enum class KeyCode : uint32_t
    {
        A,
        B,
        C,
        D,
        E,
        F,
        G,
        H,
        I,
        J,
        K,
        L,
        M,
        N,
        O,
        P,
        Q,
        R,
        S,
        T,
        U,
        V,
        W,
        X,
        Y,
        Z,

        KEY_1,
        KEY_2,
        KEY_3,
        KEY_4,
        KEY_5,
        KEY_6,
        KEY_7,
        KEY_8,
        KEY_9,
        KEY_0,

        RETURN,
        ESCAPE,
        BACKSPACE,
        TAB,
        SPACE,

        MINUS,
        EQUALS,
        LEFTBRACKET,
        RIGHTBRACKET,
        BACKSLASH,
        NONUSHASH,
        SEMICOLON,
        APOSTROPHE,
        GRAVE,
        COMMA,
        PERIOD,
        SLASH,

        CAPSLOCK,

        F1,
        F2,
        F3,
        F4,
        F5,
        F6,
        F7,
        F8,
        F9,
        F10,
        F11,
        F12,

        PRINTSCREEN,
        SCROLLLOCK,
        PAUSE,
        INSERT,
        HOME,
        PAGEUP,
        DELETE,
        END,
        PAGEDOWN,
        RIGHT,
        LEFT,
        DOWN,
        UP,
        NUMLOCKCLEAR,
        KP_DIVIDE,
        KP_MULTIPLY,
        KP_MINUS,
        KP_PLUS,
        KP_ENTER,
        KP_1,
        KP_2,
        KP_3,
        KP_4,
        KP_5,
        KP_6,
        KP_7,
        KP_8,
        KP_9,
        KP_0,
        KP_PERIOD,
        NONUSBACKSLASH,
        APPLICATION,
        POWER,
        KP_EQUALS,
        F13,
        F14,
        F15,
        F16,
        F17,
        F18,
        F19,
        F20,
        F21,
        F22,
        F23,
        F24,
        EXECUTE,
        HELP,
        MENU,
        SELECT,
        STOP,
        AGAIN,
        UNDO,
        CUT,
        COPY,
        PASTE,
        FIND,
        MUTE,
        VOLUMEUP,
        VOLUMEDOWN,
        KP_COMMA,
        KP_EQUALSAS400,
        INTERNATIONAL1,
        INTERNATIONAL2,
        INTERNATIONAL3,
        INTERNATIONAL4,
        INTERNATIONAL5,
        INTERNATIONAL6,
        INTERNATIONAL7,
        INTERNATIONAL8,
        INTERNATIONAL9,
        LANG1,
        LANG2,
        LANG3,
        LANG4,
        LANG5,
        LANG6,
        LANG7,
        LANG8,
        LANG9,

        ALTERASE,
        SYSREQ,
        CANCEL,
        CLEAR,
        PRIOR,
        RETURN2,
        SEPARATOR,
        OUT,
        OPER,
        CLEARAGAIN,
        CRSEL,
        EXSEL,

        KP_00,
        KP_000,
        THOUSANDSSEPARATOR,
        DECIMALSEPARATOR,
        CURRENCYUNIT,
        CURRENCYSUBUNIT,
        KP_LEFTPAREN,
        KP_RIGHTPAREN,
        KP_LEFTBRACE,
        KP_RIGHTBRACE,
        KP_TAB,
        KP_BACKSPACE,
        KP_A,
        KP_B,
        KP_C,
        KP_D,
        KP_E,
        KP_F,
        KP_XOR,
        KP_POWER,
        KP_PERCENT,
        KP_LESS,
        KP_GREATER,
        KP_AMPERSAND,
        KP_DBLAMPERSAND,
        KP_VERTICALBAR,
        KP_DBLVERTICALBAR,
        KP_COLON,
        KP_HASH,
        KP_SPACE,
        KP_AT,
        KP_EXCLAM,
        KP_MEMSTORE,
        KP_MEMRECALL,
        KP_MEMCLEAR,
        KP_MEMADD,
        KP_MEMSUBTRACT,
        KP_MEMMULTIPLY,
        KP_MEMDIVIDE,
        KP_PLUSMINUS,
        KP_CLEAR,
        KP_CLEARENTRY,
        KP_BINARY,
        KP_OCTAL,
        KP_DECIMAL,
        KP_HEXADECIMAL,

        LCTRL,
        LSHIFT,
        LALT,
        LGUI,
        RCTRL,
        RSHIFT,
        RALT,
        RGUI,

        MODE,

        AUDIONEXT,
        AUDIOPREV,
        AUDIOSTOP,
        AUDIOPLAY,
        AUDIOMUTE,
        MEDIASELECT,
        WWW,
        MAIL,
        CALCULATOR,
        COMPUTER,
        AC_SEARCH,
        AC_HOME,
        AC_BACK,
        AC_FORWARD,
        AC_STOP,
        AC_REFRESH,
        AC_BOOKMARKS,
        BRIGHTNESSDOWN,
        BRIGHTNESSUP,
        DISPLAYSWITCH,
        KBDILLUMTOGGLE,
        KBDILLUMDOWN,
        KBDILLUMUP,
        EJECT,
        SLEEP,

        APP1,
        APP2,

        AUDIOREWIND,
        AUDIOFASTFORWARD,
    };

    #define KEYCODE_CAST(t) (static_cast<uint32_t>(std::underlying_type<KeyCode>::type(t)))
    ENUM_CLASS_OP(KEYCODE_CAST, KeyCode);

    enum class ControllerButton : uint32_t
    {
        A,
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
        MISC1,
        PADDLE1,
        PADDLE2,
        PADDLE3,
        PADDLE4,
        TOUCHPAD,
    };

        #define CONTROLLER_BUTTON_CAST(t) (static_cast<uint32_t>(std::underlying_type<ControllerButton>::type(t)))
    ENUM_CLASS_OP(CONTROLLER_BUTTON_CAST, ControllerButton);


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