#pragma once
#include "IGfxDevice.hpp"
#include "IGfxBuffer.hpp"
#include "IGfxPipeline.hpp"
namespace RealSix
{
    class IGfxCommandBuffer
    {
    public:
        IGfxCommandBuffer() = default;
        virtual ~IGfxCommandBuffer() = default;

        static IGfxCommandBuffer *Create(IGfxDevice *device, GfxCommandType type, bool isSingleUse = false);

        virtual IGfxCommandBuffer *Begin() = 0;
        virtual IGfxCommandBuffer *End() = 0;
        virtual IGfxCommandBuffer *BeginRenderPass(IGfxSwapChain *swapChain) = 0;
        virtual IGfxCommandBuffer *BeginRenderPass(uint8_t colorAttachmentCount, GfxColorAttachment *colorAttachments, GfxDepthStencilAttachment *depthAttachments) = 0;
        virtual IGfxCommandBuffer *EndRenderPass() = 0;
        virtual IGfxCommandBuffer *CopyBuffer(IGfxBuffer *src, IGfxBuffer *dst, size_t bufferSize) = 0;
        virtual IGfxCommandBuffer *CopyBufferToImage(IGfxBuffer *src, IGfxTexture *dst, uint32_t width, uint32_t height) = 0;
        virtual IGfxCommandBuffer *BindRasterPipeline(IGfxRasterPipeline *pipeline) = 0;
        virtual IGfxCommandBuffer *BindComputePipeline(IGfxComputePipeline *pipeline) = 0;
        virtual IGfxCommandBuffer *BindVertexBuffer(const GfxVertexBuffer *vertexBuffer) = 0;
        virtual IGfxCommandBuffer *BindIndexBuffer(const GfxIndexBuffer *indexBuffer) = 0;
        virtual IGfxCommandBuffer *Draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) = 0;
        virtual IGfxCommandBuffer *DrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) = 0;
        virtual IGfxCommandBuffer *Dispatch(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) = 0;
        virtual IGfxCommandBuffer *Submit() = 0;
    };
}