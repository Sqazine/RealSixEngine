#pragma once
#include <memory>
#include "Render/Renderer.hpp"
#include "Render/Mesh.hpp"
#include "Render/Camera.hpp"
#include "Gfx/IGfxShader.hpp"
#include "Gfx/IGfxPipeline.hpp"
#include "Gfx/IGfxTexture.hpp"
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
        std::unique_ptr<IGfxVertexRasterShader> mShader;
        std::unique_ptr<IGfxRasterPipeline> mRasterPipeline;
    };

    void InsertBasicMeshPass(FrameGraph &frameGraph,size_t slot);
}