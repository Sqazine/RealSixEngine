#include "EditorApp.hpp"
#include "Core/Logger.hpp"
#include "Core/Config.hpp"
#include "Platform/PlatformInfo.hpp"
#include "EditorUIPass/EditorUIPass.hpp"
namespace RealSix
{
    void EditorApp::SetupConfig()
    {
    }

    void EditorApp::Init()
    {
        App::Init();

        AddEditorUIPass(mRenderer->GetFrameGraph());
    }

    void EditorApp::Tick()
    {
        App::Tick();
    }

    void EditorApp::Render()
    {
        App::Render();
    }

    void EditorApp::RenderGizmo()
    {
        App::RenderGizmo();
    }

    void EditorApp::RenderUI()
    {
        App::RenderUI();
    }

    void EditorApp::RenderEditorUI()
    {
    }

    void EditorApp::CleanUp()
    {
        App::CleanUp();
    }

    void EditorApp::PreTick()
    {
        App::PreTick();
    }

    void EditorApp::PostTick()
    {
        App::PostTick();
    }

    void EditorApp::AddEditorUIPass(FrameGraph &frameGraph)
    {
        frameGraph.AddRenderTask<EditorUIPass>(
            true,
            [&]()
            {
                return EditorUIPass::Create(this);
            },
            [&](EditorUIPass *task, FrameGraphRenderTaskBuilder &builder)
            {
                task->Init();
            },
            [this, &frameGraph](EditorUIPass *task)
            {
                bool onlyEditorPass = frameGraph.HasOnly<EditorUIPass>();

                task->BeginRender();

                RenderEditorUI();

                task->EndRender(onlyEditorPass);
            });
    }
}
