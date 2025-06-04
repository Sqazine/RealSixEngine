#pragma once
#include <memory>
#include "Pose.hpp"
#include "Clip.hpp"

namespace RealSix
{
    struct CrossFadeTarget
    {

        CrossFadeTarget()
            : mClip(nullptr), mTime(0.0f), mDuration(0.0f), mElapsed(0.0f)
        {
        }

        CrossFadeTarget(Clip *target, const Pose &pose, float duration)
            : mClip(target), mTime(target->GetStartTime()), mPose(pose), mDuration(duration), mElapsed(0.0f)
        {
        }

        Pose mPose;
        Clip *mClip;
        float mTime;
        float mDuration;
        float mElapsed;
    };
}