#pragma once
#include <cstdint>
#include "String.hpp"
#include <memory>
#include <vector>

#include "Core/App.hpp"
#include "Editor/EditorApp.hpp"
namespace RealSix
{
    class TestApp : public EditorApp, public Singleton<TestApp>
    {
    public:
        TestApp() = default;
        ~TestApp() override = default;

    protected:
        void SetupConfig() override;
        void Init() override;
        void PreTick() override;
        void Tick() override;
        void Render() override;
        void RenderGizmo() override;
        void RenderUI() override;
        void RenderEditorUI() override;
        void PostTick() override;
        void CleanUp() override;

        void AddPasses(FrameGraph &frameGraph);

        bool mShowDemoWindow = true;
        bool mShowAnotherWindow = false;
        bool mRemoveBasicMeshPass = false;
        bool mRemoveMandelbrotSetPass = false;
        Vector4f mClearColor = Vector4f(0.45f, 0.55f, 0.60f, 1.00f);
    };

}