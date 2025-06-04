#pragma once
#include <cstdint>
#include <memory>
#include "Render/Renderer.hpp"
#include "Gfx/IGfxShader.hpp"
#include "Gfx/IGfxPipeline.hpp"
#include "Gfx/IGfxTexture.hpp"
namespace RealSix
{
    class MandelbrotSetPass
    {
    public:
        void Init();
        void Execute(FrameGraph &frameGraph);

        struct Uniform
        {
            alignas(4) uint32_t width;
            alignas(4) uint32_t height;
        };

        Uniform mUniform;

        std::unique_ptr<GfxUniformBuffer> mUniformBuffer;
        std::unique_ptr<GfxShaderStorageBuffer> mComputeImageBuffer;

        std::unique_ptr<IGfxRasterShader> mRasterShader;
        std::unique_ptr<IGfxRasterPipeline> mRasterPipeline;

        std::unique_ptr<IGfxComputeShader> mComputeShader;
        std::unique_ptr<IGfxComputePipeline> mComputePipeline;
    };

    void InsertMandelbrotSetPass(FrameGraph &frameGraph,size_t slot);
}