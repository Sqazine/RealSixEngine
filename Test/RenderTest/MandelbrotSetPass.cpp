#include "MandelbrotSetPass.h"
#include "Core/IO.h"
#include "Render/Mesh.h"
#include "Gfx/IGfxCommandBuffer.h"
#include "Editor/EditorUIPass/EditorUIPass.h"
#define WORKGROUP_SIZE 32

namespace RealSix
{
    void MandelbrotSetPass::Init()
    {
        std::string shaderCompile;
        shaderCompile = "slangc.exe " SHADER_DIR "ScreenSpaceQuad.vert.slang"
                        " -profile sm_6_6+spirv_1_6 -target spirv -o " SHADER_DIR "ScreenSpaceQuad.vert.slang.spv"
                        " -emit-spirv-directly -fvk-use-entrypoint-name";
        system(shaderCompile.c_str());
        shaderCompile = "slangc.exe " SHADER_DIR "MandelbrotSetDrawPass.frag.slang"
                        " -profile sm_6_6+spirv_1_6 -target spirv -o " SHADER_DIR "MandelbrotSetDrawPass.frag.slang.spv"
                        " -emit-spirv-directly -fvk-use-entrypoint-name";
        system(shaderCompile.c_str());
        shaderCompile = "slangc.exe " SHADER_DIR "MandelbrotSet.comp.slang"
                        " -profile sm_6_6+spirv_1_6 -target spirv -o " SHADER_DIR "MandelbrotSet.comp.slang.spv"
                        " -emit-spirv-directly -fvk-use-entrypoint-name";
        system(shaderCompile.c_str());

        mUniform.width = Renderer::GetWindow()->GetSize().x;
        mUniform.height = Renderer::GetWindow()->GetSize().y;

        GfxBufferDesc bufferDesc{};
        bufferDesc.bufferSize = sizeof(mUniform.width * mUniform.height * sizeof(Vector4f));
        bufferDesc.elementSize = sizeof(Vector4f);
        bufferDesc.data = nullptr;
        mComputeImageBuffer.reset(GfxShaderStorageBuffer::Create(Renderer::GetGfxDevice(), bufferDesc));

        GfxBufferDesc unifomrBufferDesc{};
        unifomrBufferDesc.bufferSize = sizeof(mUniform);
        unifomrBufferDesc.elementSize = sizeof(mUniform);
        unifomrBufferDesc.data = &mUniform;
        mUniformBuffer.reset(GfxUniformBuffer::Create(Renderer::GetGfxDevice(), unifomrBufferDesc));

        auto compShaderContent = ReadBinaryFile(SHADER_DIR "MandelbrotSet.comp.slang.spv");
        mComputeShader.reset(IGfxComputeShader::Create(Renderer::GetGfxDevice(), compShaderContent));
        mComputeShader->BindBuffer("imageData", mComputeImageBuffer->GetGfxBuffer());
        mComputeShader->BindBuffer("uniform", mUniformBuffer->GetGfxBuffer());

        GfxComputePipelineStateDesc computePipelineStateDesc;
        computePipelineStateDesc.shader = mComputeShader.get();
        mComputePipeline.reset(IGfxComputePipeline::Create(Renderer::GetGfxDevice(), computePipelineStateDesc));

        auto vertShaderContent = ReadBinaryFile(SHADER_DIR "ScreenSpaceQuad.vert.slang.spv");
        auto fragShaderContent = ReadBinaryFile(SHADER_DIR "MandelbrotSetDrawPass.frag.slang.spv");
        mRasterShader.reset(IGfxRasterShader::Create(Renderer::GetGfxDevice(), vertShaderContent, fragShaderContent));
        mRasterShader->BindBuffer("imageData", mComputeImageBuffer->GetGfxBuffer());
        mRasterShader->BindBuffer("uniform", mUniformBuffer->GetGfxBuffer());

        GfxRasterPipelineStateDesc rasterPipelineState;
        rasterPipelineState.colorAttachmentCount = 1;
        rasterPipelineState.colorAttachments = &Renderer::GetGfxDevice()->GetSwapChain()->GetColorAttachment();
        rasterPipelineState.depthAttachment = &Renderer::GetGfxDevice()->GetSwapChain()->GetDepthAttachment();
        rasterPipelineState.shader = mRasterShader.get();
        rasterPipelineState.vertexBinding = Vertex::GetVertexBinding();
        mRasterPipeline.reset(IGfxRasterPipeline::Create(Renderer::GetGfxDevice(), rasterPipelineState));

        std::unique_ptr<IGfxCommandBuffer> commandBuffer = std::unique_ptr<IGfxCommandBuffer>(IGfxCommandBuffer::Create(Renderer::GetGfxDevice(), GfxCommandType::COMPUTE, true));
        commandBuffer->Begin();
        commandBuffer->BindComputePipeline(mComputePipeline.get());
        commandBuffer->Dispatch((uint32_t)ceil(mUniform.width / float(WORKGROUP_SIZE)), (uint32_t)ceil(mUniform.height / float(WORKGROUP_SIZE)), 1);
        commandBuffer->End();
    }

    void MandelbrotSetPass::Execute(FrameGraph &frameGraph)
    {
        bool isFirstTask = frameGraph.HasOnly<EditorUIPass>() || frameGraph.IsNoTask();

        auto swapChain = Renderer::GetGfxDevice()->GetSwapChain();
        if (isFirstTask)
        {
            swapChain->GetColorAttachment().loadOp = GfxAttachmentLoadOp::CLEAR;
            swapChain->GetColorAttachment().storeOp = GfxAttachmentStoreOp::STORE;

            swapChain->GetDepthAttachment().loadOp = GfxAttachmentLoadOp::CLEAR;
            swapChain->GetDepthAttachment().storeOp = GfxAttachmentStoreOp::STORE;
        }
        else
        {
            swapChain->GetColorAttachment().loadOp = GfxAttachmentLoadOp::LOAD;
            swapChain->GetColorAttachment().storeOp = GfxAttachmentStoreOp::STORE;

            swapChain->GetDepthAttachment().loadOp = GfxAttachmentLoadOp::LOAD;
            swapChain->GetDepthAttachment().storeOp = GfxAttachmentStoreOp::STORE;
        }

         auto cmdBuffer = Renderer::GetGfxDevice()->GetCurrentBackCommandBuffer();
        cmdBuffer
            ->BeginRenderPass(swapChain)
            ->BindRasterPipeline(mRasterPipeline.get())
            ->Draw(3, 1, 0, 0)
            ->EndRenderPass();
    }

    void AddMandelbrotSetPass(FrameGraph &frameGraph)
    {
        frameGraph.AddRenderTask<MandelbrotSetPass>(
            true,
            [&](MandelbrotSetPass *task, RenderTaskBuilder &builder)
            {
                task->Init();
            },
            [&](MandelbrotSetPass *task)
            {
                task->Execute(frameGraph);
            });
    }
}