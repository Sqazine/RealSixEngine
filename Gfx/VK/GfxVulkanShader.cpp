#include "GfxVulkanShader.hpp"
#include "GfxVulkanDevice.hpp"
#include "GfxVulkanUtils.hpp"
#include "GfxVulkanBuffer.hpp"
#include "GfxVulkanTexture.hpp"
#include "String.hpp"
#include "Resource/FileSystem.hpp"
#include "Resource/FileSystem.hpp"
namespace RealSix
{
    GfxVulkanShaderModule::GfxVulkanShaderModule(IGfxDevice *device, StringView content, StringView mainEntry, const std::vector<StringView> &marcos)
        : GfxVulkanObject(device), mMainEntry(mainEntry)
    {
        auto binaryContent = Compile(content, mainEntry, marcos);

        mShaderModule = CreateShaderModule(binaryContent);
        mReflectedData = SpirvReflect(mSpvModule, binaryContent);

        ZeroVulkanStruct(mStageCreateInfo, VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO);
        mStageCreateInfo.stage = static_cast<VkShaderStageFlagBits>(mSpvModule.shader_stage);
        mStageCreateInfo.module = mShaderModule;
        mStageCreateInfo.pName = mMainEntry.CString();
    }

    GfxVulkanShaderModule::~GfxVulkanShaderModule()
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

    VkShaderModule GfxVulkanShaderModule::CreateShaderModule(StringView content)
    {
        VkShaderModuleCreateInfo createInfo;
        ZeroVulkanStruct(createInfo, VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO);
        createInfo.codeSize = content.Size();
        createInfo.pCode = reinterpret_cast<const uint32_t *>(content.CString());

        VkShaderModule shaderModule;
        VK_CHECK(vkCreateShaderModule(mDevice->GetLogicDevice(), &createInfo, nullptr, &shaderModule));

        return shaderModule;
    }

