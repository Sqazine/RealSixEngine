#pragma once
#include <vector>
#include <array>

#include "IGfxDevice.hpp"
#include "IGfxTexture.hpp"
#include "IGfxBuffer.hpp"

namespace RealSix
{
    class IGfxShader
    {
    public:
        IGfxShader() = default;
        virtual ~IGfxShader() = default;

        virtual IGfxShader *BindBuffer(StringView name, const IGfxBuffer *buffer) = 0;
        virtual IGfxShader *BindTexture(StringView name, const IGfxTexture *texture) = 0;
        virtual IGfxShader *Build() = 0;

        IGfxShader *AddMarco(StringView marco);

    protected:
        std::vector<StringView> mMarcos;
    };

    enum RasterShaderSlot:uint8_t
    {
        Vertex = 0,
        Fragment,
        TessellationControl,
        TessellationEvaluation,
        Geometry,
        Num,
    };

    class IGfxRasterShader : public IGfxShader
    {
    public:
        IGfxRasterShader() = default;
         virtual ~IGfxRasterShader() override = default;

        static IGfxRasterShader *Create(IGfxDevice *device);

        IGfxRasterShader *SetVertexShader(StringView source);
        IGfxRasterShader *SetFragmentShader(StringView source);
        IGfxRasterShader *SetTessellationControlShader(StringView source);
        IGfxRasterShader *SetTessellationEvaluationShader(StringView source);
        IGfxRasterShader *SetGeometryShader(StringView source);

    protected:
        std::array<StringView, RasterShaderSlot::Num> mShaderSources;
    };

    class IGfxComputeShader : public IGfxShader
    {
    public:
        IGfxComputeShader() = default;
         virtual  ~IGfxComputeShader() override = default;

        static IGfxComputeShader *Create(IGfxDevice *device);

        IGfxComputeShader *SetComputeShader(StringView source);

    protected:
        StringView mShaderSource;
    };
}