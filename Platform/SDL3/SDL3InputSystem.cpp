#include "SDL3InputSystem.hpp"
#include "SDL3Window.hpp"
#include <memory>
#include <iostream>
#include "Platform/InputSystem.hpp"
#include "Platform/Window.hpp"
namespace RealSix
{
    uint8_t GetSDL3ScanCode(KeyCode keyCode)
    {
#define CONVERT(x)             \
    case KeyCode::KEYCODE_##x: \
        return SDL_SCANCODE_##x;

        switch (keyCode)
        {
            CONVERT(A)
            CONVERT(B)
            CONVERT(C)
            CONVERT(D)
            CONVERT(E)
            CONVERT(F)
            CONVERT(G)
            CONVERT(H)
            CONVERT(I)
            CONVERT(J)
            CONVERT(K)
            CONVERT(L)
            CONVERT(M)
            CONVERT(N)
            CONVERT(O)
            CONVERT(P)
            CONVERT(Q)
            CONVERT(R)
            CONVERT(S)
            CONVERT(T)
            CONVERT(U)
            CONVERT(V)
            CONVERT(W)
            CONVERT(X)
            CONVERT(Y)
            CONVERT(Z)
            CONVERT(1)
            CONVERT(2)
            CONVERT(3)
            CONVERT(4)
            CONVERT(5)
            CONVERT(6)
            CONVERT(7)
            CONVERT(8)
            CONVERT(9)
            CONVERT(0)
            CONVERT(RETURN)
            CONVERT(ESCAPE)
            CONVERT(BACKSPACE)
            CONVERT(TAB)
            CONVERT(SPACE)
            CONVERT(MINUS)
            CONVERT(EQUALS)
            CONVERT(LEFTBRACKET)
            CONVERT(RIGHTBRACKET)
            CONVERT(BACKSLASH)
            CONVERT(NONUSHASH)
            CONVERT(SEMICOLON)
            CONVERT(APOSTROPHE)
            CONVERT(COMMA)
            CONVERT(PERIOD)
            CONVERT(SLASH)
            CONVERT(GRAVE)
            CONVERT(CAPSLOCK)
            CONVERT(F1)
            CONVERT(F2)
            CONVERT(F3)
            CONVERT(F4)
            CONVERT(F5)
            CONVERT(F6)
            CONVERT(F7)
            CONVERT(F8)
            CONVERT(F9)
            CONVERT(F10)
            CONVERT(F11)
            CONVERT(F12)
            CONVERT(PRINTSCREEN)
            CONVERT(SCROLLLOCK)
            CONVERT(PAUSE)
            CONVERT(INSERT)
            CONVERT(HOME)
            CONVERT(PAGEUP)
            CONVERT(DELETE)
            CONVERT(END)
            CONVERT(PAGEDOWN)
            CONVERT(RIGHT)
            CONVERT(LEFT)
            CONVERT(DOWN)
            CONVERT(UP)
            CONVERT(NUMLOCKCLEAR)
            CONVERT(KP_DIVIDE)
            CONVERT(KP_MULTIPLY)
            CONVERT(KP_MINUS)
            CONVERT(KP_PLUS)
            CONVERT(KP_ENTER)
            CONVERT(KP_1)
            CONVERT(KP_2)
            CONVERT(KP_3)
            CONVERT(KP_4)
            CONVERT(KP_5)
            CONVERT(KP_6)
            CONVERT(KP_7)
            CONVERT(KP_8)
            CONVERT(KP_9)
            CONVERT(KP_0)
            CONVERT(KP_PERIOD)
            CONVERT(LCTRL)
            CONVERT(LSHIFT)
            CONVERT(LALT)
            CONVERT(RCTRL)
            CONVERT(RSHIFT)
            CONVERT(RALT)

        default:
            std::cerr << "Unsupported key code!" << std::endl;
            break;
        }
        return 0; // Return 0 for unsupported key codes
    }

