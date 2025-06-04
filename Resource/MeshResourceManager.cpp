#include "MeshResourceManager.h"

namespace RealSix
{
    StaticMesh *MeshResourceManager::GetBuiltinTriangleMesh()
    {
        if (mBuiltinMeshes.find(MeshType::TRIANGLE) == mBuiltinMeshes.end())
        {
            mBuiltinMeshes[MeshType::TRIANGLE] = std::unique_ptr<StaticMesh>(CreateBuiltinTriangle());
        }
        return mBuiltinMeshes[MeshType::TRIANGLE].get();
    }
    StaticMesh *MeshResourceManager::GetBuiltinQuadMesh()
    {
        if (mBuiltinMeshes.find(MeshType::QUAD) == mBuiltinMeshes.end())
        {
            mBuiltinMeshes[MeshType::QUAD] = std::unique_ptr<StaticMesh>(CreateBuiltinQuad());
        }
        return mBuiltinMeshes[MeshType::QUAD].get();
    }
    StaticMesh *MeshResourceManager::GetBuiltinCubeMesh()
    {
        if (mBuiltinMeshes.find(MeshType::CUBE) == mBuiltinMeshes.end())
        {
            mBuiltinMeshes[MeshType::CUBE] = std::unique_ptr<StaticMesh>(CreateBuiltinCube());
        }
        return mBuiltinMeshes[MeshType::CUBE].get();
    }
    StaticMesh *MeshResourceManager::GetBuiltinSphereMesh()
    {
        if (mBuiltinMeshes.find(MeshType::SPHERE) == mBuiltinMeshes.end())
        {
            mBuiltinMeshes[MeshType::SPHERE] = std::unique_ptr<StaticMesh>(CreateBuiltinSphere());
        }
        return mBuiltinMeshes[MeshType::SPHERE].get();
    }
    StaticMesh *MeshResourceManager::GetBuiltinCapsuleMesh()
    {
        if (mBuiltinMeshes.find(MeshType::CAPSULE) == mBuiltinMeshes.end())
        {
            mBuiltinMeshes[MeshType::CAPSULE] = std::unique_ptr<StaticMesh>(CreateBuiltinCapsule());
        }
        return mBuiltinMeshes[MeshType::CAPSULE].get();
    }
    StaticMesh *MeshResourceManager::GetBuiltinCylinderMesh()
    {
        if (mBuiltinMeshes.find(MeshType::CYLINDER) == mBuiltinMeshes.end())
        {
            mBuiltinMeshes[MeshType::CYLINDER] = std::unique_ptr<StaticMesh>(CreateBuiltinCylinder());
        }
        return mBuiltinMeshes[MeshType::CYLINDER].get();
    }

    StaticMesh *MeshResourceManager::CreateBuiltinTriangle()
    {
        Vector3f positions[] =
            {
                Vector3f(-1.0, 1.0, 0.0),
                Vector3f(1.0, 1.0, 0.0),
                Vector3f(0.0, -1.0, 0.0),
            };

        Vector3f normals[] =
            {
                Vector3f(0.0f, 0.0f, 1.0f),
                Vector3f(0.0f, 0.0f, 1.0f),
                Vector3f(0.0f, 0.0f, 1.0f),
            };

        Vector3f tangents[] =
            {
                Vector3f(1.0, 0.0, 0.0),
                Vector3f(1.0, 0.0, 0.0),
                Vector3f(1.0, 0.0, 0.0),
            };
        Vector3f binormals[] =
            {
                Vector3f(0.0, 1.0, 0.0),
                Vector3f(0.0, 1.0, 0.0),
                Vector3f(0.0, 1.0, 0.0),
            };

        Vector4f colors[] =
            {
                Vector4f(1.0, 0.0, 0.0, 1.0),
                Vector4f(0.0, 1.0, 0.0, 1.0),
                Vector4f(0.0, 0.0, 1.0, 1.0),
            };

        Vector2f texcoord0s[] =
            {
                Vector2f(0.0, 0.0),
                Vector2f(1.0, 0.0),
                Vector2f(0.5, 1.0),
            };

        std::vector<Vertex> vertices(3);
        for (size_t i = 0; i < vertices.size(); ++i)
        {
            vertices[i].position = positions[i];
            vertices[i].normal = normals[i];
            vertices[i].tangent = tangents[i];
            vertices[i].binormal = binormals[i];
            vertices[i].color = colors[i];
            vertices[i].texcoord0 = texcoord0s[i];
        }

        std::vector<uint32_t> indices = {0, 1, 2};

        StaticMesh *result = new StaticMesh();
        result->SetVertices(vertices);
        result->SetIndices(indices);

        return result;
    }

