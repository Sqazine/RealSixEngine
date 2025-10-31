#pragma once
#include "IGfxDevice.h"
#include "IGfxTexture.h"
#include "IGfxBuffer.h"
namespace RealSix
{
    class IGfxShader
    {
    public:
        IGfxShader() = default;
        virtual ~IGfxShader() = default;

        virtual IGfxShader *BindBuffer(std::string_view name, const IGfxBuffer *buffer) = 0;
        virtual IGfxShader *BindTexture(std::string_view name, const IGfxTexture *texture) = 0;
    };
    class IGfxRasterShader : public IGfxShader
    {
    public:
        IGfxRasterShader() = default;
        virtual ~IGfxRasterShader() = default;

        static IGfxRasterShader *Create(IGfxDevice *device,
                                        const std::vector<uint8_t> &vertContent,
                                        const std::vector<uint8_t> &fragContent,
                                        const std::vector<uint8_t> &tessCtrlContent = {},
                                        const std::vector<uint8_t> &tessEvalContent = {},
                                        const std::vector<uint8_t> &geomContent = {});
    };

    class IGfxComputeShader : public IGfxShader
    {
    public:
        IGfxComputeShader() = default;
        virtual ~IGfxComputeShader() = default;

        static IGfxComputeShader *Create(IGfxDevice *device,
                                        const std::vector<uint8_t> &compContent);
    };
}