    SDL3Keyboard::SDL3Keyboard()
    {
    }
    SDL3Keyboard::~SDL3Keyboard()
    {
    }
    bool SDL3Keyboard::GetKeyValue(KeyCode keyCode) const
    {
        return mCurKeyState[GetSDL3ScanCode(keyCode)] == 1 ? true : false;
    }

    ButtonState SDL3Keyboard::GetKeyState(KeyCode keyCode) const
    {
        auto sdl3Scancode = GetSDL3ScanCode(keyCode);
        if (mPreKeyState[sdl3Scancode] == 0)
        {
            if (mCurKeyState[sdl3Scancode] == 0)
                return ButtonState::NONE;
            else
                return ButtonState::PRESS;
        }
        else
        {
            if (mCurKeyState[sdl3Scancode] == 0)
                return ButtonState::RELEASE;
            else
                return ButtonState::HOLD;
        }
    }

    SDL3Mouse::SDL3Mouse()
        : mCurPos(Vector2f::ZERO), mPrePos(Vector2f::ZERO), mMouseScrollWheel(Vector2f::ZERO), mCurButtons(0), mPreButtons(0), mIsRelative(false)
    {
    }
    SDL3Mouse::~SDL3Mouse()
    {
    }
    bool SDL3Mouse::GetButtonValue(int32_t button) const
    {
        return (mCurButtons & SDL_BUTTON_MASK(button)) == 1;
    }

    ButtonState SDL3Mouse::GetButtonState(int32_t button) const
    {
        if ((mPreButtons & SDL_BUTTON_MASK(button)) == 0)
        {
            if ((mPreButtons & SDL_BUTTON_MASK(button)) == 0)
                return ButtonState::NONE;
            else
                return ButtonState::PRESS;
        }
        else
        {
            if ((mPreButtons & SDL_BUTTON_MASK(button)) == 0)
                return ButtonState::RELEASE;
            else
                return ButtonState::HOLD;
        }
    }

    Vector2f SDL3Mouse::GetMousePos() const
    {
        return mCurPos;
    }

    Vector2f SDL3Mouse::GetReleativeMove() const
    {
        return mCurPos - mPrePos;
    }

    Vector2f SDL3Mouse::GetMouseScrollWheel() const
    {
        return mMouseScrollWheel;
    }

    void SDL3Mouse::SetReleativeMode(Window *pWindow, bool isActive)
    {
        mIsRelative = isActive;

        auto sdlWindow = static_cast<SDL3Window *>(pWindow)->GetHandle();

        if (isActive)
            SDL_SetWindowRelativeMouseMode(sdlWindow, true);
        else
            SDL_SetWindowRelativeMouseMode(sdlWindow, false);
    }

    bool SDL3Mouse::IsReleativeMode(Window *pWindow) const
    {
        return mIsRelative;
    }

    SDL3Controller::SDL3Controller()
        : mIsConnected(false),
          mLeftStickValue(Vector2f::ZERO), mRightStickValue(Vector2f::ZERO),
          mLeftTriggerValue(0.0f), mRightTriggerValue(0.0f)
    {
    }

    SDL3Controller::~SDL3Controller()
    {
    }

    bool SDL3Controller::GetButtonValue(ControllerButton button) const
    {
        return mCurrentButtons[button];
    }

    ButtonState SDL3Controller::GetButtonState(ControllerButton button) const
    {
        if (mPreviousButtons[button] == 0)
        {
            if (mCurrentButtons[button] == 0)
                return ButtonState::NONE;
            else
                return ButtonState::PRESS;
        }
        else
        {
            if (mCurrentButtons[button] == 0)
                return ButtonState::RELEASE;
            else
                return ButtonState::HOLD;
        }
    }

    float SDL3Controller::GetLeftTriggerValue() const
    {
        return mLeftTriggerValue;
    }

    float SDL3Controller::GetRightTriggerValue() const
    {
        return mRightTriggerValue;
    }

    const Vector2f &SDL3Controller::GetLeftStickValue() const
    {
        return mLeftStickValue;
    }

    const Vector2f &SDL3Controller::GetRightStickValue() const
    {
        return mRightStickValue;
    }

    bool SDL3Controller::IsConnected() const
    {
        return mIsConnected;
    }