    StaticMesh *MeshResourceManager::CreateBuiltinQuad()
    {
        const Vector3f positions[] =
            {
                Vector3f(-1.0f, 1.0f, 0.0f),
                Vector3f(-1.0f, -1.0f, 0.0f),
                Vector3f(1.0f, -1.0f, 0.0f),
                Vector3f(1.0f, 1.0f, 0.0f),
            };

        const Vector3f normals[] =
            {
                Vector3f(0.0f, 0.0f, 1.0f),
                Vector3f(0.0f, 0.0f, 1.0f),
                Vector3f(0.0f, 0.0f, 1.0f),
                Vector3f(0.0f, 0.0f, 1.0f),
            };

        const Vector3f tangents[] =
            {
                Vector3f(1.0, 0.0, 0.0),
                Vector3f(1.0, 0.0, 0.0),
                Vector3f(1.0, 0.0, 0.0),
                Vector3f(1.0, 0.0, 0.0),
            };
        const Vector3f binormals[] =
            {
                Vector3f(0.0, 1.0, 0.0),
                Vector3f(0.0, 1.0, 0.0),
                Vector3f(0.0, 1.0, 0.0),
                Vector3f(0.0, 1.0, 0.0),
            };

        const Vector2f texcoord0s[] =
            {
                Vector2f(0.0f, 1.0f),
                Vector2f(0.0f, 0.0f),
                Vector2f(1.0f, 0.0f),
                Vector2f(1.0f, 1.0f),
            };

        std::vector<Vertex> vertices(4);
        for (size_t i = 0; i < vertices.size(); ++i)
        {
            vertices[i].position = positions[i];
            vertices[i].normal = normals[i];
            vertices[i].tangent = tangents[i];
            vertices[i].binormal = binormals[i];
            vertices[i].color = Vector4f(1.0f, 1.0f, 1.0f, 1.0f);
            vertices[i].texcoord0 = texcoord0s[i];
        }

        std::vector<uint32_t> indices = {0, 1, 2, 0, 2, 3};

        StaticMesh *result = new StaticMesh();
        result->SetVertices(vertices);
        result->SetIndices(indices);
        return result;
    }

