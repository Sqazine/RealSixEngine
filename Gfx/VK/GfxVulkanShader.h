#pragma once
#if _WIN32
#pragma comment(lib, "ucrtd.lib")
#endif
#include <vulkan/vulkan.h>
#include <string_view>
#include <vector>
#include <spirv_reflect.h>
#include <unordered_map>
#include "Gfx/IGfxShader.h"
#include "GfxVulkanObject.h"

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

    class GfxVulkanShader : public GfxVulkanObject, public IGfxShader
    {
    public:
        GfxVulkanShader(IGfxDevice *device);
        virtual ~GfxVulkanShader();

        VkShaderModule CreateShaderModule(const uint8_t *content, size_t contentSize);

        std::vector<VkDescriptorSetLayoutBinding> GetDescriptorLayoutBindingList();
        const std::vector<VkDescriptorSetLayout> &GetDescriptorSetLayoutList() const;
        const std::vector<VkDescriptorSet> &GetDescriptorSets();
        VkPipelineLayout GetPipelineLayout() const;

        IGfxShader *BindBuffer(std::string_view name, const IGfxBuffer *buffer) override;
        IGfxShader *BindTexture(std::string_view name, const IGfxTexture *texture) override;

        void Flush();

    protected:
        void CreateDescriptorPool();
        void AllocateDescriptorSets();
        void CreatePipelineLayout();

        const VkDescriptorPool &GetDescriptorPool() const;

        std::vector<VkWriteDescriptorSet> GetWrites();
        bool CheckDescriptorWriteValid();

        SpirvReflectedData SpirvReflect(SpvReflectShaderModule &spvModule, const uint8_t *spvCode, size_t spvCodeSize);

        void MarkDirty();

        bool mIsDirty{true};

        std::unordered_map<std::string_view, VkDescriptorSetLayoutBinding> mBindings;
        std::unordered_map<std::string_view, VkWriteDescriptorSet> mWrites;

        std::vector<VkDescriptorSetLayout> mDescriptorSetLayouts;
        std::vector<VkDescriptorSet> mDescriptorSets;
        VkDescriptorPool mDescriptorPool{VK_NULL_HANDLE};

        VkPipelineLayout mPipelineLayout{VK_NULL_HANDLE};

        std::unordered_map<std::string_view, VkDescriptorBufferInfo> mBufferInfos;
        std::unordered_map<std::string_view, VkDescriptorImageInfo> mImageInfos;
    };

    class GfxVulkanRasterShader : public GfxVulkanShader
    {
    public:
        GfxVulkanRasterShader(IGfxDevice *device,
                              const std::vector<uint8_t> &vertContent,
                              const std::vector<uint8_t> &fragContent,
                              const std::vector<uint8_t> &tessCtrlContent,
                              const std::vector<uint8_t> &tessEvalContent,
                              const std::vector<uint8_t> &geomContent);
        ~GfxVulkanRasterShader() override;

        const std::vector<VkPipelineShaderStageCreateInfo> &GetPipelineShaderStageInfoList() const;

    private:
        void CreateFromContents(const uint8_t *vertContent, size_t vertContentSize,
                                const uint8_t *fragContent, size_t fragContentSize,
                                const uint8_t *tessCtrlContent, size_t tessCtrlContentSize,
                                const uint8_t *tessEvalContent, size_t tessEvalContentSize,
                                const uint8_t *geomContent, size_t geomContentSize);
        void DumpDescriptorBindings();
        void DumpDescriptorSetLayouts();
        void DumpDescriptorWrites();

        VkShaderStageFlagBits GetShaderStageFlag(size_t idx);

        std::vector<VkPipelineShaderStageCreateInfo> mStageCreateInfos{};

        SpvReflectShaderModule mSpvModule[5]{};
        SpirvReflectedData mReflectedData[5]{};

        VkShaderModule mShaderModule[5]{};
    };

    class GfxVulkanComputeShader : public GfxVulkanShader
    {
    public:
        GfxVulkanComputeShader(IGfxDevice *device,
                               const std::vector<uint8_t> &compContent);
        ~GfxVulkanComputeShader() override;

        VkPipelineShaderStageCreateInfo GetPipelineShaderStageInfo() const;

    private:
        void CreateFromContent(const uint8_t *compContent, size_t compContentSize);
        void DumpDescriptorBindings();
        void DumpDescriptorSetLayouts();
        void DumpDescriptorWrites();

        VkShaderStageFlagBits GetShaderStageFlag();

        VkPipelineShaderStageCreateInfo mStageCreateInfo{};

        SpvReflectShaderModule mSpvModule{};
        SpirvReflectedData mReflectedData{};

        VkShaderModule mShaderModule{VK_NULL_HANDLE};
    };
}