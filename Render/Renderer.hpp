#pragma once
#include <memory>
#include <vector>
#include "Config/Config.hpp"
#include "Platform/Window.hpp"
#include "Gfx/IGfxDevice.hpp"
#include "FrameGraph/FrameGraph.hpp"
#include "FrameGraph/FrameGraphResource.hpp"
#include "FrameGraph/FrameGraphRenderTaskBuilder.hpp"
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