    StaticMesh *MeshResourceManager::CreateBuiltinCube()
    {
        const Vector3f positions[] =
            {
                //+z
                Vector3f(-0.5f, 0.5f, 0.5f),
                Vector3f(-0.5f, -0.5f, 0.5f),
                Vector3f(0.5f, -0.5f, 0.5f),
                Vector3f(0.5f, 0.5f, 0.5f),
                //-z
                Vector3f(0.5f, 0.5f, -0.5f),
                Vector3f(0.5f, -0.5f, -0.5f),
                Vector3f(-0.5f, -0.5f, -0.5f),
                Vector3f(-0.5f, 0.5f, -0.5f),
                //+x
                Vector3f(0.5f, 0.5f, 0.5f),
                Vector3f(0.5f, -0.5f, 0.5f),
                Vector3f(0.5f, -0.5f, -0.5f),
                Vector3f(0.5f, 0.5f, -0.5f),
                //-x
                Vector3f(-0.5f, 0.5f, -0.5f),
                Vector3f(-0.5f, -0.5f, -0.5f),
                Vector3f(-0.5f, -0.5f, 0.5f),
                Vector3f(-0.5f, 0.5f, 0.5f),
                //+y
                Vector3f(-0.5f, 0.5f, -0.5f),
                Vector3f(-0.5f, 0.5f, 0.5f),
                Vector3f(0.5f, 0.5f, 0.5f),
                Vector3f(0.5f, 0.5f, -0.5f),
                //-y
                Vector3f(-0.5f, -0.5f, 0.5f),
                Vector3f(-0.5f, -0.5f, -0.5f),
                Vector3f(0.5f, -0.5f, -0.5f),
                Vector3f(0.5f, -0.5f, 0.5f),
            };

        const Vector3f normals[] =
            {
                //+z
                Vector3f(0.0f, 0.0f, 1.0f),
                Vector3f(0.0f, 0.0f, 1.0f),
                Vector3f(0.0f, 0.0f, 1.0f),
                Vector3f(0.0f, 0.0f, 1.0f),
                //-z
                Vector3f(0.0f, 0.0f, -1.0f),
                Vector3f(0.0f, 0.0f, -1.0f),
                Vector3f(0.0f, 0.0f, -1.0f),
                Vector3f(0.0f, 0.0f, -1.0f),
                //+x
                Vector3f(1.0f, 0.0f, 0.0f),
                Vector3f(1.0f, 0.0f, 0.0f),
                Vector3f(1.0f, 0.0f, 0.0f),
                Vector3f(1.0f, 0.0f, 0.0f),
                //-x
                Vector3f(-1.0f, 0.0f, 0.0f),
                Vector3f(-1.0f, 0.0f, 0.0f),
                Vector3f(-1.0f, 0.0f, 0.0f),
                Vector3f(-1.0f, 0.0f, 0.0f),
                //+y
                Vector3f(0.0f, 1.0f, 0.0f),
                Vector3f(0.0f, 1.0f, 0.0f),
                Vector3f(0.0f, 1.0f, 0.0f),
                Vector3f(0.0f, 1.0f, 0.0f),
                //-y
                Vector3f(0.0f, -1.0f, 0.0f),
                Vector3f(0.0f, -1.0f, 0.0f),
                Vector3f(0.0f, -1.0f, 0.0f),
                Vector3f(0.0f, -1.0f, 0.0f),
            };

        const Vector2f texcoord0s[] =
            {
                //+z
                Vector2f(0.0f, 1.0f),
                Vector2f(0.0f, 0.0f),
                Vector2f(1.0f, 0.0f),
                Vector2f(1.0f, 1.0f),
                //-z
                Vector2f(0.0f, 1.0f),
                Vector2f(0.0f, 0.0f),
                Vector2f(1.0f, 0.0f),
                Vector2f(1.0f, 1.0f),
                //+x
                Vector2f(0.0f, 1.0f),
                Vector2f(0.0f, 0.0f),
                Vector2f(1.0f, 0.0f),
                Vector2f(1.0f, 1.0f),
                //-x
                Vector2f(0.0f, 1.0f),
                Vector2f(0.0f, 0.0f),
                Vector2f(1.0f, 0.0f),
                Vector2f(1.0f, 1.0f),
                //+y
                Vector2f(0.0f, 1.0f),
                Vector2f(0.0f, 0.0f),
                Vector2f(1.0f, 0.0f),
                Vector2f(1.0f, 1.0f),
                //-y
                Vector2f(0.0f, 1.0f),
                Vector2f(0.0f, 0.0f),
                Vector2f(1.0f, 0.0f),
                Vector2f(1.0f, 1.0f),
            };

        const Vector3f tangents[] =
            {
                //+z
                Vector3f(1.0f, 0.0f, 0.0f),
                Vector3f(1.0f, 0.0f, 0.0f),
                Vector3f(1.0f, 0.0f, 0.0f),
                Vector3f(1.0f, 0.0f, 0.0f),
                //-z
                Vector3f(-1.0f, 0.0f, 0.0f),
                Vector3f(-1.0f, 0.0f, 0.0f),
                Vector3f(-1.0f, 0.0f, 0.0f),
                Vector3f(-1.0f, 0.0f, 0.0f),
                //+x
                Vector3f(0.0f, 0.0f, -1.0f),
                Vector3f(0.0f, 0.0f, -1.0f),
                Vector3f(0.0f, 0.0f, -1.0f),
                Vector3f(0.0f, 0.0f, -1.0f),
                //-x
                Vector3f(0.0f, 0.0f, 1.0f),
                Vector3f(0.0f, 0.0f, 1.0f),
                Vector3f(0.0f, 0.0f, 1.0f),
                Vector3f(0.0f, 0.0f, 1.0f),
                //+y
                Vector3f(1.0f, 0.0f, 0.0f),
                Vector3f(1.0f, 0.0f, 0.0f),
                Vector3f(1.0f, 0.0f, 0.0f),
                Vector3f(1.0f, 0.0f, 0.0f),
                //-y
                Vector3f(1.0f, 0.0f, 0.0f),
                Vector3f(1.0f, 0.0f, 0.0f),
                Vector3f(1.0f, 0.0f, 0.0f),
                Vector3f(1.0f, 0.0f, 0.0f),
            };

        const Vector3f binormals[] = {
            //+z
            Vector3f(0.0f, 1.0f, 0.0f),
            Vector3f(0.0f, 1.0f, 0.0f),
            Vector3f(0.0f, 1.0f, 0.0f),
            Vector3f(0.0f, 1.0f, 0.0f),
            //-z
            Vector3f(0.0f, 1.0f, 0.0f),
            Vector3f(0.0f, 1.0f, 0.0f),
            Vector3f(0.0f, 1.0f, 0.0f),
            Vector3f(0.0f, 1.0f, 0.0f),
            //+x
            Vector3f(0.0f, 1.0f, 0.0f),
            Vector3f(0.0f, 1.0f, 0.0f),
            Vector3f(0.0f, 1.0f, 0.0f),
            Vector3f(0.0f, 1.0f, 0.0f),
            //-x
            Vector3f(0.0f, 1.0f, 0.0f),
            Vector3f(0.0f, 1.0f, 0.0f),
            Vector3f(0.0f, 1.0f, 0.0f),
            Vector3f(0.0f, 1.0f, 0.0f),
            //+y
            Vector3f(0.0f, 0.0f, -1.0f),
            Vector3f(0.0f, 0.0f, -1.0f),
            Vector3f(0.0f, 0.0f, -1.0f),
            Vector3f(0.0f, 0.0f, -1.0f),
            //-y
            Vector3f(0.0f, 0.0f, 1.0f),
            Vector3f(0.0f, 0.0f, 1.0f),
            Vector3f(0.0f, 0.0f, 1.0f),
            Vector3f(0.0f, 0.0f, 1.0f),
        };

        std::vector<uint32_t> indices =
            {
                0,
                1,
                2,
                0,
                2,
                3,

                4,
                5,
                6,
                4,
                6,
                7,

                8,
                9,
                10,
                8,
                10,
                11,

                12,
                13,
                14,
                12,
                14,
                15,

                16,
                17,
                18,
                16,
                18,
                19,

                20,
                21,
                22,
                20,
                22,
                23,
            };

        std::vector<Vertex> vertices(24);
        for (size_t i = 0; i < vertices.size(); ++i)
        {
            vertices[i].position = positions[i];
            vertices[i].normal = normals[i];
            vertices[i].tangent = tangents[i];
            vertices[i].binormal = binormals[i];
            vertices[i].color = Vector4f(1.0f, 1.0f, 1.0f, 1.0f);
            vertices[i].texcoord0 = texcoord0s[i];
        }

        StaticMesh *result = new StaticMesh();
        result->SetVertices(vertices);
        result->SetIndices(indices);
        return result;
    }

