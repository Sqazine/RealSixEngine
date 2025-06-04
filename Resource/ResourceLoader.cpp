#include "ResourceLoader.hpp"
#include "IO.hpp"
namespace RealSix
{
    StaticMesh *ResourceLoader::GetBuiltinTriangleMesh()
    {
        if (mBuiltinMesheCache.find(MeshType::TRIANGLE) == mBuiltinMesheCache.end())
        {
            mBuiltinMesheCache[MeshType::TRIANGLE] = std::unique_ptr<StaticMesh>(CreateBuiltinTriangle());
        }
        return mBuiltinMesheCache[MeshType::TRIANGLE].get();
    }
    StaticMesh *ResourceLoader::GetBuiltinQuadMesh()
    {
        if (mBuiltinMesheCache.find(MeshType::QUAD) == mBuiltinMesheCache.end())
        {
            mBuiltinMesheCache[MeshType::QUAD] = std::unique_ptr<StaticMesh>(CreateBuiltinQuad());
        }
        return mBuiltinMesheCache[MeshType::QUAD].get();
    }
    StaticMesh *ResourceLoader::GetBuiltinCubeMesh()
    {
        if (mBuiltinMesheCache.find(MeshType::CUBE) == mBuiltinMesheCache.end())
        {
            mBuiltinMesheCache[MeshType::CUBE] = std::unique_ptr<StaticMesh>(CreateBuiltinCube());
        }
        return mBuiltinMesheCache[MeshType::CUBE].get();
    }
    StaticMesh *ResourceLoader::GetBuiltinSphereMesh()
    {
        if (mBuiltinMesheCache.find(MeshType::SPHERE) == mBuiltinMesheCache.end())
        {
            mBuiltinMesheCache[MeshType::SPHERE] = std::unique_ptr<StaticMesh>(CreateBuiltinSphere());
        }
        return mBuiltinMesheCache[MeshType::SPHERE].get();
    }
    StaticMesh *ResourceLoader::GetBuiltinCapsuleMesh()
    {
        if (mBuiltinMesheCache.find(MeshType::CAPSULE) == mBuiltinMesheCache.end())
        {
            mBuiltinMesheCache[MeshType::CAPSULE] = std::unique_ptr<StaticMesh>(CreateBuiltinCapsule());
        }
        return mBuiltinMesheCache[MeshType::CAPSULE].get();
    }
    StaticMesh *ResourceLoader::GetBuiltinCylinderMesh()
    {
        if (mBuiltinMesheCache.find(MeshType::CYLINDER) == mBuiltinMesheCache.end())
        {
            mBuiltinMesheCache[MeshType::CYLINDER] = std::unique_ptr<StaticMesh>(CreateBuiltinCylinder());
        }
        return mBuiltinMesheCache[MeshType::CYLINDER].get();
    }

    StaticMesh *ResourceLoader::CreateBuiltinTriangle()
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

        std::vector<StaticVertex> vertices(3);
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

    StaticMesh *ResourceLoader::CreateBuiltinQuad()
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

        std::vector<StaticVertex> vertices(4);
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

    StaticMesh *ResourceLoader::CreateBuiltinCube()
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

        std::vector<StaticVertex> vertices(24);
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

    StaticMesh *ResourceLoader::CreateBuiltinSphere()
    {
        std::vector<StaticVertex> vertices;
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

                StaticVertex vertex;

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

    StaticMesh *ResourceLoader::CreateBuiltinCapsule()
    {
        return nullptr; // TODO: Implementation not ready
    }

    StaticMesh *ResourceLoader::CreateBuiltinCylinder()
    {

        // TODO: not finifhed yet
        std::vector<StaticVertex> vertices;
        std::vector<uint32_t> indices;

        float h = 1.0f, r = 1.0f;
        float PI = 3.14159f;
        int index = 0;
        for (float i = 0.0f; i < Math::PI * 2.0f; i += Math::PI / 8.0f)
        {
            StaticVertex vertex;
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

    String ResourceLoader::GetShaderContentFromDisk(StringView path)
    {
        if (mShaderContentCache.find(path) == mShaderContentCache.end())
            mShaderContentCache[path] = ReadTextFile(path);
        return mShaderContentCache[path];
    }

    IGfxTexture *ResourceLoader::GetTextureFromDisk(StringView path)
    {
        if (mTextureCache.find(path) == mTextureCache.end())
        {
            GfxTextureDesc textureDesc = ReadTexture(path);
            mTextureCache[path].reset(IGfxTexture::Create(Renderer::GetGfxDevice(), textureDesc));
        }
        return mTextureCache[path].get();
    }
}