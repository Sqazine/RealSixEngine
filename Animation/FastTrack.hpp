#pragma once
#include "Track.hpp"
#include "Math/Vector3.hpp"
#include "Math/Quaternion.hpp"

namespace RealSix
{

    template <typename T, int32_t N>
    class FastTrack : public Track<T, N>
    {
    public:
        void UpdateIndexLookupTable();

    protected:
        virtual int32_t FrameIndex(float time, bool looping);
        std::vector<uint32_t> mSampledFrames;
    };

    typedef FastTrack<float, 1> FastScalarTrack;
    typedef FastTrack<Vector3f, 3> FastVectorTrack;
    typedef FastTrack<Quaternionf, 4> FastQuaternionTrack;

    template <typename T, int32_t N>
    inline void FastTrack<T, N>::UpdateIndexLookupTable()
    {
        int numFrames = this->mFrames.size();
        if (numFrames <= 1)
            return;
        //整条轨的开始时间与结束时间
        float duration = this->GetFrameEndTime() - this->GetFrameStartTime();
        uint32_t numSamples = duration * 60;
        mSampledFrames.resize(numSamples);

        for (uint32_t i = 0; i < numSamples; ++i)
        {
            float t = (float)i / (float)(numSamples - 1);
            float time = t * duration + this->GetFrameStartTime();

            uint32_t frameIndex = 0;
            for (int j = numFrames - 1; j >= 0; --j)
            {
                if (time >= this->mFrames[j].mTime)
                {
                    frameIndex = (uint32_t)j;
                    if ((int32_t)frameIndex >= numFrames - 2)
                        frameIndex = numFrames - 2;
                    break;
                }
            }
            mSampledFrames[i] = frameIndex;
        }
    }

    template <typename T, int32_t N>
    inline int32_t FastTrack<T, N>::FrameIndex(float time, bool looping)
    {
        uint32_t size = this->mFrames.size();
        if (size <= 1)
            return -1;
        if (looping)
        {
            float startTime = this->GetFrameStartTime();
            float endTime = this->GetFrameEndTime();

            float duration = endTime - startTime;
            time = fmodf(time - startTime, endTime - startTime);
            if (time < 0.0f)
                time += endTime - startTime;
            time = time + startTime;
        }
        else
        {
            if (time <= this->mFrames[0].mTime)
                return 0;
            if (time >= this->mFrames[size - 2].mTime)
                return (int32_t)size - 2;
        }
        //从帧索引查找表中寻找帧索引,避免循环
        float duration = this->GetFrameEndTime() - this->GetFrameStartTime();
        float t = time / duration;
        uint32_t numSamples = (duration * 60.0f);
        uint32_t index = (t * (float)numSamples);
        if (index >= mSampledFrames.size())
            return -1;
        return (int)mSampledFrames[index];
    }

    template <typename T, int32_t N>
    inline FastTrack<T, N> OptimizeTrack(Track<T, N> &input)
    {
        FastTrack<T, N> result;
        result.SetFrameInterpolation(input.GetFrameInterpolation());
        uint32_t size = input.FrameSize();
        result.ResizeFrames(size);

        for (uint32_t i = 0; i < size; ++i)
            result[i] = input[i];

        result.UpdateIndexLookupTable();
        return result;
    }
}