    StaticMesh *MeshResourceManager::CreateBuiltinSphere()
    {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        float latitudeBands = 64.0f;
        float longitudeBands = 64.0f;
        float radius = 1.0f;

        for (float latNumber = 0; latNumber <= latitudeBands; latNumber++)
        {
            float theta = latNumber * Math::PI / latitudeBands;
            float sinTheta = sin(theta);
            float cosTheta = cos(theta);

            for (float longNumber = 0; longNumber <= longitudeBands; longNumber++)
            {
                float phi = longNumber * Math::TWO_PI / longitudeBands;
                float sinPhi = sin(phi);
                float cosPhi = cos(phi);

                Vector3f tmp = Vector3f(cosPhi * sinTheta, cosTheta, sinPhi * sinTheta);
                Vector3f tangent = Vector3f(cosPhi, 0.0f, -sinPhi);

                Vertex vertex;

                vertex.position = tmp * radius;
                vertex.normal = tmp;
                vertex.tangent = tangent;
                vertex.binormal = Vector3f::Cross(tmp, tangent);
                vertex.color = Vector4f(1.0f, 1.0f, 1.0f, 1.0f);
                vertex.texcoord0 = Vector2f((longNumber / longitudeBands), (latNumber / latitudeBands));

                vertices.emplace_back(vertex);
            }
        }

        for (uint32_t latNumber = 0; latNumber < latitudeBands; latNumber++)
        {
            for (uint32_t longNumber = 0; longNumber < longitudeBands; longNumber++)
            {
                uint32_t first = (latNumber * (longitudeBands + 1)) + longNumber;
                uint32_t second = first + longitudeBands + 1;

                indices.emplace_back(first);
                indices.emplace_back(second);
                indices.emplace_back(first + 1);

                indices.emplace_back(second);
                indices.emplace_back(second + 1);
                indices.emplace_back(first + 1);
            }
        }

        StaticMesh *result = new StaticMesh();
        result->SetVertices(vertices);
        result->SetIndices(indices);
        return result;
    }

    StaticMesh *MeshResourceManager::CreateBuiltinCapsule()
    {
        return nullptr; // TODO: Implementation not ready
    }

    StaticMesh *MeshResourceManager::CreateBuiltinCylinder()
    {

        // TODO: not finifhed yet
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;

        float h = 1.0f, r = 1.0f;
        float PI = 3.14159f;
        int index = 0;
        for (float i = 0.0f; i < Math::PI * 2.0f; i += Math::PI / 8.0f)
        {
            Vertex vertex;
            vertex.position = Vector3f(cos(i), h, sin(i));
            vertices.emplace_back(vertex);
            vertex.position = Vector3f(cos(i), -h, sin(i));
            vertices.emplace_back(vertex);

            indices.emplace_back(index++);
            indices.emplace_back(index++);
        }
        for (int i = 1; i < vertices.size(); i += 2)
            indices.emplace_back(i);

        for (int i = 0; i < vertices.size(); i += 2)
            indices.emplace_back(i);

        StaticMesh *result = new StaticMesh();
        result->SetVertices(vertices);
        result->SetIndices(indices);
        return result;
    }

}