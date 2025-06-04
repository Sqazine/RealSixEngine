#pragma once
#include <vector>
#include "Math/Transform.hpp"

namespace RealSix
{
    class CCDIK
    {
    public:
        CCDIK();
        ~CCDIK();

        uint32_t IKChainSize() const;
        void Resize(uint32_t newSize);

        Transform3f &operator[](uint32_t index);

        Transform3f GetGlobalTransform(uint32_t index);

        void SetNumSteps(uint32_t numSteps);
        uint32_t GetNumSteps();

        void SetThreshold(float value);
        float GetThreshold();

        void AddIKChain(const Transform3f &ikChain);

        bool Execute(const Transform3f &target);

    protected:
        //表示IK链节点的transform
        std::vector<Transform3f> mIKChains;
        //迭代次数
        uint32_t mNumSteps;
        //末端受动器与目标的距离阈值
        float mThreshold;
    };
}