#pragma once
#include "EditorUIPass.hpp"
#include <vulkan/vulkan.h>
namespace RealSix
{
    class EditorApp;
    class EditorUIPassVulkanImpl : public EditorUIPass
    {
    public:
        EditorUIPassVulkanImpl(EditorApp *editorApp);
        ~EditorUIPassVulkanImpl() override;

        void Init() override;
        void BeginRender() override;
        void EndRender(bool onlyEditorPass) override;

    private:
        EditorApp *mEditorApp;

        VkDescriptorPool mDescriptorPool;
    };
}