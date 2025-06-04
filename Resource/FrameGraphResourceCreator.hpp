#pragma once
#include "IGfxBuffer.hpp"
#include "IGfxTexture.hpp"

namespace RealSix
{
    template <>
    inline std::unique_ptr<GfxVertexBuffer> Realize(const std::vector<Vertex> &vertices)
    {
        return std::make_unique<VertexBuffer>(vertices);
    }

    template <>
    inline std::unique_ptr<GfxIndexBuffer> Realize(const std::vector<uint32_t> &indices)
    {
        return std::make_unique<IndexBuffer>(indices);
    }

    template <>
    inline std::unique_ptr<GfxUniformBuffer> Realize(const UniformBufferDesc &desc)
    {
        return std::make_unique<UniformBuffer>(desc);
    }
}