#pragma once
#include <cstdint>
#include "Pose.h"
#include "Clip.h"
#include "Skeleton.h"

namespace RealSix
{
    bool IsInHierarchy(const Pose &pose, uint32_t parent, uint32_t search);
    //混合两个姿势的局部SRT
    Pose Blend(const Pose &a, const Pose &b, float t, int root);

    Pose MakeAdditivePose(const Skeleton &skeleton, Clip &clip);
    Pose AddAdditivePose(const Pose &inPose, const Pose &addPose, const Pose &additiveBasePose, int blendRoot);
}