#pragma once
#include "TransformTrack.hpp"
#include "Pose.hpp"
#include "String.hpp"
#include <tuple>

namespace RealSix
{

    template <typename TRACK>
    class TClip
    {
    public:
        TClip(/* args */);
        ~TClip();

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

    typedef TClip<TransformTrack> Clip;
    typedef TClip<FastTransformTrack> FastClip;

    template <typename TRACK>
    inline TClip<TRACK>::TClip(/* args */)
        : mName("No name given"), mStartTime(0.0f), mEndTime(0.0f), mLooping(true)
    {
    }

    template <typename TRACK>
    inline TClip<TRACK>::~TClip()
    {
    }
    template <typename TRACK>
    inline uint32_t TClip<TRACK>::GetIdAtIndex(uint32_t index) const
    {
        return mTransformTracks[index].GetBoneID();
    }
    template <typename TRACK>
    inline void TClip<TRACK>::SetIdAtIndex(uint32_t index, uint32_t id)
    {
        mTransformTracks[index].SetBoneID(id);
    }
    template <typename TRACK>
    inline uint32_t TClip<TRACK>::GetSize() const
    {
        return mTransformTracks.size();
    }
    template <typename TRACK>
    inline float TClip<TRACK>::Sample(Pose &inPose, float inTime)
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
    inline TRACK &TClip<TRACK>::operator[](uint32_t id)
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
    inline void TClip<TRACK>::RecalculateDuration()
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
    inline const String &TClip<TRACK>::GetName() const
    {
        return mName;
    }
    template <typename TRACK>
    inline void TClip<TRACK>::SetName(const String &name)
    {
        mName = name;
    }
    template <typename TRACK>
    inline float TClip<TRACK>::GetDuration() const
    {
        return mEndTime - mStartTime;
    }
    template <typename TRACK>
    inline float TClip<TRACK>::GetStartTime() const
    {
        return mStartTime;
    }
    template <typename TRACK>
    inline float TClip<TRACK>::GetEndTime() const
    {
        return mEndTime;
    }
    template <typename TRACK>
    inline bool TClip<TRACK>::GetLooping() const
    {
        return mLooping;
    }
    template <typename TRACK>
    inline void TClip<TRACK>::SetLooping(bool isLoop)
    {
        mLooping = isLoop;
    }
    template <typename TRACK>
    inline float TClip<TRACK>::AdjustTimeToFitRange(float inTime)
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

    inline FastClip OptimizeClip(Clip &input)
    {
        FastClip result;
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