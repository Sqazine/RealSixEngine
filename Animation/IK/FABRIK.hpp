#pragma once

#include <memory>
#include <vector>
#include "Math/Transform.hpp"

namespace RealSix
{
    class FABRIK
    {
    public:
        FABRIK();
        ~FABRIK();

        uint32_t IKChainSize();

        void Resize(uint32_t newSize);

        void SetNumSteps(uint32_t numSteps);
        uint32_t GetNumSteps();

        void SetThreshold(float value);
        float GetThreshold();

        void SetLocalTransform(uint32_t index, const Transform3f &t);
        Transform3f GetLocalTransform(uint32_t index);

        Transform3f GetGlobalTransform(uint32_t index);

        bool Execute(const Transform3f &target);

    protected:
        void IKChainToWorld();
        void WorldToIKChain();
        void IterateForward(const Vector3f &base);
        void IterateBackword(const Vector3f &goal);

        std::vector<Transform3f> mIKChain;
        uint32_t mNumSteps;
        float mThreshold;
        std::vector<Vector3f> mWorldChain;
        std::vector<float> mLengths;
    };
}