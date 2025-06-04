#pragma once
#include "Frame.hpp"
#include "Interpolation.hpp"
#include <cstdint>
#include <cmath>
#include <vector>

namespace RealSix
{

    template <typename T, int32_t N>
    class Track
    {
    public:
        Track();

        void ResizeFrames(uint32_t size);
        uint32_t FrameSize();

        const Interpolation &GetFrameInterpolation() const;
        void SetFrameInterpolation(const Interpolation &interpolation);
        float GetFrameStartTime() const;
        float GetFrameEndTime() const;

        T SampleFrame(float time, bool loop);
        Frame<N> &operator[](uint32_t index);

    protected:
        T SampleConstant(float time, bool loop);
        T SampleLinear(float time, bool loop);
        T SampleCubic(float time, bool loop);

        T Hermite(float time, const T &p1, const T &s1, const T &p2, const T &s2);

        //获取时间之前的最后一帧,该时间必须在轨道的时间范围之内
        virtual int32_t FrameIndex(float time, bool looping);
        //接受轨道时间范围之外的时间，并调整为轨道时间范围之内的时间
        float AdjustTimeToFitTrack(float time, bool loop);

        T CastValue(const float *value);

        std::vector<Frame<N>> mFrames;
        Interpolation mInterpolation;
    };

    typedef Track<float, 1> ScalarTrack;
    typedef Track<Vector3f, 3> VectorTrack;
    typedef Track<Quaternionf, 4> QuaternionTrack;

    template <typename T, int32_t N>
    Track<T, N>::Track()
        : mInterpolation(Interpolation::Linear)
    {
    }

    template <typename T, int32_t N>
    inline float Track<T, N>::GetFrameStartTime() const
    {
        return mFrames[0].mTime;
    }

    template <typename T, int32_t N>
    inline float Track<T, N>::GetFrameEndTime() const
    {
        return mFrames.back().mTime;
    }

    template <typename T, int32_t N>
    inline const Interpolation &Track<T, N>::GetFrameInterpolation() const
    {
        return mInterpolation;
    }

    template <typename T, int32_t N>
    inline void Track<T, N>::SetFrameInterpolation(const Interpolation &interpolation)
    {
        mInterpolation = interpolation;
    }

    template <typename T, int32_t N>
    inline T Track<T, N>::SampleFrame(float time, bool loop)
    {
        if (mInterpolation == Interpolation::Constant)
            return SampleConstant(time, loop);
        else if (mInterpolation == Interpolation::Linear)
            return SampleLinear(time, loop);
        return SampleCubic(time, loop);
    }

    template <typename T, int32_t N>
    inline T Track<T, N>::SampleConstant(float time, bool loop)
    {
        int32_t frameIndex = FrameIndex(time, loop);
        if (frameIndex < 0 || frameIndex >= mFrames.size())
            return T();
        return CastValue(&(mFrames[frameIndex].mValue[0]));
    }
    template <typename T, int32_t N>
    inline T Track<T, N>::SampleLinear(float time, bool loop)
    {
        int32_t thisframeIndex = FrameIndex(time, loop);
        if (thisframeIndex < 0 || thisframeIndex >= mFrames.size())
            return T();
        int32_t nextFrameIndex = thisframeIndex + 1;

        float trackTime = AdjustTimeToFitTrack(time, loop);
        float thisTime = mFrames[thisframeIndex].mTime;
        float frameDelta = mFrames[nextFrameIndex].mTime - thisTime;
        if (frameDelta <= 0.0f)
            return T();

        float t = (trackTime - thisTime) / frameDelta;
        T start = CastValue(&mFrames[thisframeIndex].mValue[0]);
        T end = CastValue(&mFrames[nextFrameIndex].mValue[0]);
        return Interpolate(start, end, t);
    }
    template <typename T, int32_t N>
    inline T Track<T, N>::SampleCubic(float time, bool looping)
    {
        int32_t thisFrameIndex = FrameIndex(time, looping);
        if (thisFrameIndex < 0 || thisFrameIndex >= (mFrames.size() - 1))
            return T();

        int32_t nextFrameIndex = thisFrameIndex + 1;
        float trackTime = AdjustTimeToFitTrack(time, looping);
        float thisTime = mFrames[thisFrameIndex].mTime;
        float frameDelta = mFrames[nextFrameIndex].mTime - thisTime;

        if (frameDelta <= 0.0f)
            return T();

        float t = (trackTime - thisTime) / frameDelta;
        size_t fltSize = sizeof(float);
        T point1 = CastValue(&mFrames[thisFrameIndex].mValue[0]);
        T slope1;
        //使用memcpy而不是CastValue是避免四元数情况下的归一化
        memcpy(&slope1, mFrames[thisFrameIndex].mOutTangentSlope, N * fltSize);
        slope1 = slope1 * frameDelta;

        T point2 = CastValue(&mFrames[nextFrameIndex].mValue[0]);
        T slope2;
        memcpy(&slope2, mFrames[nextFrameIndex].mInTangentSlope, N * fltSize);
        slope2 = slope2 * frameDelta;

        return Hermite(t, point1, slope1, point2, slope2);
    }

