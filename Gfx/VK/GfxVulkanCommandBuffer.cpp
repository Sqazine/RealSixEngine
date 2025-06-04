#include "GfxVulkanCommandBuffer.hpp"
#include "Gfx/IGfxDevice.hpp"
#include "GfxVulkanUtils.hpp"
#include "GfxVulkanDevice.hpp"
#include "GfxVulkanShader.hpp"
#include "GfxVulkanPipeline.hpp"
#include "Config/Config.hpp"
#include "Core/Logger.hpp"
namespace RealSix
{
    VkQueue QueryQueueByCommandType(IGfxDevice *device, GfxCommandType type)
    {
        auto vulkanDevice = static_cast<GfxVulkanDevice *>(device);
        switch (type)
        {
        case GfxCommandType::GRAPHICS:
            return vulkanDevice->GetGraphicsQueue();
        case GfxCommandType::COMPUTE:
            return vulkanDevice->GetComputeQueue();
        case GfxCommandType::TRANSFER:
            return vulkanDevice->GetTransferQueue();
        default:
            REALSIX_LOG_ERROR("Unknown GfxCommandType: {}", static_cast<int>(type));
            return vulkanDevice->GetGraphicsQueue();
        }
    }

    GfxVulkanCommandBuffer::GfxVulkanCommandBuffer(IGfxDevice *device, GfxCommandType type, bool isSingleUse)
        : GfxVulkanObject(device), mCommandType(type), mRelatedQueue(QueryQueueByCommandType(device, type)), mPoolHandle(VK_NULL_HANDLE), mHandle(VK_NULL_HANDLE),mIsUseOnce(isSingleUse)
    {
        VkCommandPoolCreateInfo poolInfo;
        ZeroVulkanStruct(poolInfo, VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO);
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        poolInfo.queueFamilyIndex = GetVulkanQueueFamilyIndex(mDevice, type);
        VK_CHECK(vkCreateCommandPool(mDevice->GetLogicDevice(), &poolInfo, nullptr, &mPoolHandle));

        VkCommandBufferAllocateInfo allocInfo;
        ZeroVulkanStruct(allocInfo, VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO);
        allocInfo.commandPool = mPoolHandle;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = 1;
        VK_CHECK(vkAllocateCommandBuffers(mDevice->GetLogicDevice(), &allocInfo, &mHandle));

        {
            mSignalSemaphore = std::make_unique<GfxVulkanSemaphore>(mDevice);
            if (!isSingleUse)
            {
                mFence = std::make_unique<GfxVulkanFence>(mDevice, true);
            }
            else
            {
                mFence.reset(nullptr);
            }
        }
    }

    GfxVulkanCommandBuffer::~GfxVulkanCommandBuffer()
    {
        mDevice->WaitIdle();
        vkDestroyCommandPool(mDevice->GetLogicDevice(), mPoolHandle, nullptr);
    }

    IGfxCommandBuffer *GfxVulkanCommandBuffer::Begin()
    {
        VkCommandBufferBeginInfo beginInfo{};
        ZeroVulkanStruct(beginInfo, VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO);
        if(mIsUseOnce)
        {
            beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        }
        VK_CHECK(vkBeginCommandBuffer(mHandle, &beginInfo));
        return this;
    }

    IGfxCommandBuffer *GfxVulkanCommandBuffer::End()
    {
        VK_CHECK(vkEndCommandBuffer(mHandle));
        return this;
    }

