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

    class IGfxRasterShader : public IGfxShader
    {
    public:
        IGfxRasterShader() = default;
        virtual ~IGfxRasterShader() override = default;
    };

    class IGfxVertexRasterShader : public IGfxRasterShader
    {
    public:
        enum class Slot : uint8_t
        {
            Vertex = 0,
            Fragment,
            TessellationControl,
            TessellationEvaluation,
            Geometry,
            Num,
        };

        IGfxVertexRasterShader() = default;
        virtual ~IGfxVertexRasterShader() override = default;

        static IGfxVertexRasterShader *Create(IGfxDevice *device);

        IGfxVertexRasterShader *SetVertexShader(StringView source);
        IGfxVertexRasterShader *SetFragmentShader(StringView source);
        IGfxVertexRasterShader *SetTessellationControlShader(StringView source);
        IGfxVertexRasterShader *SetTessellationEvaluationShader(StringView source);
        IGfxVertexRasterShader *SetGeometryShader(StringView source);

    protected:
        std::array<StringView, static_cast<uint8_t>(Slot::Num)> mShaderSources;
    };

    class IGfxMeshTaskRasterShader : public IGfxRasterShader
    {
    public:
        enum class Slot : uint8_t
        {
            Task = 0,
            Mesh,
            Fragment,
            Num
        };

        IGfxMeshTaskRasterShader() = default;
        virtual ~IGfxMeshTaskRasterShader() override = default;

        static IGfxMeshTaskRasterShader *Create(IGfxDevice *device);

        IGfxMeshTaskRasterShader *SetTaskShader(StringView source);
        IGfxMeshTaskRasterShader *SetMeshShader(StringView source);
        IGfxMeshTaskRasterShader *SetFragmentShader(StringView source);

    protected:
        std::array<StringView, static_cast<uint8_t>(Slot::Num)> mShaderSources;
    };

    class IGfxComputeShader : public IGfxShader
    {
    public:
        IGfxComputeShader() = default;
        virtual ~IGfxComputeShader() override = default;

        static IGfxComputeShader *Create(IGfxDevice *device);

        IGfxComputeShader *SetComputeShader(StringView source);

    protected:
        StringView mShaderSource;
    };
}