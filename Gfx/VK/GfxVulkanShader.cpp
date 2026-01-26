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

    GfxVulkanShaderCommon::GfxVulkanShaderCommon(IGfxDevice *device)
        : GfxVulkanObject(device)
    {
    }

    GfxVulkanShaderCommon::~GfxVulkanShaderCommon()
    {
        VkDevice device = mDevice->GetLogicDevice();

        vkDestroyPipelineLayout(device, mPipelineLayout, nullptr);

        for (auto &setLayout : mDescriptorSetLayouts)
        {
            vkDestroyDescriptorSetLayout(device, setLayout, nullptr);
        }

        vkDestroyDescriptorPool(device, mDescriptorPool, nullptr);
    }

    void GfxVulkanShaderCommon::CreatePipelineLayout()
    {
        auto layouts = GetDescriptorSetLayoutList();

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        ZeroVulkanStruct(pipelineLayoutInfo, VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO);
        pipelineLayoutInfo.setLayoutCount = layouts.size();
        pipelineLayoutInfo.pSetLayouts = layouts.empty() ? nullptr : layouts.data();

        VkDevice device = mDevice->GetLogicDevice();

        VK_CHECK(vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &mPipelineLayout));
    }

    std::vector<VkWriteDescriptorSet> GfxVulkanShaderCommon::GetWriteList()
    {
        std::vector<VkWriteDescriptorSet> result;
        for (auto [k, v] : mWriteMap)
        {
            result.emplace_back(v);
        }

        return result;
    }

    void GfxVulkanShaderCommon::SetBinding(StringView name, VkDescriptorSetLayoutBinding binding)
    {
        mBindingMap[name] = binding;
    }

    bool GfxVulkanShaderCommon::CheckDescriptorWriteValid()
    {
        for (const auto &write : mWriteMap)
        {
            if (write.second.pBufferInfo == nullptr && write.second.pImageInfo == nullptr)
            {
                REALSIX_LOG_WARN("Descriptor write for binding {} is not bound!", write.first);
                return false;
            }
        }
        return true;
    }

    void GfxVulkanShaderCommon::MarkDirty()
    {
        mIsDirty = true;
    }

    std::vector<VkDescriptorSetLayoutBinding> GfxVulkanShaderCommon::GetDescriptorLayoutBindingList()
    {
        std::vector<VkDescriptorSetLayoutBinding> result;
        for (auto [k, v] : mBindingMap)
        {
            result.emplace_back(v);
        }

        return result;
    }

    std::vector<VkDescriptorSetLayout> &GfxVulkanShaderCommon::GetDescriptorSetLayoutList()
    {
        return mDescriptorSetLayouts;
    }

    const VkDescriptorPool &GfxVulkanShaderCommon::GetDescriptorPool() const
    {
        return mDescriptorPool;
    }

    std::vector<VkDescriptorSet> &GfxVulkanShaderCommon::GetDescriptorSetList()
    {
        return mDescriptorSets;
    }

    VkDescriptorSet GfxVulkanShaderCommon::GetDescriptorSet(uint8_t index) const
    {
        return mDescriptorSets[index];
    }

    VkPipelineLayout GfxVulkanShaderCommon::GetPipelineLayout() const
    {
        return mPipelineLayout;
    }

    void GfxVulkanShaderCommon::BindBufferImpl(StringView name, const IGfxBuffer *buffer)
    {
        MarkDirty();

        if (mWriteMap.find(name) == mWriteMap.end())
        {
            REALSIX_LOG_WARN("Cannot find buffer binding named: {}", name);
        }

        auto rawVulkanBuffer = static_cast<const GfxVulkanBuffer *>(buffer);

        mBufferInfos[name].buffer = rawVulkanBuffer->GetHandle();
        mBufferInfos[name].offset = 0;
        mBufferInfos[name].range = rawVulkanBuffer->GetSize();

        mWriteMap[name].pBufferInfo = &mBufferInfos[name];
    }

    void GfxVulkanShaderCommon::BindTextureImpl(StringView name, const IGfxTexture *texture)
    {
        MarkDirty();

        auto rawVulkanTexture = static_cast<const GfxVulkanTexture *>(texture);

        if (mWriteMap.find(name) == mWriteMap.end())
        {
            REALSIX_LOG_WARN("Cannot find texture binding named: {}", name);
        }

        mImageInfos[name].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        mImageInfos[name].imageView = rawVulkanTexture->GetView();
        mImageInfos[name].sampler = rawVulkanTexture->GetSampler();

        mWriteMap[name].pImageInfo = &mImageInfos[name];
    }

    void GfxVulkanShaderCommon::Flush()
    {
        if (mIsDirty)
        {
            if (CheckDescriptorWriteValid())
            {
                auto writeList = GetWriteList();
                vkUpdateDescriptorSets(mDevice->GetLogicDevice(), static_cast<uint32_t>(writeList.size()), writeList.data(), 0, nullptr);
            }
            mIsDirty = false;
        }
    }

    void GfxVulkanShaderCommon::CreateDescriptorPool()
    {
        if (mDescriptorSetLayouts.empty())
            return;

        std::vector<VkDescriptorPoolSize> poolSizes;

        for (auto &[k, v] : mBindingMap)
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

    void GfxVulkanShaderCommon::AllocateDescriptorSets()
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

    GfxVulkanVertexRasterShader::GfxVulkanVertexRasterShader(IGfxDevice *device)
        : GfxVulkanObject(device), mShaderCommon(std::make_unique<GfxVulkanShaderCommon>(device))
    {
    }

    GfxVulkanVertexRasterShader::~GfxVulkanVertexRasterShader()
    {
    }
    IGfxShader *GfxVulkanVertexRasterShader::BindBuffer(StringView name, const IGfxBuffer *buffer)
    {
        mShaderCommon->BindBufferImpl(name, buffer);
        return this;
    }
    IGfxShader *GfxVulkanVertexRasterShader::BindTexture(StringView name, const IGfxTexture *texture)
    {
        mShaderCommon->BindTextureImpl(name, texture);
        return this;
    }

    const std::vector<VkPipelineShaderStageCreateInfo> &GfxVulkanVertexRasterShader::GetPipelineShaderStageInfoList() const
    {
        return mPipelineShaderStageCreateInfos;
    }

    IGfxShader *GfxVulkanVertexRasterShader::Build()
    {
        mPipelineShaderStageCreateInfos.clear();

        if (!mShaderSources[static_cast<uint8_t>(Slot::Vertex)].Empty())
        {
            mShaderModules[static_cast<uint8_t>(Slot::Vertex)] = std::make_unique<GfxVulkanShaderModule>(mDevice, mShaderSources[static_cast<uint8_t>(Slot::Vertex)], "main", mMarcos);
            mPipelineShaderStageCreateInfos.emplace_back(mShaderModules[static_cast<uint8_t>(Slot::Vertex)]->GetPipelineShaderStageInfo());
        }

        if (!mShaderSources[static_cast<uint8_t>(Slot::Fragment)].Empty())
        {
            mShaderModules[static_cast<uint8_t>(Slot::Fragment)] = std::make_unique<GfxVulkanShaderModule>(mDevice, mShaderSources[static_cast<uint8_t>(Slot::Fragment)], "main", mMarcos);
            mPipelineShaderStageCreateInfos.emplace_back(mShaderModules[static_cast<uint8_t>(Slot::Fragment)]->GetPipelineShaderStageInfo());
        }

        if (!mShaderSources[static_cast<uint8_t>(Slot::TessellationControl)].Empty())
        {
            mShaderModules[static_cast<uint8_t>(Slot::TessellationControl)] = std::make_unique<GfxVulkanShaderModule>(mDevice, mShaderSources[static_cast<uint8_t>(Slot::TessellationControl)], "main", mMarcos);
            mPipelineShaderStageCreateInfos.emplace_back(mShaderModules[static_cast<uint8_t>(Slot::TessellationControl)]->GetPipelineShaderStageInfo());
        }

        if (!mShaderSources[static_cast<uint8_t>(Slot::TessellationEvaluation)].Empty())
        {
            mShaderModules[static_cast<uint8_t>(Slot::TessellationEvaluation)] = std::make_unique<GfxVulkanShaderModule>(mDevice, mShaderSources[static_cast<uint8_t>(Slot::TessellationEvaluation)], "main", mMarcos);
            mPipelineShaderStageCreateInfos.emplace_back(mShaderModules[static_cast<uint8_t>(Slot::TessellationEvaluation)]->GetPipelineShaderStageInfo());
        }

        if (!mShaderSources[static_cast<uint8_t>(Slot::Geometry)].Empty())
        {
            mShaderModules[static_cast<uint8_t>(Slot::Geometry)] = std::make_unique<GfxVulkanShaderModule>(mDevice, mShaderSources[static_cast<uint8_t>(Slot::Geometry)], "main", mMarcos);
            mPipelineShaderStageCreateInfos.emplace_back(mShaderModules[static_cast<uint8_t>(Slot::Geometry)]->GetPipelineShaderStageInfo());
        }

        DumpDescriptorBindings();
        DumpDescriptorSetLayouts();
        mShaderCommon->CreateDescriptorPool();
        mShaderCommon->AllocateDescriptorSets();
        DumpDescriptorWrites();
        mShaderCommon->CreatePipelineLayout();
        return this;
    }

    void GfxVulkanVertexRasterShader::DumpDescriptorBindings()
    {
        for (uint8_t i = static_cast<uint8_t>(Slot::Vertex); i < static_cast<uint8_t>(Slot::Num); ++i)
        {
            if (!mShaderModules[i])
                continue;
            for (auto &spvBinding : mShaderModules[i]->GetSpvReflectData().descriptorBindings)
            {
                for (auto &[k, v] : mShaderCommon->GetBindingMap())
                {
                    if (k == spvBinding->name)
                    {
                        v.stageFlags = GetShaderStageFlag((IGfxVertexRasterShader::Slot)i);
                    }
                }
                VkDescriptorSetLayoutBinding layoutBinding{};
                layoutBinding.binding = spvBinding->binding;
                layoutBinding.descriptorCount = spvBinding->count;
                layoutBinding.descriptorType = (VkDescriptorType)spvBinding->descriptor_type;
                layoutBinding.pImmutableSamplers = nullptr;
                layoutBinding.stageFlags = GetShaderStageFlag((IGfxVertexRasterShader::Slot)i);
                mShaderCommon->SetBinding(spvBinding->name, layoutBinding);
            }
        }
    }

    void GfxVulkanVertexRasterShader::DumpDescriptorSetLayouts()
    {
        size_t maxCount = 0;
        size_t descriptorSetSize = 0;
        for (uint8_t i = static_cast<uint8_t>(Slot::Vertex); i < static_cast<uint8_t>(Slot::Num); ++i)
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

        mShaderCommon->GetDescriptorSetLayoutList().resize(maxCount + 1);

        auto GetDescriptorBinding = [&](StringView name)
        {
            for (auto &[k, v] : mShaderCommon->GetBindingMap())
            {
                if (k == name)
                    return v;
            }

            return VkDescriptorSetLayoutBinding{};
        };

        for (uint8_t i = static_cast<uint8_t>(Slot::Vertex); i < static_cast<uint8_t>(Slot::Num); ++i)
        {
            if (!mShaderModules[i])
                continue;
            for (auto &spvSet : mShaderModules[i]->GetSpvReflectData().descriptorSets)
            {
                auto setSlot = spvSet->set;

                std::vector<VkDescriptorSetLayoutBinding> vkBindings(spvSet->binding_count);

                for (size_t j = 0; j < spvSet->binding_count; ++j)
                {
                    vkBindings[j] = GetDescriptorBinding(spvSet->bindings[j]->name);
                }

                VkDescriptorSetLayoutCreateInfo layoutInfo{};
                layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
                layoutInfo.bindingCount = static_cast<uint32_t>(vkBindings.size());
                layoutInfo.pBindings = vkBindings.data();

                VK_CHECK(vkCreateDescriptorSetLayout(mDevice->GetLogicDevice(), &layoutInfo, nullptr, &mShaderCommon->GetDescriptorSetLayoutList()[setSlot]));
            }
        }
    }

    void GfxVulkanVertexRasterShader::DumpDescriptorWrites()
    {
        auto GetSetIndex = [&](StringView name) -> uint32_t
        {
            for (uint8_t i = static_cast<uint8_t>(Slot::Vertex); i < static_cast<uint8_t>(Slot::Num); ++i)
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

        for (const auto &[k, v] : mShaderCommon->GetBindingMap())
        {
            VkWriteDescriptorSet write = {};
            ZeroVulkanStruct(write, VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET);
            write.dstSet = mShaderCommon->GetDescriptorSetList()[GetSetIndex(k)];
            write.dstBinding = v.binding;
            write.dstArrayElement = 0;
            write.descriptorType = v.descriptorType;
            write.descriptorCount = v.descriptorCount;

            mShaderCommon->GetWriteMap()[k] = write;
        }
    }

    VkShaderStageFlagBits GfxVulkanVertexRasterShader::GetShaderStageFlag(IGfxVertexRasterShader::Slot slot)
    {
        switch (slot)
        {
        case Slot::Vertex:
            return VK_SHADER_STAGE_VERTEX_BIT;
        case Slot::Fragment:
            return VK_SHADER_STAGE_FRAGMENT_BIT;
        case Slot::TessellationControl:
            return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
        case Slot::TessellationEvaluation:
            return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
        case Slot::Geometry:
            return VK_SHADER_STAGE_GEOMETRY_BIT;
        }

        return VK_SHADER_STAGE_ALL; // for avoiding compiler warning
    }

    GfxVulkanComputeShader::GfxVulkanComputeShader(IGfxDevice *device)
        : GfxVulkanObject(device), mShaderCommon(std::make_unique<GfxVulkanShaderCommon>(device))
    {
    }

    GfxVulkanComputeShader::~GfxVulkanComputeShader()
    {
    }

    IGfxShader *GfxVulkanComputeShader::BindBuffer(StringView name, const IGfxBuffer *buffer)
    {
        mShaderCommon->BindBufferImpl(name, buffer);
        return this;
    }
    IGfxShader *GfxVulkanComputeShader::BindTexture(StringView name, const IGfxTexture *texture)
    {
        mShaderCommon->BindTextureImpl(name, texture);
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
        mShaderCommon->CreateDescriptorPool();
        mShaderCommon->AllocateDescriptorSets();
        DumpDescriptorWrites();
        mShaderCommon->CreatePipelineLayout();

        return this;
    }

    void GfxVulkanComputeShader::DumpDescriptorBindings()
    {
        for (auto &spvBinding : mShaderModule->GetSpvReflectData().descriptorBindings)
        {
            for (auto &vkBinding : mShaderCommon->GetBindingMap())
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
            mShaderCommon->SetBinding(spvBinding->name, layoutBinding);
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

        mShaderCommon->GetDescriptorSetLayoutList().resize(maxCount + 1);

        auto GetDescriptorBinding = [&](StringView name)
        {
            for (auto &[k, v] : mShaderCommon->GetBindingMap())
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

            VK_CHECK(vkCreateDescriptorSetLayout(mDevice->GetLogicDevice(), &layoutInfo, nullptr, &mShaderCommon->GetDescriptorSetLayoutList()[setSlot]));
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

        for (const auto &[k, v] : mShaderCommon->GetBindingMap())
        {
            VkWriteDescriptorSet write = {};
            ZeroVulkanStruct(write, VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET);
            write.dstSet = mShaderCommon->GetDescriptorSet(GetSetIndex(k));
            write.dstBinding = v.binding;
            write.dstArrayElement = 0;
            write.descriptorType = v.descriptorType;
            write.descriptorCount = v.descriptorCount;

            mShaderCommon->GetWriteMap()[k] = write;
        }
    }

    VkShaderStageFlagBits GfxVulkanComputeShader::GetShaderStageFlag()
    {
        return VK_SHADER_STAGE_COMPUTE_BIT;
    }
}