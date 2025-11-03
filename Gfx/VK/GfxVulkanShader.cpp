#include "GfxVulkanShader.h"
#include "GfxVulkanDevice.h"
#include "GfxVulkanCommon.h"
#include "GfxVulkanBuffer.h"
#include "GfxVulkanTexture.h"
#include <cassert>
#include "Core/Common.h"
namespace RealSix
{
    GfxVulkanShader::GfxVulkanShader(IGfxDevice *device)
        : GfxVulkanObject(device)
    {
    }

    GfxVulkanShader::~GfxVulkanShader()
    {
        VkDevice device = mDevice->GetLogicDevice();

        vkDestroyPipelineLayout(device, mPipelineLayout, nullptr);

        for (auto &setLayout : mDescriptorSetLayouts)
        {
            vkDestroyDescriptorSetLayout(device, setLayout, nullptr);
        }

        vkDestroyDescriptorPool(device, mDescriptorPool, nullptr);
    }

    VkShaderModule GfxVulkanShader::CreateShaderModule(const uint8_t *content, size_t contentSize)
    {
        VkShaderModuleCreateInfo createInfo;
        ZeroVulkanStruct(createInfo, VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO);
        createInfo.codeSize = contentSize;
        createInfo.pCode = reinterpret_cast<const uint32_t *>(content);

        VkShaderModule shaderModule;
        VK_CHECK(vkCreateShaderModule(mDevice->GetLogicDevice(), &createInfo, nullptr, &shaderModule));

        return shaderModule;
    }

