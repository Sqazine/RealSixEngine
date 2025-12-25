#include "GLTFLoader.hpp"
#include <iostream>
#include <vector>
#include "Math/Matrix4.hpp"
#include "Math/Transform.hpp"
#include "Animation/Interpolation.hpp"

namespace RealSix::GLTFHelpers
{
    Transform3f GetLocalTransform(const cgltf_node &n);
    int GetNodeIndex(cgltf_node *target, cgltf_node *allNodes, uint32_t numNodes);

    std::vector<float> GetScalarValues(uint32_t compCount, const cgltf_accessor &inAccessor);

    template <typename T, int32_t N>
    inline void TrackFromChannel(Track<T, N> &result, const cgltf_animation_channel &channel)
    {
        cgltf_animation_sampler &sampler = *channel.sampler;
        Interpolation interpolation = Interpolation::Constant;
        if (sampler.interpolation == cgltf_interpolation_type_linear)
            interpolation = Interpolation::Linear;
        else if (sampler.interpolation == cgltf_interpolation_type_cubic_spline)
            interpolation = Interpolation::Cubic;
        bool isSamplerCubic = interpolation == Interpolation::Cubic;

        result.SetFrameInterpolation(interpolation);

        std::vector<float> time = GLTFHelpers::GetScalarValues(1, *sampler.input);

        std::vector<float> value = GLTFHelpers::GetScalarValues(N, *sampler.output);

        uint32_t numFrames = sampler.input->count;
        uint32_t compCount = value.size() / time.size();
        result.ResizeFrames(numFrames);

        for (uint32_t i = 0; i < numFrames; ++i)
        {
            int baseIndex = i * compCount;
            int offset = 0;
            result[i].mTime = time[i];
            for (int comp = 0; comp < N; ++comp)
                result[i].mInTangentSlope[comp] = isSamplerCubic ? value[baseIndex + offset++] : 0.0f;
            for (int comp = 0; comp < N; ++comp)
                result[i].mValue[comp] = value[baseIndex + offset++];
            for (int comp = 0; comp < N; ++comp)
                result[i].mOutTangentSlope[comp] = isSamplerCubic ? value[baseIndex + offset++] : 0.0f;
        }
    }

    Transform3f GLTFHelpers::GetLocalTransform(const cgltf_node &n)
    {
        Transform3f result;
        if (n.has_matrix)
        {
            Matrix4f mat4(n.matrix[0], n.matrix[1], n.matrix[2], n.matrix[3],
                          n.matrix[4], n.matrix[5], n.matrix[6], n.matrix[7],
                          n.matrix[8], n.matrix[9], n.matrix[10], n.matrix[11],
                          n.matrix[12], n.matrix[13], n.matrix[14], n.matrix[15]);
            result = Matrix4f::ToTransform(mat4);
        }

        if (n.has_translation)
            result.position = Vector3f(n.translation[0], n.translation[1], n.translation[2]);

        if (n.has_rotation)
            result.rotation = Quaternionf(n.rotation[0], n.rotation[1], n.rotation[2], n.rotation[3]);

        if (n.has_scale)
            result.scale = Vector3f(n.scale[0], n.scale[1], n.scale[2]);

        return result;
    }

    int GLTFHelpers::GetNodeIndex(cgltf_node *target, cgltf_node *allNodes, uint32_t numNodes)
    {
        if (target == 0)
            return -1;

        for (uint32_t i = 0; i < numNodes; ++i)
        {
            if (target == &allNodes[i])
                return (int)i;
        }
        return -1;
    }

    std::vector<float> GLTFHelpers::GetScalarValues(uint32_t compCount, const cgltf_accessor &inAccessor)
    {
        std::vector<float> out;
        out.resize(inAccessor.count * compCount);
        for (cgltf_size i = 0; i < inAccessor.count; ++i)
            cgltf_accessor_read_float(&inAccessor, i, &out[i * compCount], compCount);
        return out;
    }
}

namespace RealSix::GLTFLoader
{
    cgltf_data *LoadGLTFFile(const String &path)
    {
        cgltf_options options;
        memset(&options, 0, sizeof(cgltf_options));
        cgltf_data *data = nullptr;
        cgltf_result result = cgltf_parse_file(&options, path.CString(), &data);
        if (result != cgltf_result_success)
        {
            REALSIX_LOG_ERROR("Failed to open file:{}", path);
            return nullptr;
        }
        result = cgltf_load_buffers(&options, data, path.CString());
        if (result != cgltf_result_success)
        {
            cgltf_free(data);
            REALSIX_LOG_ERROR("Failed to open file:{}", path);
            return nullptr;
        }
        result = cgltf_validate(data);
        if (result != cgltf_result_success)
        {
            cgltf_free(data);
            REALSIX_LOG_ERROR("Invalid file:{}", path);
            return nullptr;
        }
        return data;
    }
    void FreeGLTFFile(cgltf_data *data)
    {
        if (!data)
            REALSIX_LOG_ERROR("WARNING:Can't free null data");
        else
            cgltf_free(data);
    }

