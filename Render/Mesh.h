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
            return position == other.position && normal == other.normal && tangent == other.tangent && binormal == other.binormal && color == other.color && texcoord0 == other.texcoord0;
        }

        static GfxVertexBinding GetVertexBinding()
        {
            static GfxVertexBinding vertexBinding;
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
                attrib.format = GfxFormat::R32G32B32A32_SFLOAT;
                attrib.offset = offsetof(Vertex, color);
                vertexBinding.attribs.emplace_back(attrib);

                attrib.name = "TEXCOORD";
                attrib.format = GfxFormat::R32G32_SFLOAT;
                attrib.offset = offsetof(Vertex, texcoord0);
                vertexBinding.attribs.emplace_back(attrib);
            }
            return vertexBinding;
        }
    };

    enum class MeshType
    {
        TRIANGLE,
        QUAD,
        CUBE,
        SPHERE,
        CAPSULE,
        CYLINDER,
    };

    class Mesh
    {
    public:
        Mesh() = default;
        ~Mesh() = default;

        static Mesh *CreateBuiltinMesh(MeshType type);

        void SetVertices(const std::vector<Vertex> &vertices);
        void SetIndices(const std::vector<uint32_t> &indices);

        const std::vector<Vertex> &GetVertices() const { return mVertices; }
        const std::vector<uint32_t> &GetIndices() const { return mIndices; }

        const GfxVertexBuffer *GetVertexBuffer() const { return mVertexBuffer.get(); }

        bool HasIndices() const { return !mIndices.empty(); }
        const GfxIndexBuffer *GetIndexBuffer() const { return mIndexBuffer.get(); }

    private:
        static Mesh *CreateBuiltinTriangle();
        static Mesh *CreateBuiltinQuad();
        static Mesh *CreateBuiltinCube();
        static Mesh *CreateBuiltinSphere();
        static Mesh *CreateBuiltinCapsule();
        static Mesh *CreateBuiltinCylinder();

        std::vector<Vertex> mVertices{};
        std::vector<uint32_t> mIndices{};

        std::unique_ptr<GfxVertexBuffer> mVertexBuffer;
        std::unique_ptr<GfxIndexBuffer> mIndexBuffer;
    };
}