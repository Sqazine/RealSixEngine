#pragma once
#include <vulkan/vulkan.h>
#include <memory>
#include "Gfx/IGfxCommandBuffer.hpp"
#include "GfxVulkanObject.hpp"
#include "GfxVulkanSyncObject.hpp"
#include "GfxVulkanTexture.hpp"
#include "GfxVulkanBuffer.hpp"
namespace RealSix
{
    class GfxVulkanCommandBuffer : public GfxVulkanObject, public IGfxCommandBuffer
    {
    public:
        GfxVulkanCommandBuffer(IGfxDevice *device, GfxCommandType type, bool isSingleUse);
        ~GfxVulkanCommandBuffer() override;

        VkCommandBuffer GetHandle() const { return mHandle; }

        IGfxCommandBuffer *Begin() override;
        IGfxCommandBuffer *End() override;
        IGfxCommandBuffer *BeginRenderPass(IGfxSwapChain *swapChain) override;
        IGfxCommandBuffer *BeginRenderPass(uint8_t colorAttachmentCount, GfxColorAttachment *colorAttachments, GfxDepthStencilAttachment *depthAttachment) override;
        IGfxCommandBuffer *EndRenderPass() override;
        IGfxCommandBuffer *CopyBuffer(IGfxBuffer *src, IGfxBuffer *dst, size_t bufferSize) override;
        IGfxCommandBuffer *CopyBufferToImage(IGfxBuffer *src, IGfxTexture *dst, uint32_t width, uint32_t height) override;
        IGfxCommandBuffer *BindRasterPipeline(IGfxRasterPipeline *pipeline) override;
        IGfxCommandBuffer *BindComputePipeline(IGfxComputePipeline *pipeline) override;
        IGfxCommandBuffer *BindVertexBuffer(const GfxVertexBuffer *vertexBuffer) override;
        IGfxCommandBuffer *BindIndexBuffer(const GfxIndexBuffer *indexBuffer) override;
        IGfxCommandBuffer *DrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) override;
        IGfxCommandBuffer *Draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) override;

        IGfxCommandBuffer *Dispatch(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) override;

        IGfxCommandBuffer *Submit() override;
        IGfxCommandBuffer *Submit(GfxVulkanSemaphore *waitSemaphore);

        IGfxCommandBuffer *TransitionImageLayout(GfxVulkanTexture *texture, VkImageLayout oldLayout, VkImageLayout newLayout);

        GfxVulkanFence *GetFence() const { return mFence.get(); }
        GfxVulkanSemaphore *GetSignalSemaphore() const { return mSignalSemaphore.get(); }

    private:
        bool mIsUseOnce;
        GfxCommandType mCommandType;
        VkQueue mRelatedQueue;
        VkCommandPool mPoolHandle;
        VkCommandBuffer mHandle;

        std::unique_ptr<GfxVulkanSemaphore> mSignalSemaphore;
        std::unique_ptr<GfxVulkanFence> mFence;
    };
}