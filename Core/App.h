#pragma once
#include <memory>
#include "Platform/Window.h"
#include "Platform/InputSystem.h"
#include "Render/Renderer.h"
namespace RealSix
{
    class App
    {
    public:
        App() = default;
        virtual ~App() = default;

        void Run();
        void Quit();

        Window *GetWindow() const;
        InputSystem *GetInputSystem() const;
        Renderer *GetRenderer() const;

    protected:
        enum class AppState
        {
            TICK,
            PAUSE,
            QUIT
        };

        virtual void SetupConfig();
        virtual void Init();
        virtual void PreTick();
        virtual void Tick();
        virtual void Render();
        virtual void RenderGizmo();
        virtual void RenderUI();
        virtual void PostTick();
        virtual void Destroy();

        AppState mState;

        std::unique_ptr<Window> mWindow;
        std::unique_ptr<InputSystem> mInputSystem;

        std::unique_ptr<Renderer> mRenderer;

    private:
        void Draw();
    };
}