    Pose LoadRestPose(cgltf_data *data)
    {
        uint32_t boneCount = data->nodes_count;

        Pose result(boneCount);

        for (uint32_t i = 0; i < boneCount; ++i)
        {
            cgltf_node *node = &(data->nodes[i]);
            Transform3f transform = GLTFHelpers::GetLocalTransform(data->nodes[i]);
            result.SetLocalTransform(i, transform);

            int parent = GLTFHelpers::GetNodeIndex(node->parent, data->nodes, boneCount);
            result.SetParent(i, parent);
        }
        return result;
    }

    Pose LoadBindPose(cgltf_data *data)
    {
        Pose restPose = LoadRestPose(data);
        std::vector<Transform3f> worldBindPose;

        uint32_t numBones = restPose.BoneSize();

        for (uint32_t i = 0; i < numBones; ++i)
            worldBindPose.emplace_back(restPose.GetGlobalTransform(i));

        uint32_t numSkins = data->skins_count;
        for (uint32_t i = 0; i < numSkins; ++i)
        {
            cgltf_skin *skin = &(data->skins[i]);
            std::vector<float> inverseBindAccessor = GLTFHelpers::GetScalarValues(16, *skin->inverse_bind_matrices);

            for (int j = 0; j < skin->joints_count; ++j)
            {
                float *matrix = &(inverseBindAccessor[j * 16]);
                Matrix4f invBindMatrix = Matrix4f(matrix[0], matrix[1], matrix[2], matrix[3],
                                                  matrix[4], matrix[5], matrix[6], matrix[7],
                                                  matrix[8], matrix[9], matrix[10], matrix[11],
                                                  matrix[12], matrix[13], matrix[14], matrix[15]);
                Matrix4f bindMatrix = Matrix4f::Inverse(invBindMatrix);
                Transform3f bindTransform = Matrix4f::ToTransform(bindMatrix);

                cgltf_node *boneNode = skin->joints[j];
                int boneIndex = GLTFHelpers::GetNodeIndex(boneNode, data->nodes, numBones);
                worldBindPose[boneIndex] = bindTransform;
            }
        }

        Pose bindPose = restPose;
        for (uint32_t i = 0; i < numBones; ++i)
        {
            Transform3f current = worldBindPose[i];
            int p = bindPose.GetParent(i);
            if (p >= 0)
            {
                Transform3f parent = worldBindPose[p];
                current = Transform3f::Combine(Transform3f::Inverse(parent), current);
            }
            bindPose.SetLocalTransform(i, current);
        }
        return bindPose;
    }

    Skeleton LoadSkeleton(cgltf_data *data)
    {
        return Skeleton(LoadRestPose(data), LoadBindPose(data), LoadBoneNames(data));
    }

    std::vector<String> LoadBoneNames(cgltf_data *data)
    {
        uint32_t boneCount = (uint32_t)data->nodes_count;
        std::vector<String> result(boneCount, "Not Set");
        for (uint32_t i = 0; i < boneCount; ++i)
        {
            cgltf_node *node = &(data->nodes[i]);
            if (node->name == 0)
                result[i] = "EMPTY NODE";
            else
                result[i] = node->name;
        }
        return result;
    }

    std::vector<AnimationTransformClip> LoadAnimationsClips(cgltf_data *data)
    {
        uint32_t numClips = data->animations_count;
        uint32_t numNodes = data->nodes_count;

        std::vector<AnimationTransformClip> result;
        result.resize(numClips);

        for (uint32_t i = 0; i < numClips; ++i)
        {
            result[i].SetName(data->animations[i].name);
            uint32_t numChannels = data->animations[i].channels_count;

            for (uint32_t j = 0; j < numChannels; ++j)
            {
                cgltf_animation_channel channel = data->animations[i].channels[j];
                cgltf_node *target = channel.target_node;

                int nodeId = GLTFHelpers::GetNodeIndex(target, data->nodes, numNodes);

                if (channel.target_path == cgltf_animation_path_type_translation)
                {
                    VectorTrack &track = result[i][nodeId].GetPositionTrack();
                    GLTFHelpers::TrackFromChannel<Vector3f, 3>(track, channel);
                }
                else if (channel.target_path == cgltf_animation_path_type_scale)
                {
                    VectorTrack &track = result[i][nodeId].GetScaleTrack();
                    GLTFHelpers::TrackFromChannel<Vector3f, 3>(track, channel);
                }
                else if (channel.target_path == cgltf_animation_path_type_rotation)
                {
                    QuaternionTrack &track = result[i][nodeId].GetRotationTrack();
                    GLTFHelpers::TrackFromChannel<Quaternionf, 4>(track, channel);
                }
            }
            result[i].RecalculateDuration();
        }
        return result;
    }

