#pragma once
#include "String.hpp"

#include <unordered_map>
#include <memory>
#include "Core/Common.hpp"
#include "Render/Mesh.hpp"
#include "Gfx/IGfxTexture.hpp"
namespace RealSix
{
    class ResourceLoader : public Singleton<ResourceLoader>
    {
    public:
        IGfxTexture *GetTextureFromDisk(StringView path);
       String GetShaderContentFromDisk(StringView path);

        std::vector<StaticMesh> LoadStaticMeshesFromDisk(StringView path);
        std::vector<SkeletalMesh> LoadSkeletalMeshesFromDisk(StringView path);

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
        std::unordered_map<StringView, String> mShaderContentCache;
        std::unordered_map<StringView, std::unique_ptr<IGfxTexture>> mTextureCache;
    };
}