#pragma once
#if _WIN32
#pragma comment(lib, "ucrtd.lib")
#endif
#include <vulkan/vulkan.h>
#include "String.hpp"
#include <vector>
#include <spirv_reflect.h>
#include <array>
#include <unordered_map>
#include "Gfx/IGfxShader.hpp"
#include "GfxVulkanObject.hpp"

namespace RealSix
{
    class GfxVulkanShaderModule;
    class GfxVulkanShaderCommon;

    class GfxVulkanVertexRasterShader : public GfxVulkanObject, public IGfxVertexRasterShader
    {
    public:
        GfxVulkanVertexRasterShader(IGfxDevice *device);
        ~GfxVulkanVertexRasterShader() override;

        const std::vector<VkPipelineShaderStageCreateInfo> &GetPipelineShaderStageInfoList() const;

        virtual IGfxShader *BindBuffer(StringView name, const IGfxBuffer *buffer) override;
        virtual IGfxShader *BindTexture(StringView name, const IGfxTexture *texture) override;
        virtual IGfxShader *Build() override;

        VkPipelineLayout GetPipelineLayout() const;
        void Flush();
        std::vector<VkDescriptorSet> &GetDescriptorSetList();

    private:
        void DumpDescriptorBindings();
        void DumpDescriptorSetLayouts();
        void DumpDescriptorWrites();

        VkShaderStageFlagBits GetShaderStageFlag(IGfxVertexRasterShader::Slot slot);

        std::unique_ptr<GfxVulkanShaderCommon> mShaderCommon;

        std::vector<VkPipelineShaderStageCreateInfo> mPipelineShaderStageCreateInfos;

        std::array<std::unique_ptr<GfxVulkanShaderModule>, static_cast<uint8_t>(IGfxVertexRasterShader::Slot::Num)> mShaderModules;
    };

    class GfxVulkanComputeShader : public GfxVulkanObject, public IGfxComputeShader
    {
    public:
        GfxVulkanComputeShader(IGfxDevice *device);
        ~GfxVulkanComputeShader() override;

        virtual IGfxShader *BindBuffer(StringView name, const IGfxBuffer *buffer) override;
        virtual IGfxShader *BindTexture(StringView name, const IGfxTexture *texture) override;
        virtual IGfxShader *Build() override;
        const VkPipelineShaderStageCreateInfo &GetPipelineShaderStageInfo() const;

        VkPipelineLayout GetPipelineLayout() const;
        void Flush();
        std::vector<VkDescriptorSet> &GetDescriptorSetList();

    private:
        void DumpDescriptorBindings();
        void DumpDescriptorSetLayouts();
        void DumpDescriptorWrites();

        VkShaderStageFlagBits GetShaderStageFlag();

        std::unique_ptr<GfxVulkanShaderCommon> mShaderCommon;

        std::unique_ptr<GfxVulkanShaderModule> mShaderModule;
    };
}