    std::vector<SkeletalMesh*> LoadSkeletalMeshes(cgltf_data *data)
    {
        std::vector<SkeletalMesh*> result;
        cgltf_node *nodes = data->nodes;
        uint32_t nodeCount = data->nodes_count;
        for (uint32_t i = 0; i < nodeCount; ++i)
        {
            cgltf_node *node = &nodes[i];
            if (node->mesh == 0 || node->skin == 0)
                continue;
            int numPrims = node->mesh->primitives_count;
            for (int j = 0; j < numPrims; ++j)
            {
                cgltf_primitive *primitive = &node->mesh->primitives[j];
                uint32_t ac = primitive->attributes_count;

                std::vector<SkeletalVertex> skeletalVertices;
                std::vector<uint32_t> indices;

                for (uint32_t k = 0; k < ac; ++k)
                {
                    cgltf_attribute *attribute = &primitive->attributes[k];
                    cgltf_accessor accessor = *attribute->data;
                    uint32_t componentCount = 0;

                    if (accessor.type == cgltf_type_vec2)
                        componentCount = 2;
                    else if (accessor.type == cgltf_type_vec3)
                        componentCount = 3;
                    else if (accessor.type == cgltf_type_vec4)
                        componentCount = 4;

                    std::vector<float> values = GLTFHelpers::GetScalarValues(componentCount, accessor);
                    uint32_t accessorCount = accessor.count;

                    for (uint32_t i = 0; i < accessorCount; ++i)
                    {
                        SkeletalVertex vertex;

                        int index = i * componentCount;
                        switch (attribute->type)
                        {
                        case cgltf_attribute_type_position:
                            vertex.position = (Vector3f(values[index + 0], values[index + 1], values[index + 2]));
                            break;
                        case cgltf_attribute_type_texcoord:
                            vertex.texcoord0 = (Vector2f(values[index + 0], values[index + 1]));
                            break;
                        case cgltf_attribute_type_normal:
                        {
                            Vector3f n = Vector3f(values[index + 0], values[index + 1], values[index + 2]);
                            if (n.SquareLength() < 0.00001f)
                                n = Vector3f(0.0f, 1.0, 0.0f);
                            vertex.normal = n;
                            break;
                        }
                        case cgltf_attribute_type_weights:
                            vertex.boneWeights = Vector4f(values[index + 0], values[index + 1], values[index + 2], values[index + 3]);
                            break;
                        case cgltf_attribute_type_joints:
                        {
                            Vector4i32 joints(values[index + 0] + 0.5f, values[index + 1] + 0.5f, values[index + 2] + 0.5f, values[index + 3] + 0.5f);
                            joints.x = GLTFHelpers::GetNodeIndex(node->skin->joints[joints.x], nodes, nodeCount);
                            joints.y = GLTFHelpers::GetNodeIndex(node->skin->joints[joints.y], nodes, nodeCount);
                            joints.z = GLTFHelpers::GetNodeIndex(node->skin->joints[joints.z], nodes, nodeCount);
                            joints.w = GLTFHelpers::GetNodeIndex(node->skin->joints[joints.w], nodes, nodeCount);

                            joints.x = std::max(0, joints.x);
                            joints.y = std::max(0, joints.y);
                            joints.z = std::max(0, joints.z);
                            joints.w = std::max(0, joints.w);

                            vertex.boneIndices = joints;
                            break;
                        }
                        }

                        skeletalVertices.emplace_back(vertex);
                    }
                }

                if (primitive->indices != 0)
                {
                    for (uint32_t k = 0; k < primitive->indices->count; ++k)
                        indices.emplace_back((uint32_t)cgltf_accessor_read_index(primitive->indices, k));
                }

                SkeletalMesh* mesh = new SkeletalMesh();
                mesh->SetVertices(skeletalVertices);
                mesh->SetIndices(indices);

                result.emplace_back(mesh);
            }
        }
        return result;
    }
}