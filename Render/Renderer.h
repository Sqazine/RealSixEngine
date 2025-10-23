#pragma once
#include <memory>
#include <vector>
#include "Config/Config.h"
#include "Platform/Window.h"
#include "Gfx/IGfxDevice.h"
#include "FrameGraph/FrameGraph.h"
#include "FrameGraph/Resource.h"
#include "FrameGraph/RenderTaskBuilder.h"
namespace RealSix
{
    class Renderer
    {
    public:
        Renderer(Window *window);
        virtual ~Renderer();

        static IGfxDevice *GetGfxDevice();
        static Window *GetWindow();

        void BeginFrame();
        virtual void Render();
        void EndFrame();

        FrameGraph &GetFrameGraph() { return mFrameGraph; }

    private:
        FrameGraph mFrameGraph;

        static Window *mWindow;
        static std::unique_ptr<IGfxDevice> mGfxDevice;
    };
}