    IGfxCommandBuffer *GfxVulkanCommandBuffer::BeginRenderPass(IGfxSwapChain *swapChain)
    {
        REALSIX_CHECK(mCommandType == GfxCommandType::GRAPHICS, "BeginRenderPass can only be called on GRAPHICS command buffer!");

        auto vulkanSwapChain = static_cast<GfxVulkanSwapChain *>(swapChain);
        auto extent = vulkanSwapChain->GetExtent();

        auto colorAttachment = vulkanSwapChain->GetColorAttachment();
        auto depthAttachment = vulkanSwapChain->GetDepthAttachment();

        const bool useMsaa = GfxConfig::GetInstance().GetMsaa() > Msaa::X1;

        VkRenderingAttachmentInfo vulkanColorAttachment = ToVkAttachment(colorAttachment);
        vulkanColorAttachment.resolveMode = useMsaa ? VK_RESOLVE_MODE_AVERAGE_BIT : VK_RESOLVE_MODE_NONE;
        vulkanColorAttachment.resolveImageView = useMsaa ? vulkanSwapChain->GetCurrentSwapChainBackTexture()->GetView() : nullptr;

        VkRenderingAttachmentInfo vulkanDepthAttachment = ToVkAttachment(depthAttachment);

        VkRenderingInfo renderingInfo;
        ZeroVulkanStruct(renderingInfo, VK_STRUCTURE_TYPE_RENDERING_INFO);
        renderingInfo.pNext = nullptr;
        renderingInfo.flags = 0;
        renderingInfo.renderArea.offset = {0, 0};
        renderingInfo.renderArea.extent = extent;
        renderingInfo.layerCount = 1;
        renderingInfo.colorAttachmentCount = 1;
        renderingInfo.pColorAttachments = &vulkanColorAttachment;
        renderingInfo.pDepthAttachment = &vulkanDepthAttachment;

        vkCmdBeginRendering(mHandle, &renderingInfo);

        return this;
    }

    IGfxCommandBuffer *GfxVulkanCommandBuffer::BeginRenderPass(uint8_t colorAttachmentCount, GfxColorAttachment *colorAttachments, GfxDepthStencilAttachment *depthAttachment)
    {
        REALSIX_CHECK(mCommandType == GfxCommandType::GRAPHICS, "BeginRenderPass can only be called on GRAPHICS command buffer!");

        std::vector<VkRenderingAttachmentInfo> rawColorAttachments(colorAttachmentCount);
        for (size_t i = 0; i < colorAttachmentCount; ++i)
        {
            ZeroVulkanStruct(rawColorAttachments[i], VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO);
            rawColorAttachments[i].imageView = static_cast<GfxVulkanTexture *>(colorAttachments[i].texture)->GetView();
            rawColorAttachments[i].imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            rawColorAttachments[i].resolveMode = VK_RESOLVE_MODE_NONE;
            rawColorAttachments[i].resolveImageView = nullptr;
            rawColorAttachments[i].resolveImageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            rawColorAttachments[i].loadOp = ToVkAttachmentOp(colorAttachments[i].loadOp);
            rawColorAttachments[i].storeOp = ToVkAttachmentOp(colorAttachments[i].storeOp);
            rawColorAttachments[i].clearValue.color.float32[0] = colorAttachments[i].clearValue.x;
            rawColorAttachments[i].clearValue.color.float32[1] = colorAttachments[i].clearValue.y;
            rawColorAttachments[i].clearValue.color.float32[2] = colorAttachments[i].clearValue.z;
            rawColorAttachments[i].clearValue.color.float32[3] = colorAttachments[i].clearValue.w;
        }

        VkRenderingAttachmentInfo rawDepthAttachment;
        ZeroVulkanStruct(rawDepthAttachment, VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO);
        rawDepthAttachment.imageView = static_cast<GfxVulkanTexture *>(depthAttachment->texture)->GetView();
        rawDepthAttachment.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        rawDepthAttachment.resolveMode = VK_RESOLVE_MODE_NONE;
        rawDepthAttachment.resolveImageView = nullptr;
        rawDepthAttachment.resolveImageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        rawDepthAttachment.loadOp = ToVkAttachmentOp(depthAttachment->loadOp);
        rawDepthAttachment.storeOp = ToVkAttachmentOp(depthAttachment->storeOp);
        rawDepthAttachment.clearValue = {{1.0f, 0.0f}};

        VkExtent2D extent = {depthAttachment->texture->GetDesc().width, depthAttachment->texture->GetDesc().height};

        VkRenderingInfo renderingInfo;
        ZeroVulkanStruct(renderingInfo, VK_STRUCTURE_TYPE_RENDERING_INFO);
        renderingInfo.pNext = nullptr;
        renderingInfo.flags = 0;
        renderingInfo.renderArea.offset = {0, 0};
        renderingInfo.renderArea.extent = extent;
        renderingInfo.layerCount = 1;
        renderingInfo.colorAttachmentCount = colorAttachmentCount;
        renderingInfo.pColorAttachments = rawColorAttachments.data();
        renderingInfo.pDepthAttachment = &rawDepthAttachment;

        vkCmdBeginRendering(mHandle, &renderingInfo);
        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float)extent.width;
        viewport.height = (float)extent.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(mHandle, 0, 1, &viewport);

        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = extent;
        vkCmdSetScissor(mHandle, 0, 1, &scissor);

