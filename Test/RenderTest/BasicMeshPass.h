#pragma once
#include <memory>
#include "Render/Renderer.h"
#include "Render/Mesh.h"
#include "Render/Camera.h"
#include "Gfx/IGfxShader.h"
#include "Gfx/IGfxPipeline.h"
#include "Gfx/IGfxTexture.h"
namespace RealSix
{
    class BasicMeshPass
    {
    public:
        void Init();
        void Execute(FrameGraph &frameGraph);

        struct MeshUniformData
        {
            alignas(16) Matrix4f model;
        };

        MeshUniformData mMeshUniformData;

        std::unique_ptr<Camera> mCamera;
        StaticMesh* mStaticMeshPtr;
        IGfxTexture* mColorTexture;
        std::unique_ptr<GfxUniformBuffer> mMeshUniformDataBuffer;
        std::unique_ptr<IGfxRasterShader> mShader;
        std::unique_ptr<IGfxRasterPipeline> mRasterPipeline;
    };

    void AddBasicMeshPass(FrameGraph &frameGraph);
}