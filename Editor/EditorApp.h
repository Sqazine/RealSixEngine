#pragma once
#include <cstdint>
#include <string>
#include <memory>
#include <vector>
#include "Core/Singleton.h"
#include "Core/App.h"
#include "Math/Matrix4.h"
namespace RealSix
{
    class EditorApp : public App
    {
    public:
        EditorApp() = default;
        ~EditorApp() override = default;

    protected:
        virtual void SetupConfig() override;
        virtual void Init() override;
        virtual void PreTick() override;
        virtual void Tick() override;
        virtual void Render() override;
        virtual void RenderGizmo() override;
        virtual void RenderUI() override;
        virtual void RenderEditorUI();
        virtual void PostTick() override;
        virtual void Destroy() override;

        void AddEditorUIPass(FrameGraph& frameGraph);
    };
}