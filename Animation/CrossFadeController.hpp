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

        void Play(Clip *target);

        void FadeTo(Clip *target, float fadeTime);

        void Update(float deltaTime);

        const Pose &GetCurrentPose() const;

        const Clip *GetCurrentClip() const;

    protected:
        std::vector<CrossFadeTarget> mCrossFadeTargets;
        Clip *mClip;
        float mTime;
        Pose mPose;
        Skeleton mSkeleton;
        bool mWasSkeletonSet;
    };
}