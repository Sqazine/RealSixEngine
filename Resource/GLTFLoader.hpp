#pragma once
#pragma warning(disable : 26451)

#define _CRT_SECURE_NO_WARNINGS

#ifndef CGLTF_IMPLEMENTATION
#define CGLTF_IMPLEMENTATION
#endif

#ifndef CGLTF_WRITE_IMPLEMENTATION
#define CGLTF_WRITE_IMPLEMENTATION
#endif

#include <cgltf.h>
#include "Math/Transform.hpp"
#include "Animation/Pose.hpp"
#include "Animation/Track.hpp"
#include "Animation/AnimationClip.hpp"
#include "Animation/Skeleton.hpp"
#include "Render/Mesh.hpp"

namespace RealSix::GLTFLoader
{
    cgltf_data *LoadGLTFFile(const String &path);
    void FreeGLTFFile(cgltf_data *data);

    Pose LoadRestPose(cgltf_data *data);

    Pose LoadBindPose(cgltf_data *data);
    Skeleton LoadSkeleton(cgltf_data *data);

    std::vector<String> LoadBoneNames(cgltf_data *data);

    std::vector<AnimationTransformClip> LoadAnimationsClips(cgltf_data *data);

    std::vector<SkeletalMesh*> LoadSkeletalMeshes(cgltf_data *data);
} // namespace GLTFLoader
