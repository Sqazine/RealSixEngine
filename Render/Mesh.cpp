#include "Mesh.h"
#include <vector>
#include "Render/Renderer.h"
#include "Logger/Logger.h"
namespace RealSix
{
    Mesh *Mesh::CreateBuiltinMesh(MeshType type)
    {
        switch (type)
        {
        case MeshType::TRIANGLE:
            return CreateBuiltinTriangle();
        case MeshType::QUAD:
            return CreateBuiltinQuad();
        case MeshType::CUBE:
            return CreateBuiltinCube();
        case MeshType::SPHERE:
            return CreateBuiltinSphere();
        case MeshType::CAPSULE:
            return CreateBuiltinCapsule();
        case MeshType::CYLINDER:
            return CreateBuiltinCylinder();
        default:
            REALSIX_LOG_ERROR(TEXT("Unsupported mesh type!"));
            return nullptr;
        }
    }
    void Mesh::SetVertices(const std::vector<Vertex> &vertices)
    {
        if (vertices == mVertices)
            return;

        mVertices = vertices;

        GfxBufferDesc gfxDesc{};
        gfxDesc.elementSize = sizeof(Vertex);
        gfxDesc.bufferSize = mVertices.size() * gfxDesc.elementSize;
        gfxDesc.data = mVertices.data();

        mVertexBuffer.reset(GfxVertexBuffer::Create(Renderer::GetGfxDevice(), gfxDesc));
    }
    void Mesh::SetIndices(const std::vector<uint32_t> &indices)
    {
        if (indices == mIndices)
            return;

        mIndices = indices;

        GfxBufferDesc gfxDesc{};
        gfxDesc.elementSize = sizeof(uint32_t);
        gfxDesc.bufferSize = mIndices.size() * gfxDesc.elementSize;
        gfxDesc.data = mIndices.data();

        mIndexBuffer.reset(GfxIndexBuffer::Create(Renderer::GetGfxDevice(), gfxDesc));
    }

    Mesh *Mesh::CreateBuiltinTriangle()
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

        Mesh *result = new Mesh();
        result->SetVertices(vertices);
        result->SetIndices(indices);

        return result;
    }

    Mesh *Mesh::CreateBuiltinQuad()
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

        Mesh *result = new Mesh();
        result->SetVertices(vertices);
        result->SetIndices(indices);
        return result;
    }

    Mesh *Mesh::CreateBuiltinCube()
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

        Mesh *result = new Mesh();
        result->SetVertices(vertices);
        result->SetIndices(indices);
        return result;
    }

    Mesh *Mesh::CreateBuiltinSphere()
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

        Mesh *result = new Mesh();
        result->SetVertices(vertices);
        result->SetIndices(indices);
        return result;
    }

    Mesh *Mesh::CreateBuiltinCapsule()
    {
        return nullptr; // TODO: Implementation not ready
    }

    Mesh *Mesh::CreateBuiltinCylinder()
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

        Mesh *result = new Mesh();
        result->SetVertices(vertices);
        result->SetIndices(indices);
        return result;
    }

}