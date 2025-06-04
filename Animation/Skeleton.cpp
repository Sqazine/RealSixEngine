#include "Skeleton.hpp"
#include "Math/Transform.hpp"

namespace RealSix
{

    Skeleton::Skeleton()
    {
    }

    Skeleton::Skeleton(const Pose &rest, const Pose &bind, const std::vector<String> &names)
    {
        Set(rest, bind, names);
    }
    Skeleton::~Skeleton()
    {
    }

    Skeleton::Skeleton(const Skeleton &other)
    {
        this->mRestPose = other.mRestPose;
        this->mBindPose = other.mBindPose;
        this->mInverseBindPoseMatrix4Form = other.mInverseBindPoseMatrix4Form;
        this->mInverseBindPoseDualQuaternionForm = other.mInverseBindPoseDualQuaternionForm;
        this->mBoneNames = other.mBoneNames;
    }

    void Skeleton::Set(const Pose &rest, const Pose &bind, const std::vector<String> &names)
    {
        mRestPose = rest;
        mBindPose = bind;
        mBoneNames = names;
        UpdateInverseBindPoseMatrix4Form();
        UpdateInverseBindPoseDualQuaternionForm();
    }

    const Pose &Skeleton::GetBindPose() const
    {
        return mBindPose;
    }
    const Pose &Skeleton::GetRestPose() const
    {
        return mRestPose;
    }

    const std::vector<Matrix4f> &Skeleton::GetInverseBindPoseMatrix4Form() const
    {
        return mInverseBindPoseMatrix4Form;
    }

    const std::vector<DualQuaternionf> &Skeleton::GetInverseBindPoseDualQuaternionForm() const
    {
        return mInverseBindPoseDualQuaternionForm;
    }

    const std::vector<String> &Skeleton::GetBoneNames() const
    {
        return mBoneNames;
    }

    const String &Skeleton::GetBoneName(uint32_t index) const
    {
        return mBoneNames[index];
    }

    void Skeleton::UpdateInverseBindPoseMatrix4Form()
    {
        uint32_t size = mBindPose.BoneSize();
        mInverseBindPoseMatrix4Form.resize(size);

        for (uint32_t i = 0; i < size; ++i)
        {
            Transform3f world = mBindPose.GetGlobalTransform(i);
            mInverseBindPoseMatrix4Form[i] = Matrix4f::Inverse(Transform3f::ToMatrix4(world));
        }
    }

    void Skeleton::UpdateInverseBindPoseDualQuaternionForm()
    {
        uint32_t size = mBindPose.BoneSize();
        mInverseBindPoseDualQuaternionForm.resize(size);

        for (uint32_t i = 0; i < size; ++i)
        {
            DualQuaternionf world = mBindPose.GetGlobalDualQuaternion(i);
            mInverseBindPoseDualQuaternionForm[i] = DualQuaternionf::Conjugate(world);
        }
    }
}