        return this;
    }

    IGfxCommandBuffer *GfxVulkanCommandBuffer::EndRenderPass()
    {
        vkCmdEndRendering(mHandle);
        return this;
    }

    IGfxCommandBuffer *GfxVulkanCommandBuffer::Submit(GfxVulkanSemaphore *waitSemaphore)
    {
        VkSemaphore signalRawSemaphore = mSignalSemaphore->GetHandle();

        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};

        VkSubmitInfo submitInfo;
        ZeroVulkanStruct(submitInfo, VK_STRUCTURE_TYPE_SUBMIT_INFO);
        if (waitSemaphore)
        {
            VkSemaphore waitRawSemaphore = waitSemaphore->GetHandle();
            submitInfo.waitSemaphoreCount = 1;
            submitInfo.pWaitSemaphores = &waitRawSemaphore;
        }
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = &signalRawSemaphore;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &mHandle;

        if (mFence)
            VK_CHECK(vkQueueSubmit(mRelatedQueue, 1, &submitInfo, mFence->GetHandle()));
        else
            VK_CHECK(vkQueueSubmit(mRelatedQueue, 1, &submitInfo, VK_NULL_HANDLE));

        return this;
    }

    IGfxCommandBuffer *GfxVulkanCommandBuffer::Submit()
    {
        this->Submit(nullptr);
        return this;
    }

    IGfxCommandBuffer *GfxVulkanCommandBuffer::TransitionImageLayout(GfxVulkanTexture *texture, VkImageLayout oldLayout, VkImageLayout newLayout)
    {
        VkImageMemoryBarrier barrier{};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.oldLayout = oldLayout;
        barrier.newLayout = newLayout;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.image = texture->GetHandle();
        barrier.subresourceRange.aspectMask = texture->GetAspect();
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = texture->GetMipLevelCount();
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;

        VkPipelineStageFlags sourceStage;
        VkPipelineStageFlags destinationStage;

        if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
        {
            barrier.srcAccessMask = 0;
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

            sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        }
        else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
        {
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
            destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        }
        else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)
        {
            barrier.srcAccessMask = 0;
            barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

            sourceStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            destinationStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        }
        else if (oldLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR)
        {
            barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            barrier.dstAccessMask = 0;

            sourceStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            destinationStage = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
        }
        else
        {
            REALSIX_LOG_ERROR("unsupported layout transition!");
        }

        vkCmdPipelineBarrier(
            GetHandle(),
            sourceStage, destinationStage,
            0,
            0, nullptr,
            0, nullptr,
            1, &barrier);

        return this;
    }

    IGfxCommandBuffer *GfxVulkanCommandBuffer::CopyBuffer(IGfxBuffer *src, IGfxBuffer *dst, size_t bufferSize)
    {
        auto srcVulkanBuffer = static_cast<GfxVulkanBuffer *>(src);
        auto dstVulkanBuffer = static_cast<GfxVulkanBuffer *>(dst);
        VkBufferCopy copyRegion{};
        copyRegion.size = static_cast<VkDeviceSize>(bufferSize);
        vkCmdCopyBuffer(mHandle, srcVulkanBuffer->GetHandle(), dstVulkanBuffer->GetHandle(), 1, &copyRegion);
        return this;
    }

    IGfxCommandBuffer *GfxVulkanCommandBuffer::CopyBufferToImage(IGfxBuffer *src, IGfxTexture *dst, uint32_t width, uint32_t height)
    {
        auto srcVulkanBuffer = dynamic_cast<GfxVulkanBuffer *>(src);
        auto dstVulkanTexture = dynamic_cast<GfxVulkanTexture *>(dst);

        VkBufferImageCopy region{};
        region.bufferOffset = 0;
        region.bufferRowLength = 0;
        region.bufferImageHeight = 0;
        region.imageSubresource.aspectMask = dstVulkanTexture->GetAspect();
        region.imageSubresource.mipLevel = 0;
        region.imageSubresource.baseArrayLayer = 0;
        region.imageSubresource.layerCount = 1;
        region.imageOffset = {0, 0, 0};
        region.imageExtent = {width, height, 1};

        vkCmdCopyBufferToImage(mHandle, srcVulkanBuffer->GetHandle(), dstVulkanTexture->GetHandle(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

        return this;
    }

    IGfxCommandBuffer *GfxVulkanCommandBuffer::BindRasterPipeline(IGfxRasterPipeline *pipeline)
    {
        REALSIX_CHECK(mCommandType == GfxCommandType::GRAPHICS, "BindRasterPipeline can only be called on GRAPHICS command buffer!");

        auto vulkanRasterShader = static_cast<GfxVulkanRasterShader *>(static_cast<IGfxShader *>(pipeline->GetShader()));
        vulkanRasterShader->Flush();

        auto sets = vulkanRasterShader->GetDescriptorSets();
        if (!sets.empty())
            vkCmdBindDescriptorSets(mHandle, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkanRasterShader->GetPipelineLayout(), 0, sets.size(), sets.data(), 0, nullptr);

        auto vulkanRasterPipeline = static_cast<GfxVulkanRasterPipeline *>(pipeline);
        vkCmdBindPipeline(mHandle, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkanRasterPipeline->GetHandle());
        return this;
    }

    IGfxCommandBuffer *GfxVulkanCommandBuffer::BindComputePipeline(IGfxComputePipeline *pipeline)
    {
        REALSIX_CHECK(mCommandType == GfxCommandType::COMPUTE, "BindComputePipeline can only be called on COMPUTE command buffer!");

        auto vulkanComputeShader = static_cast<GfxVulkanComputeShader *>(static_cast<IGfxShader *>(pipeline->GetShader()));
        vulkanComputeShader->Flush();

        auto sets = vulkanComputeShader->GetDescriptorSets();
        if (!sets.empty())
            vkCmdBindDescriptorSets(mHandle, VK_PIPELINE_BIND_POINT_COMPUTE, vulkanComputeShader->GetPipelineLayout(), 0, sets.size(), sets.data(), 0, nullptr);

        auto vulkanComputePipeline = static_cast<GfxVulkanComputePipeline *>(pipeline);
        vkCmdBindPipeline(mHandle, VK_PIPELINE_BIND_POINT_COMPUTE, vulkanComputePipeline->GetHandle());
        return this;
    }

    IGfxCommandBuffer *GfxVulkanCommandBuffer::BindVertexBuffer(const GfxVertexBuffer *vertexBuffer)
    {
        REALSIX_CHECK(mCommandType == GfxCommandType::GRAPHICS, "BindVertexBuffer can only be called on GRAPHICS command buffer!");

        auto vulkanVertexBuffer = static_cast<const GfxVulkanBuffer *>(vertexBuffer->GetGfxBuffer());
        VkBuffer vertexBuffers[] = {vulkanVertexBuffer->GetHandle()};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(mHandle, 0, 1, vertexBuffers, offsets);
        return this;
    }
    IGfxCommandBuffer *GfxVulkanCommandBuffer::BindIndexBuffer(const GfxIndexBuffer *indexBuffer)
    {
        REALSIX_CHECK(mCommandType == GfxCommandType::GRAPHICS, "BindIndexBuffer can only be called on GRAPHICS command buffer!");

        auto vulkanIndexBuffer = static_cast<const GfxVulkanBuffer *>(indexBuffer->GetGfxBuffer());
        vkCmdBindIndexBuffer(mHandle, vulkanIndexBuffer->GetHandle(), 0, ToVkIndexType(indexBuffer->GetIndexType()));
        return this;
    }

    IGfxCommandBuffer *GfxVulkanCommandBuffer::DrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance)
    {
        REALSIX_CHECK(mCommandType == GfxCommandType::GRAPHICS, "DrawIndexed can only be called on GRAPHICS command buffer!");

        vkCmdDrawIndexed(mHandle, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
        return this;
    }

    IGfxCommandBuffer *GfxVulkanCommandBuffer::Draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance)
    {
        REALSIX_CHECK(mCommandType == GfxCommandType::GRAPHICS, "Draw can only be called on GRAPHICS command buffer!");

        vkCmdDraw(mHandle, vertexCount, instanceCount, firstVertex, firstInstance);
        return this;
    }

    IGfxCommandBuffer *GfxVulkanCommandBuffer::Dispatch(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ)
    {
        REALSIX_CHECK(mCommandType == GfxCommandType::COMPUTE, "Dispatch can only be called on COMPUTE command buffer!");

        vkCmdDispatch(mHandle, groupCountX, groupCountY, groupCountZ);
        return this;
    }
}