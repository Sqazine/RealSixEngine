#pragma once
#include <cstdint>
#include "Core/String.hpp"
#include <vector>
#include "Math/Vector2.hpp"

namespace RealSix
{
    class Window
    {
    public:
        enum Event : uint32_t
        {
            NONE = 0,
            MIN = 1,
            MAX = 2,
            RESTORE = 4,
            CLOSE = 8,
            RESIZE = 16,
            MOVE = 32,
            ENTER = 64,
            LEAVE = 128,
            EXPOSE = 256,
        };

        static Window *Create();

        Window() = default;
        virtual ~Window() = default;

        virtual void SetTitle(StringView str) = 0;
        virtual StringView GetTitle() const = 0;
        virtual void Resize(const Vector2u32 &extent) = 0;
        virtual void Resize(uint32_t w, uint32_t h) = 0;
        virtual Vector2u32 GetSize() = 0;
        virtual float GetAspect() = 0;

        virtual void Show() = 0;
        virtual void Hide() = 0;

        virtual void SetEvent(Event event)
        {
            mEvent = Event((uint32_t)mEvent | (uint32_t)event);
        }

        virtual Event GetEvent() const
        {
            return mEvent;
        }

        virtual bool HasEvent(Event event)
        {
            return (mEvent & event) == event;
        }

        virtual bool HasEvent(uint32_t event)
        {
            return (mEvent & (Event)event) != Event::NONE;
        }

        virtual void ClearEvent()
        {
            mEvent = Event::NONE;
        }
    protected:
        Event mEvent{Event::NONE};
    };
}