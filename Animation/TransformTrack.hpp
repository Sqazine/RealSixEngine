#pragma once
#include <cstdint>
#include "Track.hpp"
#include "FastTrack.hpp"
#include "Math/Transform.hpp"

namespace RealSix
{

    template <typename VTRACK, typename QTRACK>
    class TTransformTrack
    {
    public:
        TTransformTrack();

        void SetBoneID(uint32_t id);
        uint32_t GetBoneID() const;

        VTRACK &GetPositionTrack();
        VTRACK &GetScaleTrack();
        QTRACK &GetRotationTrack();

        float GetFrameStartTime();
        float GetFrameEndTime();

        bool IsValid();

        Transform3f SampleTrack(const Transform3f &ref, float time, bool looping);

    protected:
        uint32_t mBoneID;

        VTRACK mPositionTrack;
        VTRACK mScaleTrack;
        QTRACK mRotationTrack;
    };

    typedef TTransformTrack<VectorTrack, QuaternionTrack> TransformTrack;
    typedef TTransformTrack<FastVectorTrack, FastQuaternionTrack> FastTransformTrack;

    template <typename VTRACK, typename QTRACK>
    inline TTransformTrack<VTRACK, QTRACK>::TTransformTrack()
        : mBoneID(0)
    {
    }
    template <typename VTRACK, typename QTRACK>
    inline void TTransformTrack<VTRACK, QTRACK>::SetBoneID(uint32_t id)
    {
        mBoneID = id;
    }
    template <typename VTRACK, typename QTRACK>
    inline uint32_t TTransformTrack<VTRACK, QTRACK>::GetBoneID() const
    {
        return mBoneID;
    }
    template <typename VTRACK, typename QTRACK>
    inline VTRACK &TTransformTrack<VTRACK, QTRACK>::GetPositionTrack()
    {
        return mPositionTrack;
    }
    template <typename VTRACK, typename QTRACK>
    inline VTRACK &TTransformTrack<VTRACK, QTRACK>::GetScaleTrack()
    {
        return mScaleTrack;
    }
    template <typename VTRACK, typename QTRACK>
    inline QTRACK &TTransformTrack<VTRACK, QTRACK>::GetRotationTrack()
    {
        return mRotationTrack;
    }
    template <typename VTRACK, typename QTRACK>
    inline float TTransformTrack<VTRACK, QTRACK>::GetFrameStartTime()
    {
        float result = 0.0f;
        bool isSet = false;
        if (mPositionTrack.FrameSize() > 1)
        {
            result = mPositionTrack.GetFrameStartTime();
            isSet = true;
        }

        if (mPositionTrack.FrameSize() > 1)
        {
            float rotationStart = mRotationTrack.GetFrameStartTime();
            if (rotationStart < result || !isSet)
            {
                result = rotationStart;
                isSet = true;
            }
        }

        if (mScaleTrack.FrameSize() > 1)
        {
            float scaleStart = mScaleTrack.GetFrameStartTime();
            if (scaleStart < result || !isSet)
            {
                result = scaleStart;
                isSet = true;
            }
        }
        return result;
    }
    template <typename VTRACK, typename QTRACK>
    inline float TTransformTrack<VTRACK, QTRACK>::GetFrameEndTime()
    {
        float result = 0.0f;
        bool isSet = false;
        if (mPositionTrack.FrameSize() > 1)
        {
            result = mPositionTrack.GetFrameEndTime();
            isSet = true;
        }

        if (mRotationTrack.FrameSize() > 1)
        {
            float rotationStart = mRotationTrack.GetFrameEndTime();
            if (rotationStart > result || !isSet)
            {
                result = rotationStart;
                isSet = true;
            }
        }

        if (mScaleTrack.FrameSize() > 1)
        {
            float scaleStart = mScaleTrack.GetFrameEndTime();
            if (scaleStart > result || !isSet)
            {
                result = scaleStart;
                isSet = true;
            }
        }
        return result;
    }
    template <typename VTRACK, typename QTRACK>
    inline bool TTransformTrack<VTRACK, QTRACK>::IsValid()
    {
        return mPositionTrack.FrameSize() > 1 || mRotationTrack.FrameSize() > 1 || mScaleTrack.FrameSize() > 1;
    }

    template <typename VTRACK, typename QTRACK>
    inline Transform3f TTransformTrack<VTRACK, QTRACK>::SampleTrack(const Transform3f &ref, float time, bool looping)
    {
        Transform3f result = ref;
        if (mPositionTrack.FrameSize() > 1)
            result.position = mPositionTrack.SampleFrame(time, looping);
        if (mRotationTrack.FrameSize() > 1)
            result.rotation = mRotationTrack.SampleFrame(time, looping);
        if (mScaleTrack.FrameSize() > 1)
            result.scale = mScaleTrack.SampleFrame(time, looping);
        return result;
    }

    inline FastTransformTrack OptimizeTransformTrack(TransformTrack &input)
    {
        FastTransformTrack result;
        result.SetBoneID(input.GetBoneID());
        result.GetPositionTrack() = OptimizeTrack<Vector3f, 3>(input.GetPositionTrack());
        result.GetScaleTrack() = OptimizeTrack<Vector3f, 3>(input.GetScaleTrack());
        result.GetRotationTrack() = OptimizeTrack<Quaternionf, 4>(input.GetRotationTrack());
        return result;
    }
}