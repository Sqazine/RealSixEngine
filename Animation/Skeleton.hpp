#pragma once
#include "Pose.hpp"
#include "Math/Matrix4.hpp"
#include "Math/DualQuaternion.hpp"
#include <vector>
#include "String.hpp"

namespace RealSix
{
    class Skeleton
    {
    public:
        Skeleton();
        Skeleton(const Pose &rest, const Pose &bind, const std::vector<String> &names);
        ~Skeleton();

        Skeleton(const Skeleton & other);

        void Set(const Pose &rest, const Pose &bind, const std::vector<String> &names);

        const Pose &GetBindPose() const;
        const Pose &GetRestPose() const;

        const std::vector<Matrix4f> &GetInverseBindPoseMatrix4Form() const;
        const std::vector<DualQuaternionf> &GetInverseBindPoseDualQuaternionForm() const;

        const std::vector<String> &GetBoneNames() const;

        const String &GetBoneName(uint32_t index) const;

    protected:
        void UpdateInverseBindPoseMatrix4Form();
        void UpdateInverseBindPoseDualQuaternionForm();

        Pose mRestPose;
        Pose mBindPose;

        std::vector<Matrix4f> mInverseBindPoseMatrix4Form;
        std::vector<DualQuaternionf> mInverseBindPoseDualQuaternionForm;

        std::vector<String> mBoneNames;
    };
}