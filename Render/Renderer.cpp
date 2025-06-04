#include "Renderer.hpp"
#include "Config/Config.hpp"

namespace RealSix
{
    std::unique_ptr<IGfxDevice> Renderer::mGfxDevice{nullptr};
    Window* Renderer::mWindow{nullptr};

    Renderer::Renderer(Window *window)
    {
        mWindow = window;
        mGfxDevice.reset(IGfxDevice::Create(mWindow));
    }

    Renderer::~Renderer()
    {
        mFrameGraph.ExportGraphviz("frameGraph.gv");
        mFrameGraph.Clear();
    }

    IGfxDevice *Renderer::GetGfxDevice()
    {
        return mGfxDevice.get();
    }

    Window *Renderer::GetWindow()
    {
        return mWindow;
    }

    void Renderer::BeginFrame()
    {
        mGfxDevice->BeginFrame();
    }

    void Renderer::Render()
    {
        mFrameGraph.Execute();
    }

    void Renderer::EndFrame()
    {
        mGfxDevice->EndFrame();
    }

} // namespace RealSix
