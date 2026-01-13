#include "BasicMeshPass.hpp"
#include "Gfx/IGfxCommandBuffer.hpp"
#include "Gfx/IGfxPipeline.hpp"
#include "Editor/EditorUIPass/EditorUIPass.hpp"
#include "Resource/ResourceLoader.hpp"
#include "Resource/FileSystem.hpp"
namespace RealSix
{
    void BasicMeshPass::Init()
    {
        mCamera = std::make_unique<Camera>();
        mCamera->SetPosition(Vector3f(0.0f, 0.0f, 10.0f));
        mCamera->SetFovByDegree(60.0f);

        mStaticMeshPtr = ResourceLoader::GetInstance().GetBuiltinSphereMesh();
        mColorTexture = ResourceLoader::GetInstance().GetTextureFromDisk(TEST_TEXTURE_DIR "uv.png");

        GfxBufferDesc bufferDesc{};
        bufferDesc.bufferSize = sizeof(MeshUniformData);
        bufferDesc.elementSize = sizeof(MeshUniformData);
        bufferDesc.data = &mMeshUniformData;
        mMeshUniformDataBuffer.reset(GfxUniformBuffer::Create(Renderer::GetGfxDevice(), bufferDesc));

        auto vertShaderContent = ResourceLoader::GetInstance().GetShaderContentFromDisk(TEST_SHADER_DIR "BasicMeshPass.vert.slang");
        auto fragShaderContent = ResourceLoader::GetInstance().GetShaderContentFromDisk(TEST_SHADER_DIR "BasicMeshPass.frag.slang");
        mShader.reset(IGfxVertexRasterShader::Create(Renderer::GetGfxDevice()));
        mShader->SetVertexShader(vertShaderContent);
        mShader->SetFragmentShader(fragShaderContent);
        mShader->Build();

        GfxRasterPipelineStateDesc pipelineState;
        pipelineState.colorAttachmentCount = 1;
        pipelineState.colorAttachments = &Renderer::GetGfxDevice()->GetSwapChain()->GetColorAttachment();
        pipelineState.depthAttachment = &Renderer::GetGfxDevice()->GetSwapChain()->GetDepthAttachment();
        pipelineState.shader = mShader.get();
        pipelineState.vertexBinding = mStaticMeshPtr->GetVertexInputBinding();

        mRasterPipeline.reset(IGfxRasterPipeline::Create(Renderer::GetGfxDevice(), pipelineState));

        mShader->BindBuffer("cameraData", mCamera->GetRenderDataBuffer()->GetGfxBuffer());
        mShader->BindBuffer("meshUBO", mMeshUniformDataBuffer->GetGfxBuffer());
        mShader->BindTexture("texSampler", mColorTexture);
    }

    void BasicMeshPass::Execute(FrameGraph &frameGraph)
    {
        mCamera->SetAspect(Renderer::GetGfxDevice()->GetSwapChain()->GetAspect());

        bool isFirstTask = frameGraph.GetSlotOf<BasicMeshPass>() == 0;

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
            ->BindVertexBuffer(mStaticMeshPtr->GetVertexBuffer())
            ->BindIndexBuffer(mStaticMeshPtr->GetIndexBuffer())
            ->DrawIndexed(mStaticMeshPtr->GetIndexBuffer()->GetElementCount(), 1, 0, 0, 0)
            ->EndRenderPass();
    }

    void InsertBasicMeshPass(FrameGraph &frameGraph, size_t slot)
    {
        if (frameGraph.Has<BasicMeshPass>())
            return;
            
        frameGraph.InsertRenderTask<BasicMeshPass>(
            slot,
            true,
            [&](BasicMeshPass *task, FrameGraphRenderTaskBuilder &builder)
            {
                task->Init();
            },
            [&](BasicMeshPass *task)
            {
                task->Execute(frameGraph);
            });
    }
}