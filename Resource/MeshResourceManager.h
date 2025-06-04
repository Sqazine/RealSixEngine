#pragma once
#include <unordered_map>
#include "Render/Mesh.h"
#include "Core/Singleton.h"
namespace RealSix
{
    class MeshResourceManager : public Singleton<MeshResourceManager>
    {
    public:
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

        std::unordered_map<MeshType, std::unique_ptr<StaticMesh>> mBuiltinMeshes;
    };
}