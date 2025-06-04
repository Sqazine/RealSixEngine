#pragma once
#include "Pose.h"
#include "Math/Matrix4.h"
#include "Math/DualQuaternion.h"
#include <vector>
#include <string>

namespace RealSix
{
    class Skeleton
    {
    public:
        Skeleton();
        Skeleton(const Pose &rest, const Pose &bind, const std::vector<std::string> &names);
        ~Skeleton();

        void Set(const Pose &rest, const Pose &bind, const std::vector<std::string> &names);

        const Pose &GetBindPose() const;
        const Pose &GetRestPose() const;

        const std::vector<Matrix4f> &GetInverseBindPoseMatrix4Form() const;
        const std::vector<DualQuaternionf> &GetInverseBindPoseDualQuaternionForm() const;

        const std::vector<std::string> &GetBoneNames() const;

        const std::string &GetBoneName(uint32_t index) const;

    protected:
        void UpdateInverseBindPoseMatrix4Form();
        void UpdateInverseBindPoseDualQuaternionForm();

        Pose mRestPose;
        Pose mBindPose;

        std::vector<Matrix4f> mInverseBindPoseMatrix4Form;
        std::vector<DualQuaternionf> mInverseBindPoseDualQuaternionForm;

        std::vector<std::string> mBoneNames;
    };
}