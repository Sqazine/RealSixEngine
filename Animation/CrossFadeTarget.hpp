#pragma once
#include <memory>
#include "Pose.hpp"
#include "AnimationClip.hpp"

namespace RealSix
{
    struct CrossFadeTarget
    {

        CrossFadeTarget()
            : mClip(nullptr), mTime(0.0f), mDuration(0.0f), mElapsed(0.0f)
        {
        }

        CrossFadeTarget(AnimationTransformClip *target, const Pose &pose, float duration)
            : mClip(target), mTime(target->GetStartTime()), mPose(pose), mDuration(duration), mElapsed(0.0f)
        {
        }

        Pose mPose;
        AnimationTransformClip *mClip;
        float mTime;
        float mDuration;
        float mElapsed;
    };
}