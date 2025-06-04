#pragma once
#include <vector>
#include <memory>
#include <type_traits>
#include "Math/Vector3.h"
#include "Math/Vector2.h"
#include "Math/Vector4.h"
#include "Gfx/IGfxCommon.h"
#include "Gfx/IGfxBuffer.h"
#include "Gfx/IGfxPipeline.h"
#include "Render/Renderer.h"
namespace RealSix
{
    struct Vertex
    {
        alignas(16) Vector3f position;
        alignas(16) Vector3f normal;
        alignas(16) Vector3f tangent;
        alignas(16) Vector3f binormal;
        alignas(16) Vector4f color;
        alignas(8) Vector2f texcoord0;

        bool operator==(const Vertex &other) const
        {
            return position == other.position &&
                   normal == other.normal &&
                   tangent == other.tangent &&
                   binormal == other.binormal &&
                   color == other.color &&
                   texcoord0 == other.texcoord0;
        }

        static GfxVertexInputBinding GetVertexInputBinding()
        {
            static GfxVertexInputBinding vertexBinding;
            vertexBinding.vertexInputType = GfxVertexInputType::PER_VERTEX;

            static bool isInit = false;

            if (!isInit)
            {
                isInit = true;

                vertexBinding.bindingPoint = 0;
                vertexBinding.size = sizeof(Vertex);

                GfxVertexAttribute attrib;
                attrib.name = "POSITION";
                attrib.format = GfxFormat::R32G32B32_SFLOAT;
                attrib.offset = offsetof(Vertex, position);
                vertexBinding.attribs.emplace_back(attrib);

                attrib.name = "NORMAL";
                attrib.format = GfxFormat::R32G32B32_SFLOAT;
                attrib.offset = offsetof(Vertex, normal);
                vertexBinding.attribs.emplace_back(attrib);

                attrib.name = "TANGENT";
                attrib.format = GfxFormat::R32G32B32_SFLOAT;
                attrib.offset = offsetof(Vertex, tangent);
                vertexBinding.attribs.emplace_back(attrib);

                attrib.name = "BINORMAL";
                attrib.format = GfxFormat::R32G32B32_SFLOAT;
                attrib.offset = offsetof(Vertex, binormal);
                vertexBinding.attribs.emplace_back(attrib);

                attrib.name = "COLOR";
                attrib.format = GfxFormat::R8G8B8A8_UNORM;
                attrib.offset = offsetof(Vertex, color);
                vertexBinding.attribs.emplace_back(attrib);

                attrib.name = "TEXCOORD0";
                attrib.format = GfxFormat::R32G32_SFLOAT;
                attrib.offset = offsetof(Vertex, texcoord0);
                vertexBinding.attribs.emplace_back(attrib);
            }
            return vertexBinding;
        }
    };

    struct SkeletalVertex : public Vertex
    {
        alignas(16) Vector4f boneWeights;
        alignas(16) Vector4u32 boneIndices;

        static GfxVertexInputBinding GetVertexInputBinding()
        {
            static GfxVertexInputBinding vertexBinding = Vertex::GetVertexInputBinding();

            static bool isInit = false;

            if (!isInit)
            {
                isInit = true;

                GfxVertexAttribute attrib;
                attrib.name = "BONE_INDICES";
                attrib.format = GfxFormat::R32G32B32A32_UINT;
                attrib.offset = offsetof(SkeletalVertex, boneIndices);
                vertexBinding.attribs.emplace_back(attrib);

                attrib.name = "BONE_WEIGHTS";
                attrib.format = GfxFormat::R32G32B32A32_SFLOAT;
                attrib.offset = offsetof(SkeletalVertex, boneWeights);
                vertexBinding.attribs.emplace_back(attrib);
            }
            return vertexBinding;
        }
    };

    template <typename VertexType, typename IndexType>
    class Mesh
    {
    public:
        Mesh() = default;
        ~Mesh() = default;

        GfxVertexInputBinding GetVertexInputBinding()
        {
            return VertexType::GetVertexInputBinding();
        }

        void SetVertices(const std::vector<VertexType> &vertices)
        {
            if (vertices == mVertices)
                return;

            mVertices = vertices;

            GfxBufferDesc gfxDesc{};
            gfxDesc.elementSize = sizeof(VertexType);
            gfxDesc.bufferSize = mVertices.size() * gfxDesc.elementSize;
            gfxDesc.data = mVertices.data();

            mVertexBuffer.reset(GfxVertexBuffer::Create(Renderer::GetGfxDevice(), gfxDesc));
        }

        void SetIndices(const std::vector<IndexType> &indices)
        {
            if (indices == mIndices)
                return;

            mIndices = indices;

            GfxBufferDesc gfxDesc{};
            gfxDesc.elementSize = sizeof(IndexType);
            gfxDesc.bufferSize = mIndices.size() * gfxDesc.elementSize;
            gfxDesc.data = mIndices.data();

            mIndexBuffer.reset(GfxIndexBuffer::Create(Renderer::GetGfxDevice(), gfxDesc));
        }

        const std::vector<VertexType> &GetVertices() const { return mVertices; }
        const std::vector<IndexType> &GetIndices() const { return mIndices; }

        bool HasIndices() const { return !mIndices.empty(); }

        const GfxVertexBuffer *GetVertexBuffer() const { return mVertexBuffer.get(); }
        const GfxIndexBuffer *GetIndexBuffer() const { return mIndexBuffer.get(); }

    private:
        std::vector<VertexType> mVertices{};
        std::vector<IndexType> mIndices{};

        std::unique_ptr<GfxVertexBuffer> mVertexBuffer;
        std::unique_ptr<GfxIndexBuffer> mIndexBuffer;
    };

    using StaticMesh = Mesh<Vertex, uint32_t>;
    using SkeletalMesh = Mesh<SkeletalVertex, uint32_t>;
}