    SpirvReflectedData GfxVulkanShaderModule::SpirvReflect(SpvReflectShaderModule &spvModule, StringView content)
    {
#define SPIRV_REFLECT_CHECK(v)                                                       \
    do                                                                               \
    {                                                                                \
        REALSIX_CHECK(v == SPV_REFLECT_RESULT_SUCCESS, "Spirv reflect check error"); \
    } while (false)

        SpirvReflectedData result;

        SPIRV_REFLECT_CHECK(spvReflectCreateShaderModule(content.Size(), (const void *)content.CString(), &spvModule));

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

    String GfxVulkanShaderModule::Compile(StringView content, StringView mainEntry, const std::vector<StringView> &marcos)
    {
        // TODO: need to be optimize
        String fullContent = "/*MainEntry:" + String(mainEntry.CString()) + "*/\n"; // head

        for (const auto &marco : marcos)
        {
            fullContent += "#define " + String(marco) + "\n";
        }

        fullContent += content;

        mSourceCodeHash = fullContent.GetHash();

        if (!FileSystem::Exists("ShaderSource"))
            FileSystem::CreateDirectory("ShaderSource");

        String path = "ShaderSource/" + std::to_string(mSourceCodeHash) + ".slang";

        FileSystem::WriteBinaryFile(path, fullContent);

        if (!FileSystem::Exists("ShaderBinary"))
            FileSystem::CreateDirectory("ShaderBinary");

        String destPath = "ShaderBinary/" + std::to_string(mSourceCodeHash) + ".slang.spv";

        auto shaderCompile = "slangc.exe " + path +
                             " -profile sm_6_6+spirv_1_6 -target spirv -o " + destPath +
                             " -emit-spirv-directly -fvk-use-entrypoint-name";
        system(shaderCompile.CString());

        return FileSystem::ReadBinaryFile(destPath);
    }

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
                REALSIX_LOG_WARN("Descriptor write for binding {} is not bound!", write.first);
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

    void GfxVulkanShader::BindBufferImpl(StringView name, const IGfxBuffer *buffer)
    {
        MarkDirty();

        if (mWrites.find(name) == mWrites.end())
        {
            REALSIX_LOG_WARN("Cannot find buffer binding named: {}", name);
        }

        auto rawVulkanBuffer = static_cast<const GfxVulkanBuffer *>(buffer);

        mBufferInfos[name].buffer = rawVulkanBuffer->GetHandle();
        mBufferInfos[name].offset = 0;
        mBufferInfos[name].range = rawVulkanBuffer->GetSize();

        mWrites[name].pBufferInfo = &mBufferInfos[name];
    }

    void GfxVulkanShader::BindTextureImpl(StringView name, const IGfxTexture *texture)
    {
        MarkDirty();

        auto rawVulkanTexture = static_cast<const GfxVulkanTexture *>(texture);

        if (mWrites.find(name) == mWrites.end())
        {
            REALSIX_LOG_WARN("Cannot find texture binding named: {}", name);
        }

        mImageInfos[name].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        mImageInfos[name].imageView = rawVulkanTexture->GetView();
        mImageInfos[name].sampler = rawVulkanTexture->GetSampler();

        mWrites[name].pImageInfo = &mImageInfos[name];
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

    GfxVulkanRasterShader::GfxVulkanRasterShader(IGfxDevice *device)
        : GfxVulkanShader(device)
    {
    }

    GfxVulkanRasterShader::~GfxVulkanRasterShader()
    {
    }
    IGfxShader *GfxVulkanRasterShader::BindBuffer(StringView name, const IGfxBuffer *buffer)
    {
        BindBufferImpl(name, buffer);
        return this;
    }
    IGfxShader *GfxVulkanRasterShader::BindTexture(StringView name, const IGfxTexture *texture)
    {
        BindTextureImpl(name, texture);
        return this;
    }

    const std::vector<VkPipelineShaderStageCreateInfo> &GfxVulkanRasterShader::GetPipelineShaderStageInfoList() const
    {
        return mPipelineShaderStageCreateInfos;
    }

    IGfxShader *GfxVulkanRasterShader::Build()
    {
        mPipelineShaderStageCreateInfos.clear();

        if (!mShaderSources[RasterShaderSlot::Vertex].Empty())
        {
            mShaderModules[RasterShaderSlot::Vertex] = std::make_unique<GfxVulkanShaderModule>(mDevice, mShaderSources[RasterShaderSlot::Vertex], "main", mMarcos);
            mPipelineShaderStageCreateInfos.emplace_back(mShaderModules[RasterShaderSlot::Vertex]->GetPipelineShaderStageInfo());
        }

        if (!mShaderSources[RasterShaderSlot::Fragment].Empty())
        {
            mShaderModules[RasterShaderSlot::Fragment] = std::make_unique<GfxVulkanShaderModule>(mDevice, mShaderSources[RasterShaderSlot::Fragment], "main", mMarcos);
            mPipelineShaderStageCreateInfos.emplace_back(mShaderModules[RasterShaderSlot::Fragment]->GetPipelineShaderStageInfo());
        }

        if (!mShaderSources[RasterShaderSlot::TessellationControl].Empty())
        {
            mShaderModules[RasterShaderSlot::TessellationControl] = std::make_unique<GfxVulkanShaderModule>(mDevice, mShaderSources[RasterShaderSlot::TessellationControl], "main", mMarcos);
            mPipelineShaderStageCreateInfos.emplace_back(mShaderModules[RasterShaderSlot::TessellationControl]->GetPipelineShaderStageInfo());
        }

        if (!mShaderSources[RasterShaderSlot::TessellationEvaluation].Empty())
        {
            mShaderModules[RasterShaderSlot::TessellationEvaluation] = std::make_unique<GfxVulkanShaderModule>(mDevice, mShaderSources[RasterShaderSlot::TessellationEvaluation], "main", mMarcos);
            mPipelineShaderStageCreateInfos.emplace_back(mShaderModules[RasterShaderSlot::TessellationEvaluation]->GetPipelineShaderStageInfo());
        }

        if (!mShaderSources[RasterShaderSlot::Geometry].Empty())
        {
            mShaderModules[RasterShaderSlot::Geometry] = std::make_unique<GfxVulkanShaderModule>(mDevice, mShaderSources[RasterShaderSlot::Geometry], "main", mMarcos);
            mPipelineShaderStageCreateInfos.emplace_back(mShaderModules[RasterShaderSlot::Geometry]->GetPipelineShaderStageInfo());
        }

        DumpDescriptorBindings();
        DumpDescriptorSetLayouts();
        CreateDescriptorPool();
        AllocateDescriptorSets();
        DumpDescriptorWrites();
        CreatePipelineLayout();
        return this;
    }

    void GfxVulkanRasterShader::DumpDescriptorBindings()
    {
        for (size_t i = RasterShaderSlot::Vertex; i < RasterShaderSlot::Num; ++i)
        {
            if (!mShaderModules[i])
                continue;
            for (auto &spvBinding : mShaderModules[i]->GetSpvReflectData().descriptorBindings)
            {
                for (auto &vkBinding : mBindings)
                {
                    if (vkBinding.first == spvBinding->name)
                    {
                        vkBinding.second.stageFlags = GetShaderStageFlag((RasterShaderSlot)i);
                    }
                }
                VkDescriptorSetLayoutBinding layoutBinding{};
                layoutBinding.binding = spvBinding->binding;
                layoutBinding.descriptorCount = spvBinding->count;
                layoutBinding.descriptorType = (VkDescriptorType)spvBinding->descriptor_type;
                layoutBinding.pImmutableSamplers = nullptr;
                layoutBinding.stageFlags = GetShaderStageFlag((RasterShaderSlot)i);
                mBindings[spvBinding->name] = layoutBinding;
            }
        }
    }

    void GfxVulkanRasterShader::DumpDescriptorSetLayouts()
    {
        size_t maxCount = 0;
        size_t descriptorSetSize = 0;
        for (size_t i = RasterShaderSlot::Vertex; i < RasterShaderSlot::Num; ++i)
        {
            if (!mShaderModules[i])
                continue;
            descriptorSetSize += mShaderModules[i]->GetSpvReflectData().descriptorSets.size();
            for (auto &spvSet : mShaderModules[i]->GetSpvReflectData().descriptorSets)
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

        auto GetDescriptorBinding = [&](StringView name)
        {
            for (auto &[k, v] : mBindings)
            {
                if (k == name)
                    return v;
            }

            return VkDescriptorSetLayoutBinding{};
        };

        for (size_t i = RasterShaderSlot::Vertex; i < RasterShaderSlot::Num; ++i)
        {
            if (!mShaderModules[i])
                continue;
            for (auto &spvSet : mShaderModules[i]->GetSpvReflectData().descriptorSets)
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
        auto GetSetIndex = [&](StringView name) -> uint32_t
        {
            for (size_t i = RasterShaderSlot::Vertex; i < RasterShaderSlot::Num; ++i)
            {
                if (!mShaderModules[i])
                    continue;
                for (const auto &spvBinding : mShaderModules[i]->GetSpvReflectData().descriptorBindings)
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

    VkShaderStageFlagBits GfxVulkanRasterShader::GetShaderStageFlag(RasterShaderSlot slot)
    {
        switch (slot)
        {
        case RasterShaderSlot::Vertex:
            return VK_SHADER_STAGE_VERTEX_BIT;
        case RasterShaderSlot::Fragment:
            return VK_SHADER_STAGE_FRAGMENT_BIT;
        case RasterShaderSlot::TessellationControl:
            return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
        case RasterShaderSlot::TessellationEvaluation:
            return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
        case RasterShaderSlot::Geometry:
            return VK_SHADER_STAGE_GEOMETRY_BIT;
        }

        return VK_SHADER_STAGE_ALL; // for avoiding compiler warning
    }

    GfxVulkanComputeShader::GfxVulkanComputeShader(IGfxDevice *device)
        : GfxVulkanShader(device)
    {
    }

    GfxVulkanComputeShader::~GfxVulkanComputeShader()
    {
    }

    IGfxShader *GfxVulkanComputeShader::BindBuffer(StringView name, const IGfxBuffer *buffer)
    {
        BindBufferImpl(name, buffer);
        return this;
    }
    IGfxShader *GfxVulkanComputeShader::BindTexture(StringView name, const IGfxTexture *texture)
    {
        BindTextureImpl(name, texture);
        return this;
    }

    const VkPipelineShaderStageCreateInfo &GfxVulkanComputeShader::GetPipelineShaderStageInfo() const
    {
        return mShaderModule->GetPipelineShaderStageInfo();
    }

    IGfxShader *GfxVulkanComputeShader::Build()
    {
        if (!mShaderSource.Empty())
        {
            mShaderModule = std::make_unique<GfxVulkanShaderModule>(mDevice, mShaderSource, "main", mMarcos);
        }

        DumpDescriptorBindings();
        DumpDescriptorSetLayouts();
        CreateDescriptorPool();
        AllocateDescriptorSets();
        DumpDescriptorWrites();
        CreatePipelineLayout();

        return this;
    }

    void GfxVulkanComputeShader::DumpDescriptorBindings()
    {
        for (auto &spvBinding : mShaderModule->GetSpvReflectData().descriptorBindings)
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
        size_t descriptorSetSize = mShaderModule->GetSpvReflectData().descriptorSets.size();
        for (auto &spvSet : mShaderModule->GetSpvReflectData().descriptorSets)
        {
            if (maxCount < spvSet->set)
            {
                maxCount = spvSet->set;
            }
        }

        if (descriptorSetSize == 0)
            return;

        mDescriptorSetLayouts.resize(maxCount + 1);

        auto GetDescriptorBinding = [&](StringView name)
        {
            for (auto &[k, v] : mBindings)
            {
                if (k == name)
                    return v;
            }

            return VkDescriptorSetLayoutBinding{};
        };

        for (auto &spvSet : mShaderModule->GetSpvReflectData().descriptorSets)
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
        auto GetSetIndex = [&](StringView name) -> uint32_t
        {
            for (const auto &spvBinding : mShaderModule->GetSpvReflectData().descriptorBindings)
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