    template <typename T, int32_t N>
    inline Frame<N> &Track<T, N>::operator[](uint32_t index)
    {
        return mFrames[index];
    }

    template <typename T, int32_t N>
    inline void Track<T, N>::ResizeFrames(uint32_t size)
    {
        mFrames.resize(size);
    }
    template <typename T, int32_t N>
    inline uint32_t Track<T, N>::FrameSize()
    {
        return mFrames.size();
    }
    template <typename T, int32_t N>
    inline T Track<T, N>::Hermite(float time, const T &p1, const T &s1, const T &p2, const T &s2)
    {
        float timeSquare = Math::Pow(time,2);
        float timeCubic = Math::Pow(time,3);
        T tmp_p2 = p2;
        Neighborhood(p1, tmp_p2);
        float h1 = 2.0f * timeCubic - 3.0f * timeSquare + 1.0f;
        float h2 = -2.0f * timeCubic + 3.0f * timeSquare;
        float h3 = timeCubic - 2.0f * timeSquare + time;
        float h4 = timeCubic - timeSquare;
        T result = p1 * h1 + tmp_p2 * h2 + s1 * h3 + s2 * h4;
        return AdjustHermiteResult(result);
    }
    template <typename T, int32_t N>
    inline int32_t Track<T, N>::FrameIndex(float time, bool looping)
    {
        uint32_t size = mFrames.size();
        if (size < 1)
            return -1;
        if (looping)
        {
            float startTime = mFrames[0].mTime;
            float endTime = mFrames[size - 1].mTime;
            float duration = endTime - startTime;
            time = fmodf(time - startTime, endTime - startTime);
            if (time < 0.0f)
                time += endTime - startTime;
            time += startTime;
        }
        else
        {
            if (time <= mFrames[0].mTime)
                return 0;
            if (time >= mFrames[size - 2].mTime)
                return (int)size - 2;
        }

        for (int i = (int)size - 1; i >= 0; --i)
        {
            if (time >= mFrames[i].mTime)
                return i;
        }
        return -1;
    }

    template <typename T, int32_t N>
    inline float Track<T, N>::AdjustTimeToFitTrack(float time, bool loop)
    {
        uint32_t size = mFrames.size();
        if (size <= 1)
            return 0.0f;
        float startTime = mFrames[0].mTime;
        float endTime = mFrames[size - 1].mTime;
        float duration = endTime - startTime;
        if (duration < 0.0f)
            return 0.0f;
        if (loop)
        {
            time = fmodf(time - startTime, endTime - startTime);
            if (time < 0.0f)
                time += endTime - startTime;
            time += startTime;
        }
        else
        {
            if (time <= mFrames[0].mTime)
                time = startTime;
            if (time >= mFrames[size - 1].mTime)
                time = endTime;
        }
        return time;
    }

    template <>
    inline float Track<float, 1>::CastValue(const float *value)
    {
        return value[0];
    }

    template <>
    inline Vector3f Track<Vector3f, 3>::CastValue(const float *value)
    {
        return Vector3f(value[0], value[1], value[2]);
    }

    template <>
    inline Quaternionf Track<Quaternionf, 4>::CastValue(const float *value)
    {
        return Quaternionf::Normalize(Quaternionf(value[0], value[1], value[2], value[3]));
    }
}