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
    struct SpirvReflectedData
    {
        std::vector<SpvReflectInterfaceVariable *> inputVariables;
        std::vector<SpvReflectInterfaceVariable *> ouputVariables;
        std::vector<SpvReflectBlockVariable *> pushConstants;
        std::vector<SpvReflectDescriptorSet *> descriptorSets;
        std::vector<SpvReflectDescriptorBinding *> descriptorBindings;
    };

    class GfxVulkanShaderModule : public GfxVulkanObject
    {
    public:
        GfxVulkanShaderModule(IGfxDevice *device, StringView content, StringView mainEntry = "main", const std::vector<StringView> &marcos = {});
        ~GfxVulkanShaderModule() override;

        const VkPipelineShaderStageCreateInfo &GetPipelineShaderStageInfo() const { return mStageCreateInfo; }

        const SpirvReflectedData &GetSpvReflectData() const { return mReflectedData; }

    private:
        String Compile(StringView content, StringView mainEntry, const std::vector<StringView> &marcos);

        VkShaderModule CreateShaderModule(StringView content);
        SpirvReflectedData SpirvReflect(SpvReflectShaderModule &spvModule, StringView content);

        size_t mSourceCodeHash;

        StringView mMainEntry;

        VkPipelineShaderStageCreateInfo mStageCreateInfo{};

        SpvReflectShaderModule mSpvModule{};
        SpirvReflectedData mReflectedData{};

        VkShaderModule mShaderModule{VK_NULL_HANDLE};
    };

    class GfxVulkanShader : public GfxVulkanObject
    {
    public:
        GfxVulkanShader(IGfxDevice *device);
        virtual ~GfxVulkanShader();

        std::vector<VkDescriptorSetLayoutBinding> GetDescriptorLayoutBindingList();
        const std::vector<VkDescriptorSetLayout> &GetDescriptorSetLayoutList() const;
        const std::vector<VkDescriptorSet> &GetDescriptorSets();
        VkPipelineLayout GetPipelineLayout() const;

        void Flush();

    protected:
        void BindBufferImpl(StringView name, const IGfxBuffer *buffer);
        void BindTextureImpl(StringView name, const IGfxTexture *texture);

        void CreateDescriptorPool();
        void AllocateDescriptorSets();
        void CreatePipelineLayout();

        const VkDescriptorPool &GetDescriptorPool() const;

        std::vector<VkWriteDescriptorSet> GetWrites();
        bool CheckDescriptorWriteValid();

        void MarkDirty();

        bool mIsDirty{true};

        std::unordered_map<StringView, VkDescriptorSetLayoutBinding> mBindings;
        std::unordered_map<StringView, VkWriteDescriptorSet> mWrites;

        std::vector<VkDescriptorSetLayout> mDescriptorSetLayouts;
        std::vector<VkDescriptorSet> mDescriptorSets;
        VkDescriptorPool mDescriptorPool{VK_NULL_HANDLE};

        VkPipelineLayout mPipelineLayout{VK_NULL_HANDLE};

        std::unordered_map<StringView, VkDescriptorBufferInfo> mBufferInfos;
        std::unordered_map<StringView, VkDescriptorImageInfo> mImageInfos;
    };

    class GfxVulkanRasterShader : public GfxVulkanShader, public IGfxRasterShader
    {
    public:
        GfxVulkanRasterShader(IGfxDevice *device);
        ~GfxVulkanRasterShader() override;

        const std::vector<VkPipelineShaderStageCreateInfo> &GetPipelineShaderStageInfoList() const;

        virtual IGfxShader *BindBuffer(StringView name, const IGfxBuffer *buffer) override;
        virtual IGfxShader *BindTexture(StringView name, const IGfxTexture *texture) override;
        virtual IGfxShader *Build() override;

    private:
        void DumpDescriptorBindings();
        void DumpDescriptorSetLayouts();
        void DumpDescriptorWrites();

        VkShaderStageFlagBits GetShaderStageFlag(RasterShaderSlot slot);

        std::vector<VkPipelineShaderStageCreateInfo> mPipelineShaderStageCreateInfos;

        std::array<std::unique_ptr<GfxVulkanShaderModule>, RasterShaderSlot::Num> mShaderModules;
    };

    class GfxVulkanComputeShader : public GfxVulkanShader, public IGfxComputeShader
    {
    public:
        GfxVulkanComputeShader(IGfxDevice *device);
        ~GfxVulkanComputeShader() override;

        virtual IGfxShader *BindBuffer(StringView name, const IGfxBuffer *buffer) override;
        virtual IGfxShader *BindTexture(StringView name, const IGfxTexture *texture) override;
        virtual IGfxShader *Build() override;
        const VkPipelineShaderStageCreateInfo &GetPipelineShaderStageInfo() const;

    private:
        void DumpDescriptorBindings();
        void DumpDescriptorSetLayouts();
        void DumpDescriptorWrites();

        VkShaderStageFlagBits GetShaderStageFlag();

        std::unique_ptr<GfxVulkanShaderModule> mShaderModule;
    };
}