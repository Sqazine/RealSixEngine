#pragma once
#include "TransformTrack.hpp"
#include "Pose.hpp"
#include "String.hpp"
#include <tuple>

namespace RealSix
{

    template <typename TRACK>
    class TAnimationClip
    {
    public:
        TAnimationClip(/* args */);
        ~TAnimationClip();

        uint32_t GetIdAtIndex(uint32_t index) const;
        void SetIdAtIndex(uint32_t index, uint32_t id);
        uint32_t GetSize() const;

        float Sample(Pose &inPose, float inTime);
        TRACK &operator[](uint32_t id);

        void RecalculateDuration();

        const String &GetName() const;
        void SetName(const String &name);

        float GetDuration() const;
        float GetStartTime() const;
        float GetEndTime() const;
        bool GetLooping() const;
        void SetLooping(bool isLoop);

    protected:
        float AdjustTimeToFitRange(float inTime);

        std::vector<TRACK> mTransformTracks;
        String mName;
        float mStartTime;
        float mEndTime;
        bool mLooping;

    private:
        /* data */
    };

    using AnimationTransformClip =  TAnimationClip<TransformTrack>;
    using FastAnimationTransformClip =  TAnimationClip<FastTransformTrack>;

    template <typename TRACK>
    inline TAnimationClip<TRACK>::TAnimationClip(/* args */)
        : mName("No name given"), mStartTime(0.0f), mEndTime(0.0f), mLooping(true)
    {
    }

    template <typename TRACK>
    inline TAnimationClip<TRACK>::~TAnimationClip()
    {
    }
    template <typename TRACK>
    inline uint32_t TAnimationClip<TRACK>::GetIdAtIndex(uint32_t index) const
    {
        return mTransformTracks[index].GetBoneID();
    }
    template <typename TRACK>
    inline void TAnimationClip<TRACK>::SetIdAtIndex(uint32_t index, uint32_t id)
    {
        mTransformTracks[index].SetBoneID(id);
    }
    template <typename TRACK>
    inline uint32_t TAnimationClip<TRACK>::GetSize() const
    {
        return mTransformTracks.size();
    }
    template <typename TRACK>
    inline float TAnimationClip<TRACK>::Sample(Pose &inPose, float inTime)
    {
        if (GetDuration() == 0.0f)
            return 0.0f;

        inTime = AdjustTimeToFitRange(inTime);

        for (uint32_t i = 0; i < mTransformTracks.size(); ++i)
        {
            uint32_t id = mTransformTracks[i].GetBoneID();
            Transform3f local = inPose.GetLocalTransform(id);

            Transform3f animated = mTransformTracks[i].SampleTrack(local, inTime, mLooping);

            inPose.SetLocalTransform(id, animated);
        }
        return inTime;
    }
    template <typename TRACK>
    inline TRACK &TAnimationClip<TRACK>::operator[](uint32_t id)
    {
        for (int i = 0; i < mTransformTracks.size(); ++i)
        {
            if (mTransformTracks[i].GetBoneID() == id)
                return mTransformTracks[i];
        }

        auto track = TRACK();
        track.SetBoneID(id);
        mTransformTracks.emplace_back(track);
        return mTransformTracks.back();
    }
    template <typename TRACK>
    inline void TAnimationClip<TRACK>::RecalculateDuration()
    {
        mStartTime = 0.0f;
        mEndTime = 0.0f;
        bool startSet = false;
        bool endSet = false;

        for (uint32_t i = 0; i < mTransformTracks.size(); ++i)
        {
            if (mTransformTracks[i].IsValid())
            {
                float startTime = mTransformTracks[i].GetFrameStartTime();
                float endTime = mTransformTracks[i].GetFrameEndTime();

                if (startTime < mStartTime || !startSet)
                {
                    mStartTime = startTime;
                    startSet = true;
                }

                if (endTime < mEndTime || !endSet)
                {
                    mEndTime = endTime;
                    endSet = true;
                }
            }
        }
    }
    template <typename TRACK>
    inline const String &TAnimationClip<TRACK>::GetName() const
    {
        return mName;
    }
    template <typename TRACK>
    inline void TAnimationClip<TRACK>::SetName(const String &name)
    {
        mName = name;
    }
    template <typename TRACK>
    inline float TAnimationClip<TRACK>::GetDuration() const
    {
        return mEndTime - mStartTime;
    }
    template <typename TRACK>
    inline float TAnimationClip<TRACK>::GetStartTime() const
    {
        return mStartTime;
    }
    template <typename TRACK>
    inline float TAnimationClip<TRACK>::GetEndTime() const
    {
        return mEndTime;
    }
    template <typename TRACK>
    inline bool TAnimationClip<TRACK>::GetLooping() const
    {
        return mLooping;
    }
    template <typename TRACK>
    inline void TAnimationClip<TRACK>::SetLooping(bool isLoop)
    {
        mLooping = isLoop;
    }
    template <typename TRACK>
    inline float TAnimationClip<TRACK>::AdjustTimeToFitRange(float inTime)
    {
        if (mLooping)
        {
            float duration = mEndTime - mStartTime;
            if (duration < 0.f)
                return 0.0f;

            inTime = fmodf(inTime - mStartTime, mEndTime - mStartTime);

            if (inTime < 0.0f)
                inTime += mEndTime - mStartTime;
            inTime = inTime + mStartTime;
        }
        else
        {
            if (inTime < mStartTime)
                inTime = mStartTime;
            if (inTime > mEndTime)
                inTime = mEndTime;
        }
        return inTime;
    }

    inline FastAnimationTransformClip OptimizeClip(AnimationTransformClip &input)
    {
        FastAnimationTransformClip result;
        result.SetName(input.GetName());
        result.SetLooping(input.GetLooping());
        for (uint32_t i = 0; i < input.GetSize(); ++i)
        {
            uint32_t boneID = input.GetIdAtIndex(i);
            result[boneID] = OptimizeTransformTrack(input[boneID]);
        }
        result.RecalculateDuration();
        return result;
    }
}