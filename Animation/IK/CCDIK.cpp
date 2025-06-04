#include "CCDIK.hpp"
#include "Math/Vector3.hpp"

namespace RealSix
{

    CCDIK::CCDIK()
        : mNumSteps(15), mThreshold(0.00001f)
    {
    }
    CCDIK::~CCDIK() {}

    uint32_t CCDIK::IKChainSize() const
    {
        return mIKChains.size();
    }
    void CCDIK::Resize(uint32_t newSize)
    {
        mIKChains.resize(newSize);
    }

    Transform3f &CCDIK::operator[](uint32_t index)
    {
        return mIKChains[index];
    }

    Transform3f CCDIK::GetGlobalTransform(uint32_t index)
    {
        Transform3f world = mIKChains[index];

        for (int i = (int)index - 1; i >= 0; --i)
            world = Transform3f::Combine(mIKChains[i], world);

        return world;
    }

    uint32_t CCDIK::GetNumSteps()
    {
        return mNumSteps;
    }

    void CCDIK::SetNumSteps(uint32_t numSteps)
    {
        mNumSteps = numSteps;
    }

    float CCDIK::GetThreshold()
    {
        return mThreshold;
    }

    void CCDIK::SetThreshold(float value)
    {
        mThreshold = value;
    }

    void CCDIK::AddIKChain(const Transform3f &ikChain)
    {
        mIKChains.emplace_back(ikChain);
    }

    bool CCDIK::Execute(const Transform3f &target)
    {
        uint32_t size = IKChainSize();
        if (size == 0)
            return false;

        uint32_t last = size - 1;

        float thresholdSq = mThreshold * mThreshold;

        Vector3f goal = target.position;

        for (uint32_t i = 0; i < mNumSteps; ++i)
        {
            Vector3f effector = GetGlobalTransform(last).position;
            if ((goal - effector).SquareLength() < thresholdSq)
                return true;

            for (int j = (int)size - 2; j >= 0; --j)
            {
                effector = GetGlobalTransform(last).position;

                Transform3f world = GetGlobalTransform(j);

                Vector3f position = world.position;
                Quaternionf rotation = world.rotation;

                Vector3f boneToEffector = effector - position;
                Vector3f boneToGoal = goal - position;

                Quaternionf effectorToGoal;
                if (boneToGoal.SquareLength() > 0.00001f)
                    effectorToGoal = Quaternionf::FromTo(boneToEffector, boneToGoal);

                Quaternionf worldRotated = rotation * effectorToGoal;

                Quaternionf localRotate = worldRotated * Quaternionf::Inverse(rotation);

                mIKChains[j].rotation = localRotate * mIKChains[j].rotation;

                effector = GetGlobalTransform(last).position;

                if ((goal - effector).SquareLength() < thresholdSq)
                    return true;
            }
        }
        return false;
    }
}