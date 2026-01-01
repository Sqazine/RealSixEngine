#pragma once
#include "EditorUIPass.hpp"
#include <vulkan/vulkan.h>
namespace RealSix
{
    class EditorApp;
    class EditorUIPassImguiVulkanImpl : public EditorUIPass
    {
    public:
        EditorUIPassImguiVulkanImpl(EditorApp *editorApp);
        ~EditorUIPassImguiVulkanImpl() override;

        void Init() override;
        void BeginRender() override;
        void EndRender(bool onlyEditorPass) override;

    private:
        EditorApp *mEditorApp;

        VkDescriptorPool mDescriptorPool;
    };
}