#pragma once
#include <string>
#include <string_view>
#include <unordered_map>
#include <memory>
#include "Core/Common.h"
#include "Render/Mesh.h"
#include "Gfx/IGfxTexture.h"
namespace RealSix
{
    class ResourceLoader : public Singleton<ResourceLoader>
    {
    public:
        IGfxTexture *GetTextureFromDisk(std::string_view path);
        std::vector<uint8_t> GetShaderContentFromDisk(std::string_view path);

        StaticMesh *GetBuiltinTriangleMesh();
        StaticMesh *GetBuiltinQuadMesh();
        StaticMesh *GetBuiltinCubeMesh();
        StaticMesh *GetBuiltinSphereMesh();
        StaticMesh *GetBuiltinCapsuleMesh();
        StaticMesh *GetBuiltinCylinderMesh();

    private:
        enum class MeshType
        {
            TRIANGLE,
            QUAD,
            CUBE,
            SPHERE,
            CAPSULE,
            CYLINDER,
        };

        StaticMesh *CreateBuiltinTriangle();
        StaticMesh *CreateBuiltinQuad();
        StaticMesh *CreateBuiltinCube();
        StaticMesh *CreateBuiltinSphere();
        StaticMesh *CreateBuiltinCapsule();
        StaticMesh *CreateBuiltinCylinder();

        std::unordered_map<MeshType, std::unique_ptr<StaticMesh>> mBuiltinMesheCache;
        std::unordered_map<std::string_view, std::vector<uint8_t>> mShaderContentCache;
        std::unordered_map<std::string_view, std::unique_ptr<IGfxTexture>> mTextureCache;
    };
}