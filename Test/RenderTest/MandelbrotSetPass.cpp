#include "MandelbrotSetPass.hpp"
#include <filesystem>
#include "Resource/ResourceLoader.hpp"
#include "Render/Mesh.hpp"
#include "Gfx/IGfxCommandBuffer.hpp"
#include "Resource/FileSystem.hpp"
#define WORKGROUP_SIZE 32

namespace RealSix
{
    void MandelbrotSetPass::Init()
    {
        mUniform.width = Renderer::GetWindow()->GetSize().x;
        mUniform.height = Renderer::GetWindow()->GetSize().y;

        GfxBufferDesc bufferDesc{};
        bufferDesc.bufferSize = mUniform.width * mUniform.height * sizeof(Vector4f);
        bufferDesc.elementSize = sizeof(Vector4f);
        bufferDesc.data = nullptr;
        mComputeImageBuffer.reset(GfxShaderStorageBuffer::Create(Renderer::GetGfxDevice(), bufferDesc));

        GfxBufferDesc unifomrBufferDesc{};
        unifomrBufferDesc.bufferSize = sizeof(Uniform);
        unifomrBufferDesc.elementSize = sizeof(Uniform);
        unifomrBufferDesc.data = &mUniform;
        mUniformBuffer.reset(GfxUniformBuffer::Create(Renderer::GetGfxDevice(), unifomrBufferDesc));

        auto compShaderContent = ResourceLoader::GetInstance().GetShaderContentFromDisk(TEST_SHADER_DIR "MandelbrotSet.comp.slang");
        mComputeShader.reset(IGfxComputeShader::Create(Renderer::GetGfxDevice()));
        mComputeShader->SetComputeShader(compShaderContent);
        mComputeShader->Build();
        mComputeShader->BindBuffer("imageData", mComputeImageBuffer->GetGfxBuffer());
        mComputeShader->BindBuffer("uniform", mUniformBuffer->GetGfxBuffer());

        mComputePipeline.reset(IGfxComputePipeline::Create(Renderer::GetGfxDevice(), mComputeShader.get()));

        auto vertShaderContent = ResourceLoader::GetInstance().GetShaderContentFromDisk(TEST_SHADER_DIR "ScreenSpaceQuad.vert.slang");
        auto fragShaderContent = ResourceLoader::GetInstance().GetShaderContentFromDisk(TEST_SHADER_DIR "MandelbrotSetDrawPass.frag.slang");
        mRasterShader.reset(IGfxRasterShader::Create(Renderer::GetGfxDevice()));
        mRasterShader->SetVertexShader(vertShaderContent);
        mRasterShader->SetFragmentShader(fragShaderContent);
        mRasterShader->Build();
        mRasterShader->BindBuffer("imageData", mComputeImageBuffer->GetGfxBuffer());
        mRasterShader->BindBuffer("uniform", mUniformBuffer->GetGfxBuffer());

        GfxRasterPipelineStateDesc rasterPipelineState;
        rasterPipelineState.colorAttachmentCount = 1;
        rasterPipelineState.colorAttachments = &Renderer::GetGfxDevice()->GetSwapChain()->GetColorAttachment();
        rasterPipelineState.depthAttachment = &Renderer::GetGfxDevice()->GetSwapChain()->GetDepthAttachment();
        rasterPipelineState.shader = mRasterShader.get();
        rasterPipelineState.vertexBinding = GfxVertexInputBinding::Default();
        rasterPipelineState.depthTest = false;
        rasterPipelineState.depthWrite = false;
        mRasterPipeline.reset(IGfxRasterPipeline::Create(Renderer::GetGfxDevice(), rasterPipelineState));

        std::unique_ptr<IGfxCommandBuffer> commandBuffer = std::unique_ptr<IGfxCommandBuffer>(IGfxCommandBuffer::Create(Renderer::GetGfxDevice(), GfxCommandType::COMPUTE, true));
        commandBuffer->Begin();
        commandBuffer->BindComputePipeline(mComputePipeline.get());
        commandBuffer->Dispatch((uint32_t)ceil(mUniform.width / float(WORKGROUP_SIZE)), (uint32_t)ceil(mUniform.height / float(WORKGROUP_SIZE)), 1);
        commandBuffer->End();
        commandBuffer->Submit();
    }

    void MandelbrotSetPass::Execute(FrameGraph &frameGraph)
    {
        bool isFirstTask = frameGraph.GetSlotOf<MandelbrotSetPass>() == 0;

        auto swapChain = Renderer::GetGfxDevice()->GetSwapChain();
        if (isFirstTask)
        {
            swapChain->GetColorAttachment().loadOp = GfxAttachmentLoadOp::CLEAR;
            swapChain->GetColorAttachment().storeOp = GfxAttachmentStoreOp::STORE;

            swapChain->GetDepthAttachment().loadOp = GfxAttachmentLoadOp::CLEAR;
            swapChain->GetDepthAttachment().storeOp = GfxAttachmentStoreOp::DONT_CARE;
        }
        else
        {
            swapChain->GetColorAttachment().loadOp = GfxAttachmentLoadOp::LOAD;
            swapChain->GetColorAttachment().storeOp = GfxAttachmentStoreOp::STORE;

            swapChain->GetDepthAttachment().loadOp = GfxAttachmentLoadOp::LOAD;
            swapChain->GetDepthAttachment().storeOp = GfxAttachmentStoreOp::DONT_CARE;
        }

        auto cmdBuffer = Renderer::GetGfxDevice()->GetCurrentBackCommandBuffer();
        cmdBuffer
            ->BeginRenderPass(swapChain)
            ->BindRasterPipeline(mRasterPipeline.get())
            ->Draw(3, 1, 0, 0)
            ->EndRenderPass();
    }

    void InsertMandelbrotSetPass(FrameGraph &frameGraph, size_t slot)
    {
        if (frameGraph.Has<MandelbrotSetPass>())
            return;

        frameGraph.InsertRenderTask<MandelbrotSetPass>(
            slot,
            true,
            [&](MandelbrotSetPass *task, FrameGraphRenderTaskBuilder &builder)
            {
                task->Init();
            },
            [&](MandelbrotSetPass *task)
            {
                task->Execute(frameGraph);
            });
    }
}