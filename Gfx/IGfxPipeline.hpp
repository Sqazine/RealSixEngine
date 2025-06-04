#pragma once
#include "Gfx/IGfxCommon.hpp"
#include "Gfx/IGfxShader.hpp"
namespace RealSix
{
    struct GfxVertexAttribute
    {
        StringView name;
        GfxFormat format;
        size_t offset;

        friend bool operator==(const GfxVertexAttribute &lhs, const GfxVertexAttribute &rhs);
        friend bool operator!=(const GfxVertexAttribute &lhs, const GfxVertexAttribute &rhs);
    };

    struct GfxVertexInputBinding
    {
        uint32_t bindingPoint;
        size_t size;
        GfxVertexInputType vertexInputType;
        std::vector<GfxVertexAttribute> attribs;

        static GfxVertexInputBinding Default()
        {
            static GfxVertexInputBinding vertexBinding;
            static bool isInit = false;
            if (!isInit)
            {
                vertexBinding.bindingPoint = 0;
                vertexBinding.size = 0;
                vertexBinding.vertexInputType = GfxVertexInputType::PER_VERTEX;
                vertexBinding.attribs = {};
                isInit = true;
            }
            return vertexBinding;
        }
        friend bool operator==(const GfxVertexInputBinding &lhs, const GfxVertexInputBinding &rhs);
        friend bool operator!=(const GfxVertexInputBinding &lhs, const GfxVertexInputBinding &rhs);
    };

    struct GfxRasterPipelineStateDesc
    {
        IGfxRasterShader *shader;

        uint8_t colorAttachmentCount{0};
        GfxColorAttachment *colorAttachments;
        GfxDepthStencilAttachment *depthAttachment;

        GfxVertexInputBinding vertexBinding;
        GfxPrimitiveTopology primitiveTopology{GfxPrimitiveTopology::TRIANGLE_LIST};
        bool primitiveRestartEnable{false};
        GfxFrontFace frontFace{GfxFrontFace::COUNTER_CLOCKWISE};
        GfxPolygonMode polygonMode{GfxPolygonMode::FILL};
        float lineWidth{1.0f};
        GfxCullMode cullMode{GfxCullMode::NONE};
        bool depthTest{true};
        bool depthWrite{true};
        GfxCompareOp depthCompare{GfxCompareOp::LESS};
        bool stencilTest{false};
    };

    class IGfxRasterPipeline
    {
    public:
        IGfxRasterPipeline(const GfxRasterPipelineStateDesc &pipelineState);
        virtual ~IGfxRasterPipeline() = default;

        static IGfxRasterPipeline *Create(IGfxDevice *device, const GfxRasterPipelineStateDesc &pipelineState);

        IGfxRasterShader *GetShader() const { return mPipelineStateDesc.shader; }

    protected:
        GfxRasterPipelineStateDesc mPipelineStateDesc;
    };

    class IGfxComputePipeline
    {
    public:
        IGfxComputePipeline(IGfxComputeShader *shader);
        virtual ~IGfxComputePipeline() = default;

        static IGfxComputePipeline *Create(IGfxDevice *device, IGfxComputeShader *shader);

        IGfxComputeShader *GetShader() const { return mShader; }

    protected:
        IGfxComputeShader *mShader{nullptr};
    };
}