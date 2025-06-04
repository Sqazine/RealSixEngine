#pragma once
#include <cstdint>
#include "String.hpp"
#include <memory>
#include <vector>

#include "Core/App.hpp"
#include "Math/Matrix4.hpp"
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