    SDL3InputSystem::SDL3InputSystem()
    {
        mMouse = std::make_unique<SDL3Mouse>();
        mKeyboard = std::make_unique<SDL3Keyboard>();

        Init();
    }
    SDL3InputSystem::~SDL3InputSystem()
    {
    }

    void SDL3InputSystem::Init()
    {
        ((SDL3Keyboard *)mKeyboard.get())->mCurKeyState = SDL_GetKeyboardState(nullptr);
        ((SDL3Keyboard *)mKeyboard.get())->mPreKeyState = new bool[SDL_SCANCODE_COUNT];
        memset(((SDL3Keyboard *)mKeyboard.get())->mPreKeyState, 0, SDL_SCANCODE_COUNT);
    }

    void SDL3InputSystem::PreTick(Window *pWindow)
    {
        memcpy(((SDL3Keyboard *)mKeyboard.get())->mPreKeyState, ((SDL3Keyboard *)mKeyboard.get())->mCurKeyState, SDL_SCANCODE_COUNT);
        ((SDL3Mouse *)mMouse.get())->mPreButtons = ((SDL3Mouse *)mMouse.get())->mCurButtons;
        ((SDL3Mouse *)mMouse.get())->mPrePos = ((SDL3Mouse *)mMouse.get())->mCurPos;
        ((SDL3Mouse *)mMouse.get())->mMouseScrollWheel = Vector2f::ZERO;

        ProcessEvent(pWindow);
    }

    void SDL3InputSystem::PostTick()
    {
        Vector2f p = Vector2f::ZERO;
        if (!((SDL3Mouse *)mMouse.get())->mIsRelative)
            ((SDL3Mouse *)mMouse.get())->mCurButtons = SDL_GetMouseState((&p.x), (&p.y));
        else
            ((SDL3Mouse *)mMouse.get())->mCurButtons = SDL_GetRelativeMouseState((&p.x), (&p.y));
        ((SDL3Mouse *)mMouse.get())->mCurPos = p;
    }

    void SDL3InputSystem::RegisterEventCallback(std::function<void(SDL_Event)> callback)
    {
        mCallbacks.emplace_back(callback);
    }

    void SDL3InputSystem::ProcessEvent(Window *pWindow)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            for (const auto& callback : mCallbacks)
            {
                callback(event);
            }

            switch (event.type)
            {
            case SDL_EVENT_MOUSE_WHEEL:
                ((SDL3Mouse *)mMouse.get())->mMouseScrollWheel = Vector2f(event.wheel.x, static_cast<float>(event.wheel.y));
                break;
            case SDL_EVENT_QUIT:
                pWindow->SetEvent(Window::Event::CLOSE);
                break;
            case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
                {
                    if(event.window.windowID == SDL_GetWindowID(static_cast<SDL3Window*>(pWindow)->GetHandle()))
                        pWindow->SetEvent(Window::Event::CLOSE);
                    break;
                }
            case SDL_EVENT_WINDOW_MINIMIZED:
                pWindow->SetEvent(Window::Event::MIN);
                break;
            case SDL_EVENT_WINDOW_MAXIMIZED:
                pWindow->SetEvent(Window::Event::MAX);
                break;
            case SDL_EVENT_WINDOW_RESIZED:
                pWindow->SetEvent(Window::Event::RESIZE);
                break;
            case SDL_EVENT_WINDOW_RESTORED:
                pWindow->SetEvent(Window::Event::RESTORE);
                break;
            case SDL_EVENT_WINDOW_MOVED:
                pWindow->SetEvent(Window::Event::MOVE);
                break;
            case SDL_EVENT_WINDOW_EXPOSED:
                pWindow->SetEvent(Window::Event::EXPOSE);
                break;
            case SDL_EVENT_WINDOW_MOUSE_ENTER:
                pWindow->SetEvent(Window::Event::ENTER);
                break;
            case SDL_EVENT_WINDOW_MOUSE_LEAVE:
                pWindow->SetEvent(Window::Event::LEAVE);
                break;
            default:
                break;
            }
        }
    }
}