    void GfxVulkanShader::CreatePipelineLayout()
    {
        auto layouts = GetDescriptorSetLayoutList();

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        ZeroVulkanStruct(pipelineLayoutInfo, VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO);
        pipelineLayoutInfo.setLayoutCount = layouts.size();
        pipelineLayoutInfo.pSetLayouts = layouts.empty() ? nullptr : layouts.data();

        VkDevice device = mDevice->GetLogicDevice();

        VK_CHECK(vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &mPipelineLayout));
    }

    std::vector<VkWriteDescriptorSet> GfxVulkanShader::GetWrites()
    {
        std::vector<VkWriteDescriptorSet> result;
        for (auto [k, v] : mWrites)
        {
            result.emplace_back(v);
        }

        return result;
    }

    bool GfxVulkanShader::CheckDescriptorWriteValid()
    {
        for (const auto &write : mWrites)
        {
            if (write.second.pBufferInfo == nullptr && write.second.pImageInfo == nullptr)
            {
#ifdef REALSIX_UTF8_ENCODE
                STRING utf8Name = Utf8::Decode(write.first.data());
                REALSIX_LOG_WARN(TEXT("Descriptor write for binding {} is not bound!"), utf8Name);
#else
                REALSIX_LOG_WARN(TEXT("Descriptor write for binding {} is not bound!"), write.first);
#endif
                return false;
            }
        }
        return true;
    }

    void GfxVulkanShader::MarkDirty()
    {
        mIsDirty = true;
    }

    std::vector<VkDescriptorSetLayoutBinding> GfxVulkanShader::GetDescriptorLayoutBindingList()
    {
        std::vector<VkDescriptorSetLayoutBinding> result;
        for (auto [k, v] : mBindings)
        {
            result.emplace_back(v);
        }

        return result;
    }

    const std::vector<VkDescriptorSetLayout> &GfxVulkanShader::GetDescriptorSetLayoutList() const
    {
        return mDescriptorSetLayouts;
    }

    const VkDescriptorPool &GfxVulkanShader::GetDescriptorPool() const
    {
        return mDescriptorPool;
    }

    const std::vector<VkDescriptorSet> &GfxVulkanShader::GetDescriptorSets()
    {
        return mDescriptorSets;
    }

    VkPipelineLayout GfxVulkanShader::GetPipelineLayout() const
    {
        return mPipelineLayout;
    }

    IGfxShader *GfxVulkanShader::BindBuffer(std::string_view name, const IGfxBuffer *buffer)
    {
        MarkDirty();

        if (mWrites.find(name) == mWrites.end())
        {
#ifdef REALSIX_UTF8_ENCODE
            STRING utf8Name = Utf8::Decode(name.data());
            REALSIX_LOG_WARN(TEXT("Cannot find buffer binding named: {}"), utf8Name);
#else
            REALSIX_LOG_WARN(TEXT("Cannot find buffer binding named: {}"), name);
#endif
            return this;
        }

        auto rawVulkanBuffer = static_cast<const GfxVulkanBuffer *>(buffer);

        mBufferInfos[name].buffer = rawVulkanBuffer->GetHandle();
        mBufferInfos[name].offset = 0;
        mBufferInfos[name].range = rawVulkanBuffer->GetSize();

        mWrites[name].pBufferInfo = &mBufferInfos[name];

        return this;
    }

    IGfxShader *GfxVulkanShader::BindTexture(std::string_view name, const IGfxTexture *texture)
    {
        MarkDirty();

        auto rawVulkanTexture = static_cast<const GfxVulkanTexture *>(texture);

        if (mWrites.find(name) == mWrites.end())
        {
#ifdef REALSIX_UTF8_ENCODE
            STRING utf8Name = Utf8::Decode(name.data());
            REALSIX_LOG_WARN(TEXT("Cannot find texture binding named: {}"), utf8Name);
#else
            REALSIX_LOG_WARN(TEXT("Cannot find texture binding named: {}"), name);
#endif
            return this;
        }

        mImageInfos[name].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        mImageInfos[name].imageView = rawVulkanTexture->GetView();
        mImageInfos[name].sampler = rawVulkanTexture->GetSampler();

        mWrites[name].pImageInfo = &mImageInfos[name];
        return this;
    }

    void GfxVulkanShader::Flush()
    {
        if (mIsDirty)
        {
            if (CheckDescriptorWriteValid())
            {
                auto writeList = GetWrites();
                vkUpdateDescriptorSets(mDevice->GetLogicDevice(), static_cast<uint32_t>(writeList.size()), writeList.data(), 0, nullptr);
            }
            mIsDirty = false;
        }
    }

    void GfxVulkanShader::CreateDescriptorPool()
    {
        if (mDescriptorSetLayouts.empty())
            return;

        std::vector<VkDescriptorPoolSize> poolSizes;

        for (auto &[k, v] : mBindings)
        {
            bool alreadyExists = false;
            for (auto &poolSize : poolSizes)
            {
                if (v.descriptorType == poolSize.type)
                {
                    poolSize.descriptorCount++;
                    alreadyExists = true;
                    break;
                }
            }

            if (!alreadyExists)
            {
                VkDescriptorPoolSize newPoolSize;
                newPoolSize.type = v.descriptorType;
                newPoolSize.descriptorCount = 1;
                poolSizes.emplace_back(newPoolSize);
            }
        }

        VkDescriptorPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
        poolInfo.pPoolSizes = poolSizes.data();
        poolInfo.maxSets = static_cast<uint32_t>(mDescriptorSetLayouts.size());

        VK_CHECK(vkCreateDescriptorPool(mDevice->GetLogicDevice(), &poolInfo, nullptr, &mDescriptorPool));
    }

    void GfxVulkanShader::AllocateDescriptorSets()
    {
        mDescriptorSets.resize(mDescriptorSetLayouts.size());
        for (size_t i = 0; i < mDescriptorSetLayouts.size(); ++i)
        {
            if (mDescriptorSetLayouts[i] == nullptr)
                continue;

            VkDescriptorSetAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
            allocInfo.descriptorPool = GetDescriptorPool();
            allocInfo.descriptorSetCount = 1;
            allocInfo.pSetLayouts = &mDescriptorSetLayouts[i];

            VK_CHECK(vkAllocateDescriptorSets(mDevice->GetLogicDevice(), &allocInfo, &mDescriptorSets[i]));
        }
    }

    SpirvReflectedData GfxVulkanShader::SpirvReflect(SpvReflectShaderModule &spvModule, const uint8_t *spvCode, size_t spvCodeSize)
    {
#define SPIRV_REFLECT_CHECK(v)                   \
    do                                           \
    {                                            \
        assert(v == SPV_REFLECT_RESULT_SUCCESS); \
    } while (false)

        SpirvReflectedData result;

        SPIRV_REFLECT_CHECK(spvReflectCreateShaderModule(spvCodeSize, (const void *)spvCode, &spvModule));

        uint32_t varCount = 0;
        SPIRV_REFLECT_CHECK(spvReflectEnumerateInputVariables(&spvModule, &varCount, nullptr));
        result.inputVariables.resize(varCount);
        SPIRV_REFLECT_CHECK(spvReflectEnumerateInputVariables(&spvModule, &varCount, result.inputVariables.data()));

        varCount = 0;
        SPIRV_REFLECT_CHECK(spvReflectEnumerateOutputVariables(&spvModule, &varCount, nullptr));
        result.ouputVariables.resize(varCount);
        SPIRV_REFLECT_CHECK(spvReflectEnumerateOutputVariables(&spvModule, &varCount, result.ouputVariables.data()));

        varCount = 0;
        SPIRV_REFLECT_CHECK(spvReflectEnumerateDescriptorBindings(&spvModule, &varCount, nullptr));
        result.descriptorBindings.resize(varCount);
        SPIRV_REFLECT_CHECK(spvReflectEnumerateDescriptorBindings(&spvModule, &varCount, result.descriptorBindings.data()));

        varCount = 0;
        SPIRV_REFLECT_CHECK(spvReflectEnumerateDescriptorSets(&spvModule, &varCount, nullptr));
        result.descriptorSets.resize(varCount);
        SPIRV_REFLECT_CHECK(spvReflectEnumerateDescriptorSets(&spvModule, &varCount, result.descriptorSets.data()));

        varCount = 0;
        SPIRV_REFLECT_CHECK(spvReflectEnumeratePushConstantBlocks(&spvModule, &varCount, nullptr));
        result.pushConstants.resize(varCount);
        SPIRV_REFLECT_CHECK(spvReflectEnumeratePushConstantBlocks(&spvModule, &varCount, result.pushConstants.data()));

        return result;
    }

    GfxVulkanRasterShader::GfxVulkanRasterShader(IGfxDevice *device,
                                                 const std::vector<uint8_t> &vertContent,
                                                 const std::vector<uint8_t> &fragContent,
                                                 const std::vector<uint8_t> &tessCtrlContent,
                                                 const std::vector<uint8_t> &tessEvalContent,
                                                 const std::vector<uint8_t> &geomContent)
        : GfxVulkanShader(device)
    {
        for (int i = 0; i < 5; i++)
        {
            mShaderModule[i] = VK_NULL_HANDLE;
            mSpvModule[i] = {};
        }

        CreateFromContents(vertContent.data(), vertContent.size(),
                           fragContent.data(), fragContent.size(),
                           tessCtrlContent.data(), tessCtrlContent.size(),
                           tessEvalContent.data(), tessEvalContent.size(),
                           geomContent.data(), geomContent.size());
    }

    GfxVulkanRasterShader::~GfxVulkanRasterShader()
    {
        VkDevice device = mDevice->GetLogicDevice();

        for (int i = 0; i < 5; i++)
        {
            if (mSpvModule[i].entry_point_name != nullptr)
                spvReflectDestroyShaderModule(&mSpvModule[i]);

            if (mShaderModule[i] != VK_NULL_HANDLE)
            {
                vkDestroyShaderModule(device, mShaderModule[i], nullptr);
                mShaderModule[i] = VK_NULL_HANDLE;
            }
        }
        for (auto &info : mStageCreateInfos)
            ZeroVulkanStruct(info, VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO);
    }

    void GfxVulkanRasterShader::CreateFromContents(const uint8_t *vertContent, size_t vertContentSize,
                                                   const uint8_t *fragContent, size_t fragContentSize,
                                                   const uint8_t *tessCtrlContent, size_t tessCtrlContentSize,
                                                   const uint8_t *tessEvalContent, size_t tessEvalContentSize,
                                                   const uint8_t *geomContent, size_t geomContentSize)
    {
        mStageCreateInfos.clear();

        VkPipelineShaderStageCreateInfo shaderStageCreateInfo;
        ZeroVulkanStruct(shaderStageCreateInfo, VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO);

        mShaderModule[0] = CreateShaderModule(vertContent, vertContentSize);
        mReflectedData[0] = SpirvReflect(mSpvModule[0], vertContent, vertContentSize);
        shaderStageCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        shaderStageCreateInfo.module = mShaderModule[0];
        shaderStageCreateInfo.pName = "main";
        mStageCreateInfos.emplace_back(shaderStageCreateInfo);

        mShaderModule[1] = CreateShaderModule(fragContent, fragContentSize);
        mReflectedData[1] = SpirvReflect(mSpvModule[1], fragContent, fragContentSize);
        shaderStageCreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        shaderStageCreateInfo.module = mShaderModule[1];
        shaderStageCreateInfo.pName = "main";
        mStageCreateInfos.emplace_back(shaderStageCreateInfo);

        if (tessCtrlContentSize > 0)
        {
            mShaderModule[2] = CreateShaderModule(tessCtrlContent, tessCtrlContentSize);
            mReflectedData[2] = SpirvReflect(mSpvModule[2], tessCtrlContent, tessCtrlContentSize);
            shaderStageCreateInfo.stage = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
            shaderStageCreateInfo.module = mShaderModule[2];
            shaderStageCreateInfo.pName = "main";
            mStageCreateInfos.emplace_back(shaderStageCreateInfo);
        }

        if (tessEvalContentSize > 0)
        {

            mShaderModule[3] = CreateShaderModule(tessEvalContent, tessEvalContentSize);
            mReflectedData[3] = SpirvReflect(mSpvModule[3], tessEvalContent, tessEvalContentSize);
            shaderStageCreateInfo.stage = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
            shaderStageCreateInfo.module = mShaderModule[3];
            shaderStageCreateInfo.pName = "main";
            mStageCreateInfos.emplace_back(shaderStageCreateInfo);
        }

        if (geomContentSize > 0)
        {
            mShaderModule[4] = CreateShaderModule(geomContent, geomContentSize);
            mReflectedData[4] = SpirvReflect(mSpvModule[4], geomContent, geomContentSize);
            shaderStageCreateInfo.stage = VK_SHADER_STAGE_GEOMETRY_BIT;
            shaderStageCreateInfo.module = mShaderModule[4];
            shaderStageCreateInfo.pName = "main";
            mStageCreateInfos.emplace_back(shaderStageCreateInfo);
        }

        DumpDescriptorBindings();
        DumpDescriptorSetLayouts();
        CreateDescriptorPool();
        AllocateDescriptorSets();
        DumpDescriptorWrites();
        CreatePipelineLayout();
    }

    const std::vector<VkPipelineShaderStageCreateInfo> &GfxVulkanRasterShader::GetPipelineShaderStageInfoList() const
    {
        return mStageCreateInfos;
    }

    void GfxVulkanRasterShader::DumpDescriptorBindings()
    {
        for (size_t i = 0; i < 5; ++i)
        {
            for (auto &spvBinding : mReflectedData[i].descriptorBindings)
            {
                for (auto &vkBinding : mBindings)
                {
                    if (vkBinding.first == spvBinding->name)
                    {
                        vkBinding.second.stageFlags = GetShaderStageFlag(i);
                    }
                }
                VkDescriptorSetLayoutBinding layoutBinding{};
                layoutBinding.binding = spvBinding->binding;
                layoutBinding.descriptorCount = spvBinding->count;
                layoutBinding.descriptorType = (VkDescriptorType)spvBinding->descriptor_type;
                layoutBinding.pImmutableSamplers = nullptr;
                layoutBinding.stageFlags = GetShaderStageFlag(i);
                mBindings[spvBinding->name] = layoutBinding;
            }
        }
    }

    void GfxVulkanRasterShader::DumpDescriptorSetLayouts()
    {
        size_t maxCount = 0;
        size_t descriptorSetSize = 0;
        for (size_t i = 0; i < 5; ++i)
        {
            descriptorSetSize += mReflectedData[i].descriptorSets.size();
            for (auto &spvSet : mReflectedData[i].descriptorSets)
            {
                if (maxCount < spvSet->set)
                {
                    maxCount = spvSet->set;
                }
            }
        }

        if (descriptorSetSize == 0)
            return;

        mDescriptorSetLayouts.resize(maxCount + 1);

        auto GetDescriptorBinding = [&](std::string_view name)
        {
            for (auto &[k, v] : mBindings)
            {
                if (k == name)
                    return v;
            }

            return VkDescriptorSetLayoutBinding{};
        };

        for (size_t i = 0; i < 5; ++i)
        {
            for (auto &spvSet : mReflectedData[i].descriptorSets)
            {
                auto setSlot = spvSet->set;

                std::vector<VkDescriptorSetLayoutBinding> vkBindings(spvSet->binding_count);

                for (size_t i = 0; i < spvSet->binding_count; ++i)
                {
                    vkBindings[i] = GetDescriptorBinding(spvSet->bindings[i]->name);
                }

                VkDescriptorSetLayoutCreateInfo layoutInfo{};
                layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
                layoutInfo.bindingCount = static_cast<uint32_t>(vkBindings.size());
                layoutInfo.pBindings = vkBindings.data();

                VK_CHECK(vkCreateDescriptorSetLayout(mDevice->GetLogicDevice(), &layoutInfo, nullptr, &mDescriptorSetLayouts[setSlot]));
            }
        }
    }

    void GfxVulkanRasterShader::DumpDescriptorWrites()
    {
        auto GetSetIndex = [&](std::string_view name) -> uint32_t
        {
            for (size_t i = 0; i < 5; ++i)
            {
                for (const auto &spvBinding : mReflectedData[i].descriptorBindings)
                {
                    if (name == spvBinding->name)
                    {
                        return spvBinding->set;
                    }
                }
            }
            return 4096;
        };

        for (const auto &[k, v] : mBindings)
        {
            VkWriteDescriptorSet write = {};
            ZeroVulkanStruct(write, VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET);
            write.dstSet = mDescriptorSets[GetSetIndex(k)];
            write.dstBinding = v.binding;
            write.dstArrayElement = 0;
            write.descriptorType = v.descriptorType;
            write.descriptorCount = v.descriptorCount;

            mWrites[k] = write;
        }
    }

    VkShaderStageFlagBits GfxVulkanRasterShader::GetShaderStageFlag(size_t idx)
    {
        switch (idx)
        {
        case 0:
            return VK_SHADER_STAGE_VERTEX_BIT;
        case 1:
            return VK_SHADER_STAGE_FRAGMENT_BIT;
        case 2:
            return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
        case 3:
            return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
        case 4:
            return VK_SHADER_STAGE_GEOMETRY_BIT;
        }

        return VK_SHADER_STAGE_ALL; // for avoiding compiler warning
    }

    GfxVulkanComputeShader::GfxVulkanComputeShader(IGfxDevice *device,
                                                   const std::vector<uint8_t> &compContent)
        : GfxVulkanShader(device)
    {
        CreateFromContent(compContent.data(), compContent.size());
    }

    GfxVulkanComputeShader::~GfxVulkanComputeShader()
    {
        VkDevice device = mDevice->GetLogicDevice();

        if (mSpvModule.entry_point_name != nullptr)
            spvReflectDestroyShaderModule(&mSpvModule);

        if (mShaderModule != VK_NULL_HANDLE)
        {
            vkDestroyShaderModule(device, mShaderModule, nullptr);
            mShaderModule = VK_NULL_HANDLE;
        }

        ZeroVulkanStruct(mStageCreateInfo, VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO);
    }

    VkPipelineShaderStageCreateInfo GfxVulkanComputeShader::GetPipelineShaderStageInfo() const
    {
        return mStageCreateInfo;
    }

    void GfxVulkanComputeShader::CreateFromContent(const uint8_t *compContent, size_t compContentSize)
    {
        VkPipelineShaderStageCreateInfo shaderStageCreateInfo;
        ZeroVulkanStruct(shaderStageCreateInfo, VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO);

        mShaderModule = CreateShaderModule(compContent, compContentSize);
        mReflectedData = SpirvReflect(mSpvModule, compContent, compContentSize);

        ZeroVulkanStruct(mStageCreateInfo, VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO);
        mStageCreateInfo.stage = VK_SHADER_STAGE_COMPUTE_BIT;
        mStageCreateInfo.module = mShaderModule;
        mStageCreateInfo.pName = "main";

        DumpDescriptorBindings();
        DumpDescriptorSetLayouts();
        CreateDescriptorPool();
        AllocateDescriptorSets();
        DumpDescriptorWrites();
        CreatePipelineLayout();
    }

    void GfxVulkanComputeShader::DumpDescriptorBindings()
    {

        for (auto &spvBinding : mReflectedData.descriptorBindings)
        {
            for (auto &vkBinding : mBindings)
            {
                if (vkBinding.first == spvBinding->name)
                {
                    vkBinding.second.stageFlags = GetShaderStageFlag();
                }
            }
            VkDescriptorSetLayoutBinding layoutBinding{};
            layoutBinding.binding = spvBinding->binding;
            layoutBinding.descriptorCount = spvBinding->count;
            layoutBinding.descriptorType = (VkDescriptorType)spvBinding->descriptor_type;
            layoutBinding.pImmutableSamplers = nullptr;
            layoutBinding.stageFlags = GetShaderStageFlag();
            mBindings[spvBinding->name] = layoutBinding;
        }
    }
    void GfxVulkanComputeShader::DumpDescriptorSetLayouts()
    {
        size_t maxCount = 0;
        size_t descriptorSetSize = mReflectedData.descriptorSets.size();
        for (auto &spvSet : mReflectedData.descriptorSets)
        {
            if (maxCount < spvSet->set)
            {
                maxCount = spvSet->set;
            }
        }

        if (descriptorSetSize == 0)
            return;

        mDescriptorSetLayouts.resize(maxCount + 1);

        auto GetDescriptorBinding = [&](std::string_view name)
        {
            for (auto &[k, v] : mBindings)
            {
                if (k == name)
                    return v;
            }

            return VkDescriptorSetLayoutBinding{};
        };

        for (auto &spvSet : mReflectedData.descriptorSets)
        {
            auto setSlot = spvSet->set;

            std::vector<VkDescriptorSetLayoutBinding> vkBindings(spvSet->binding_count);

            for (size_t i = 0; i < spvSet->binding_count; ++i)
            {
                vkBindings[i] = GetDescriptorBinding(spvSet->bindings[i]->name);
            }

            VkDescriptorSetLayoutCreateInfo layoutInfo{};
            layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            layoutInfo.bindingCount = static_cast<uint32_t>(vkBindings.size());
            layoutInfo.pBindings = vkBindings.data();

            VK_CHECK(vkCreateDescriptorSetLayout(mDevice->GetLogicDevice(), &layoutInfo, nullptr, &mDescriptorSetLayouts[setSlot]));
        }
    }
    void GfxVulkanComputeShader::DumpDescriptorWrites()
    {
        auto GetSetIndex = [&](std::string_view name) -> uint32_t
        {
            for (const auto &spvBinding : mReflectedData.descriptorBindings)
            {
                if (name == spvBinding->name)
                {
                    return spvBinding->set;
                }
            }

            return 4096;
        };

        for (const auto &[k, v] : mBindings)
        {
            VkWriteDescriptorSet write = {};
            ZeroVulkanStruct(write, VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET);
            write.dstSet = mDescriptorSets[GetSetIndex(k)];
            write.dstBinding = v.binding;
            write.dstArrayElement = 0;
            write.descriptorType = v.descriptorType;
            write.descriptorCount = v.descriptorCount;

            mWrites[k] = write;
        }
    }

    VkShaderStageFlagBits GfxVulkanComputeShader::GetShaderStageFlag()
    {
        return VK_SHADER_STAGE_COMPUTE_BIT;
    }
}