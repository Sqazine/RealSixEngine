#include "GfxVulkanPipeline.hpp"
#include "GfxVulkanUtils.hpp"
#include "GfxVulkanDevice.hpp"
#include "GfxVulkanShader.hpp"
#include "Config/Config.hpp"

namespace RealSix
{
    GfxVulkanRasterPipeline::GfxVulkanRasterPipeline(IGfxDevice *device, const GfxRasterPipelineStateDesc &pipelineState)
        : GfxVulkanObject(device), IGfxRasterPipeline(pipelineState)
    {
        Create();
    }

    GfxVulkanRasterPipeline::~GfxVulkanRasterPipeline()
    {
        mDevice->WaitIdle();
        vkDestroyPipeline(mDevice->GetLogicDevice(), mHandle, nullptr);
    }

    void GfxVulkanRasterPipeline::Create()
    {
        VkPipelineVertexInputStateCreateInfo vertexState;
        ZeroVulkanStruct(vertexState, VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO);
 
        VkVertexInputBindingDescription vertexInputBindingState = GetVulkanVertexInputBindingDescription(mPipelineStateDesc.vertexBinding);
        std::vector<VkVertexInputAttributeDescription> vertexAttributes = GetVulkanVertexInputAttributeDescriptions(mPipelineStateDesc.vertexBinding);
        if (mPipelineStateDesc.vertexBinding != GfxVertexInputBinding::Default())
        {
            vertexState.vertexBindingDescriptionCount = 1;
            vertexState.pVertexBindingDescriptions = &vertexInputBindingState;
            vertexState.vertexAttributeDescriptionCount = static_cast<uint32_t>(vertexAttributes.size());
            vertexState.pVertexAttributeDescriptions = vertexAttributes.data();
        }

        VkPipelineInputAssemblyStateCreateInfo inputAssemblyState;
        ZeroVulkanStruct(inputAssemblyState, VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO);
        inputAssemblyState.topology = ToVkPrimitiveTopology(mPipelineStateDesc.primitiveTopology);
        inputAssemblyState.primitiveRestartEnable = mPipelineStateDesc.primitiveRestartEnable;

        VkPipelineViewportStateCreateInfo viewportState;
        ZeroVulkanStruct(viewportState, VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO);
        viewportState.viewportCount = 1;
        viewportState.scissorCount = 1;

        VkPipelineRasterizationStateCreateInfo rasterizerState;
        ZeroVulkanStruct(rasterizerState, VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO);
        rasterizerState.depthClampEnable = VK_FALSE;
        rasterizerState.rasterizerDiscardEnable = VK_FALSE;
        rasterizerState.polygonMode = ToVkPolygonMode(mPipelineStateDesc.polygonMode);
        rasterizerState.lineWidth = mPipelineStateDesc.lineWidth;
        rasterizerState.cullMode = ToVkCullMode(mPipelineStateDesc.cullMode);
        rasterizerState.frontFace = ToVkFrontFace(mPipelineStateDesc.frontFace);
        rasterizerState.depthBiasEnable = VK_FALSE;

        VkPipelineMultisampleStateCreateInfo multiSamplingState;
        ZeroVulkanStruct(multiSamplingState, VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO);
        multiSamplingState.sampleShadingEnable = VK_FALSE;
        multiSamplingState.rasterizationSamples = static_cast<VkSampleCountFlagBits>(GfxConfig::GetInstance().GetMsaa());

        VkPipelineDepthStencilStateCreateInfo depthStencilState;
        ZeroVulkanStruct(depthStencilState, VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO);
        depthStencilState.depthTestEnable = mPipelineStateDesc.depthTest;
        depthStencilState.depthWriteEnable = mPipelineStateDesc.depthWrite;
        depthStencilState.depthCompareOp = ToVkCompareOp(mPipelineStateDesc.depthCompare);
        depthStencilState.depthBoundsTestEnable = VK_FALSE;
        depthStencilState.stencilTestEnable = mPipelineStateDesc.stencilTest;

        std::vector<VkDynamicState> dynamicStates = {
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_SCISSOR,
        };
        VkPipelineDynamicStateCreateInfo dynamicState;
        ZeroVulkanStruct(dynamicState, VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO);
        dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
        dynamicState.pDynamicStates = dynamicStates.data();

        std::vector<VkPipelineColorBlendAttachmentState> colorBlendAttachments{};
        std::vector<VkFormat> colorAttachmentFormats;

        for (size_t i = 0; i < mPipelineStateDesc.colorAttachmentCount; ++i)
        {
            auto colorAttachment = mPipelineStateDesc.colorAttachments[i];
            VkPipelineColorBlendAttachmentState state{};
            state.blendEnable = colorAttachment.blendEnable;
            state.colorWriteMask = static_cast<uint32_t>(colorAttachment.colorChannelMask);
            colorBlendAttachments.emplace_back(state);
            colorAttachmentFormats.emplace_back(ToVkFormat(colorAttachment.texture->GetDesc().format));
        }

        VkPipelineColorBlendStateCreateInfo colorBlendState;
        ZeroVulkanStruct(colorBlendState, VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO);
        colorBlendState.logicOpEnable = VK_FALSE;
        colorBlendState.logicOp = VK_LOGIC_OP_COPY;
        colorBlendState.attachmentCount = colorBlendAttachments.size();
        colorBlendState.pAttachments = colorBlendAttachments.data();
        colorBlendState.blendConstants[0] = 0.0f;
        colorBlendState.blendConstants[1] = 0.0f;
        colorBlendState.blendConstants[2] = 0.0f;
        colorBlendState.blendConstants[3] = 0.0f;

        VkPipelineRenderingCreateInfoKHR pipelineRendering;
        ZeroVulkanStruct(pipelineRendering, VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO_KHR);
        pipelineRendering.pNext = nullptr;
        pipelineRendering.colorAttachmentCount = colorAttachmentFormats.size();
        pipelineRendering.pColorAttachmentFormats = colorAttachmentFormats.data();
        pipelineRendering.depthAttachmentFormat = ToVkFormat(mPipelineStateDesc.depthAttachment->texture->GetDesc().format);
        pipelineRendering.stencilAttachmentFormat = VK_FORMAT_UNDEFINED;

        auto rawShader = static_cast<GfxVulkanRasterShader *>(static_cast<IGfxShader *>(mPipelineStateDesc.shader));

        VkGraphicsPipelineCreateInfo pipelineInfo;
        ZeroVulkanStruct(pipelineInfo, VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO);
        pipelineInfo.pNext = &pipelineRendering;
        pipelineInfo.stageCount = rawShader->GetPipelineShaderStageInfoList().size();
        pipelineInfo.pStages = rawShader->GetPipelineShaderStageInfoList().data();
        pipelineInfo.pVertexInputState = &vertexState;
        pipelineInfo.pInputAssemblyState = &inputAssemblyState;
        pipelineInfo.pViewportState = &viewportState;
        pipelineInfo.pRasterizationState = &rasterizerState;
        pipelineInfo.pMultisampleState = &multiSamplingState;
        pipelineInfo.pDepthStencilState = &depthStencilState;
        pipelineInfo.pColorBlendState = &colorBlendState;
        pipelineInfo.pDynamicState = &dynamicState;
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
        pipelineInfo.layout = rawShader->GetPipelineLayout();

        VK_CHECK(vkCreateGraphicsPipelines(mDevice->GetLogicDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &mHandle));
    }

    GfxVulkanComputePipeline::GfxVulkanComputePipeline(IGfxDevice *device, IGfxComputeShader *shader)
        : GfxVulkanObject(device), IGfxComputePipeline(shader)
    {
        Create();
    }

    GfxVulkanComputePipeline::~GfxVulkanComputePipeline()
    {
        mDevice->WaitIdle();
        vkDestroyPipeline(mDevice->GetLogicDevice(), mHandle, nullptr);
    }

    void GfxVulkanComputePipeline::Create()
    {
        auto rawShader = static_cast<GfxVulkanComputeShader *>(static_cast<IGfxShader *>(mShader));

        VkComputePipelineCreateInfo pipelineInfo{};
        ZeroVulkanStruct(pipelineInfo, VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO);
        pipelineInfo.pNext = nullptr;
        pipelineInfo.flags = 0;
        pipelineInfo.stage = rawShader->GetPipelineShaderStageInfo();
        pipelineInfo.layout = rawShader->GetPipelineLayout();
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
        pipelineInfo.basePipelineIndex = -1;

        VK_CHECK(vkCreateComputePipelines(mDevice->GetLogicDevice(), nullptr, 1, &pipelineInfo, nullptr, &mHandle));
    }
}