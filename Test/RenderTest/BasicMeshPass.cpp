#include "BasicMeshPass.h"
#include "Core/IO.h"
#include "Gfx/IGfxCommandBuffer.h"
#include "Gfx/IGfxPipeline.h"
#include "Editor/EditorUIPass/EditorUIPass.h"
namespace RealSix
{
    void BasicMeshPass::Init()
    {
        std::string shaderCompile;
        shaderCompile = "slangc.exe " SHADER_DIR "BasicMeshPass.vert.slang"
                        " -profile sm_6_6+spirv_1_6 -target spirv -o " SHADER_DIR "BasicMeshPass.vert.slang.spv"
                        " -emit-spirv-directly -fvk-use-entrypoint-name";
        system(shaderCompile.c_str());
        shaderCompile = "slangc.exe " SHADER_DIR "BasicMeshPass.frag.slang"
                        " -profile sm_6_6+spirv_1_6 -target spirv -o " SHADER_DIR "BasicMeshPass.frag.slang.spv"
                        " -emit-spirv-directly -fvk-use-entrypoint-name";
        system(shaderCompile.c_str());

        mCamera = std::make_unique<Camera>();
        mCamera->SetPosition(Vector3f(0.0f, 0.0f, 10.0f));
        mCamera->SetFovByDegree(60.0f);
        mMesh.reset(Mesh::CreateBuiltinMesh(MeshType::SPHERE));

        GfxTextureDesc textureDesc = ReadTexture(ASSETS_DIR "uv.png");
        mColorTexture.reset(IGfxTexture::Create(Renderer::GetGfxDevice(), textureDesc));

        GfxBufferDesc bufferDesc{};
        bufferDesc.bufferSize = sizeof(MeshUniformData);
        bufferDesc.elementSize = sizeof(MeshUniformData);
        bufferDesc.data = &mMeshUniformData;
        mMeshUniformDataBuffer.reset(GfxUniformBuffer::Create(Renderer::GetGfxDevice(), bufferDesc));

        auto vertShaderContent = ReadBinaryFile(SHADER_DIR "BasicMeshPass.vert.slang.spv");
        auto fragShaderContent = ReadBinaryFile(SHADER_DIR "BasicMeshPass.frag.slang.spv");
        mShader.reset(IGfxRasterShader::Create(Renderer::GetGfxDevice(), vertShaderContent, fragShaderContent));

        GfxRasterPipelineStateDesc pipelineState;
        pipelineState.colorAttachmentCount = 1;
        pipelineState.colorAttachments = &Renderer::GetGfxDevice()->GetSwapChain()->GetColorAttachment();
        pipelineState.depthAttachment = &Renderer::GetGfxDevice()->GetSwapChain()->GetDepthAttachment();
        pipelineState.shader = mShader.get();
        pipelineState.vertexBinding = Vertex::GetVertexBinding();

        mRasterPipeline.reset(IGfxRasterPipeline::Create(Renderer::GetGfxDevice(), pipelineState));

        mShader->BindBuffer("cameraData", mCamera->GetRenderDataBuffer()->GetGfxBuffer());
        mShader->BindBuffer("meshUBO", mMeshUniformDataBuffer->GetGfxBuffer());
        mShader->BindTexture("texSampler", mColorTexture.get());
    }

    void BasicMeshPass::Execute(FrameGraph &frameGraph)
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
            ->BindVertexBuffer(mMesh->GetVertexBuffer())
            ->BindIndexBuffer(mMesh->GetIndexBuffer())
            ->DrawIndexed(mMesh->GetIndexBuffer()->GetElementCount(), 1, 0, 0, 0)
            ->EndRenderPass();
    }

    void AddBasicMeshPass(FrameGraph &frameGraph)
    {
        frameGraph.AddRenderTask<BasicMeshPass>(
            true,
            [&](BasicMeshPass *task, RenderTaskBuilder &builder)
            {
                task->Init();
            },
            [&](BasicMeshPass *task)
            {
                task->Execute(frameGraph);
            });
    }
}