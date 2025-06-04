#include "FABRIK.hpp"
namespace RealSix
{
    FABRIK::FABRIK()
        : mNumSteps(15), mThreshold(0.00001f)
    {
    }

    FABRIK::~FABRIK()
    {
    }

    uint32_t FABRIK::IKChainSize()
    {
        return mIKChain.size();
    }

    void FABRIK::Resize(uint32_t newSize)
    {
        mIKChain.resize(newSize);
        mWorldChain.resize(newSize);
        mLengths.resize(newSize);
    }

    void FABRIK::SetNumSteps(uint32_t numSteps)
    {
        mNumSteps = numSteps;
    }

    uint32_t FABRIK::GetNumSteps()
    {
        return mNumSteps;
    }

    void FABRIK::SetThreshold(float value)
    {
        mThreshold = value;
    }
    float FABRIK::GetThreshold()
    {
        return mThreshold;
    }

    void FABRIK::SetLocalTransform(uint32_t index, const Transform3f &t)
    {
        mIKChain[index] = t;
    }
    Transform3f FABRIK::GetLocalTransform(uint32_t index)
    {
        return mIKChain[index];
    }

    Transform3f FABRIK::GetGlobalTransform(uint32_t index)
    {
        uint32_t size = mIKChain.size();

        Transform3f world = mIKChain[index];

        for (int i = index - 1; i >= 0; --i)
            world = Transform3f::Combine(mIKChain[i], world);
        return world;
    }

    bool FABRIK::Execute(const Transform3f &target)
    {
        uint32_t size = IKChainSize();
        if (size == 0)
            return false;
        uint32_t last = size - 1;

        float thresholdSq = mThreshold * mThreshold;

        IKChainToWorld();

        Vector3f goal = target.position;

        Vector3f base = mWorldChain[0];

        for (uint32_t i = 0; i < mNumSteps; ++i)
        {
            Vector3f effector = mWorldChain[last];
            if ((goal - effector).SquareLength() < thresholdSq)
            {
                WorldToIKChain();
                return true;
            }
            IterateBackword(goal);
            IterateForward(base);
        }
        WorldToIKChain();
        Vector3f effector = GetGlobalTransform(last).position;
        if ((goal - effector).SquareLength() < thresholdSq)
            return true;
        return false;
    }

    void FABRIK::IKChainToWorld()
    {
        uint32_t size = IKChainSize();
        for (uint32_t i = 0; i < size; ++i)
        {
            Transform3f world = GetGlobalTransform(i);
            mWorldChain[i] = world.position;
            if (i >= 1)
            {
                Vector3f prev = mWorldChain[i - 1];
                mLengths[i] = (world.position - prev).Length();
            }
        }

        if (size > 0)
            mLengths[0] = 0.0f;
    }

    void FABRIK::WorldToIKChain()
    {
        uint32_t size = IKChainSize();
        if (size == 0)
            return;
        for (uint32_t i = 0; i < size - 1; ++i)
        {
            Transform3f world = GetGlobalTransform(i);
            Transform3f next = GetGlobalTransform(i + 1);
            Vector3f position = world.position;
            Quaternionf rotation = world.rotation;

            Vector3f toNext = next.position - position;
            toNext = Quaternionf::Inverse(rotation) * toNext;

            Vector3f toDesired = mWorldChain[i + 1] - position;
            toDesired = Quaternionf::Inverse(rotation) * toDesired;

            Quaternionf delta = Quaternionf::FromTo(toNext, toDesired);
            mIKChain[i].rotation = delta * mIKChain[i].rotation;
        }
    }

    void FABRIK::IterateForward(const Vector3f &base)
    {
        uint32_t size = IKChainSize();

        if (size > 0)
            mWorldChain[0] = base;

        for (int i = 1; i < size; ++i)
        {
            Vector3f direction = Vector3f::Normalize(mWorldChain[i] - mWorldChain[i - 1]);
            Vector3f offset = direction * mLengths[i];
            mWorldChain[i] = mWorldChain[i - 1] + offset;
        }
    }

    void FABRIK::IterateBackword(const Vector3f &goal)
    {
        int32_t size = IKChainSize();

        if (size > 0)
            mWorldChain[size - 1] = goal;
        for (int32_t i = size - 2; i >= 0; --i)
        {
            Vector3f direction = Vector3f::Normalize(mWorldChain[i] - mWorldChain[i + 1]);
            Vector3f offset = direction * mLengths[i + 1];
            mWorldChain[i] = mWorldChain[i + 1] + offset;
        }
    }
}