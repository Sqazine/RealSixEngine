#include "CrossFadeController.hpp"
#include "Blending.hpp"

namespace RealSix
{
    CrossFadeController::CrossFadeController()
        : mClip(nullptr), mTime(0.0f), mWasSkeletonSet(false)
    {
    }
    CrossFadeController::CrossFadeController(const Skeleton &skeleton)
        : mClip(nullptr), mTime(0.0f)
    {
        SetSkeleton(skeleton);
    }

    void CrossFadeController::SetSkeleton(const Skeleton &skeleton)
    {
        mSkeleton = skeleton;
        mPose = mSkeleton.GetRestPose();
        mWasSkeletonSet = true;
    }

    void CrossFadeController::Play(AnimationTransformClip *target)
    {
        mCrossFadeTargets.clear();
        mClip = target;
        mPose = mSkeleton.GetRestPose();
        mTime = target->GetStartTime();
    }

    void CrossFadeController::FadeTo(AnimationTransformClip *target, float fadeTime)
    {
        if (mClip == nullptr)
        {
            Play(target);
            return;
        }

        if (mCrossFadeTargets.size() >= 1)
        {
            AnimationTransformClip *clip = mCrossFadeTargets.back().mClip;
            if (clip == target)
                return;
        }
        else
        {
            if (mClip == target)
                return;
        }

        mCrossFadeTargets.emplace_back(CrossFadeTarget(target, mSkeleton.GetRestPose(), fadeTime));
    }

    void CrossFadeController::Update(float deltaTime)
    {
        if (mClip == nullptr || !mWasSkeletonSet)
            return;

        for (uint32_t i = 0; i < mCrossFadeTargets.size(); ++i)
        {
            float duration = mCrossFadeTargets[i].mDuration;
            if (mCrossFadeTargets[i].mElapsed >= duration)
            {
                mClip = mCrossFadeTargets[i].mClip;
                mPose = mCrossFadeTargets[i].mPose;
                mTime = mCrossFadeTargets[i].mTime;
                mCrossFadeTargets.erase(mCrossFadeTargets.begin() + i);
                break;
            }
        }

        mPose = mSkeleton.GetRestPose();
        mTime = mClip->Sample(mPose, mTime + deltaTime);

        for (uint32_t i = 0; i < mCrossFadeTargets.size(); ++i)
        {
            CrossFadeTarget &target = mCrossFadeTargets[i];
            target.mTime = target.mClip->Sample(target.mPose, target.mTime + deltaTime);
            target.mElapsed += deltaTime;
            float t = target.mElapsed / target.mDuration;
            if (t > 1.0f)
                t = 1.0f;
            mPose = Blend(mPose, target.mPose, t, -1);
        }
    }

    const Pose &CrossFadeController::GetCurrentPose() const
    {
        return mPose;
    }

    const AnimationTransformClip *CrossFadeController::GetCurrentClip() const
    {
        return mClip;
    }
}