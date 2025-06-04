#pragma once
#include "Math/Transform.hpp"
#include "Math/Matrix4.hpp"
#include "Math/DualQuaternion.hpp"
#include <vector>
#include <tuple>

namespace RealSix
{
    class Pose
    {
    public:
        Pose(/* args */);
        Pose(uint32_t boneNum);
        Pose(const Pose &p);
        Pose &operator=(const Pose &p);
        ~Pose();

        void Resize(uint32_t size);
        uint32_t BoneSize() const;

        int GetParent(uint32_t index) const;
        void SetParent(uint32_t index, int parent);

        Transform3f GetLocalTransform(uint32_t index) const;
        void SetLocalTransform(uint32_t index, const Transform3f &transform);

        Transform3f GetGlobalTransform(uint32_t index) const;
        Transform3f operator[](uint32_t index);

        std::vector<Matrix4f> GetMatrixPalette() const;

        std::vector<DualQuaternionf> GetDualQuaternionPalette() const;

        DualQuaternionf GetGlobalDualQuaternion(uint32_t index) const;

        bool operator==(const Pose &other);
        bool operator!=(const Pose &other);

    protected:
        std::vector<std::tuple<int, Transform3f>> mBones; //int表示该骨骼的父节点,transform表示骨骼的SRT变换
    };
}