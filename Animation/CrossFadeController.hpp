#pragma once
#include "CrossFadeTarget.hpp"
#include "Skeleton.hpp"
#include "Pose.hpp"
#include "Skeleton.hpp"

namespace RealSix
{
    class CrossFadeController
    {
    public:
        CrossFadeController();
        CrossFadeController(const Skeleton &skeleton);

        void SetSkeleton(const Skeleton &skeleton);

        void Play(AnimationTransformClip *target);

        void FadeTo(AnimationTransformClip *target, float fadeTime);

        void Update(float deltaTime);

        const Pose &GetCurrentPose() const;

        const AnimationTransformClip *GetCurrentClip() const;

    protected:
        std::vector<CrossFadeTarget> mCrossFadeTargets;
        AnimationTransformClip *mClip;
        float mTime;
        Pose mPose;
        Skeleton mSkeleton;
        bool mWasSkeletonSet;
    };
}