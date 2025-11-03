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
#include "Math/Transform.h"
#include "Animation/Pose.h"
#include "Animation/Track.h"
#include "Animation/Clip.h"
#include "Animation/Skeleton.h"
#include "Render/Mesh.h"

namespace RealSix::GLTFLoader
{
    cgltf_data *LoadGLTFFile(const std::string &path);
    void FreeGLTFFile(cgltf_data *data);

    Pose LoadRestPose(cgltf_data *data);

    Pose LoadBindPose(cgltf_data *data);
    Skeleton LoadSkeleton(cgltf_data *data);

    std::vector<std::string> LoadBoneNames(cgltf_data *data);

    std::vector<Clip> LoadAnimationsClips(cgltf_data *data);

    std::vector<SkeletalMesh*> LoadSkeletalMeshes(cgltf_data *data);
} // namespace GLTFLoader
