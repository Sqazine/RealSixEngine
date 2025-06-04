#include "Pose.hpp"
#include "Math/Quaternion.hpp"

namespace RealSix
{

    Pose::Pose(/* args */)
    {
    }
    Pose::Pose(uint32_t boneNum)
    {
        Resize(boneNum);
    }
    Pose::Pose(const Pose &p)
    {
        *this = p;
    }
    Pose &Pose::operator=(const Pose &p)
    {
        if (this == &p) //地址相同
            return *this;

        if (mBones.size() != p.mBones.size())
            mBones.resize(p.mBones.size());

        if (mBones.size() != 0)
            memcpy(&mBones[0], &p.mBones[0], mBones.size() * sizeof(std::tuple<int, Transform3f>));
        return *this;
    }
    Pose::~Pose()
    {
    }

    void Pose::Resize(uint32_t size)
    {
        mBones.resize(size);
    }
    uint32_t Pose::BoneSize() const
    {
        return mBones.size();
    }

    int Pose::GetParent(uint32_t index) const
    {
        return std::get<0>(mBones[index]);
    }
    void Pose::SetParent(uint32_t index, int parent)
    {
        mBones[index] = std::make_tuple(parent, std::get<1>(mBones[index]));
    }

    Transform3f Pose::GetLocalTransform(uint32_t index) const
    {
        return std::get<1>(mBones[index]);
    }
    void Pose::SetLocalTransform(uint32_t index, const Transform3f &transform)
    {
        mBones[index] = std::make_tuple(std::get<0>(mBones[index]), transform);
    }

    Transform3f Pose::GetGlobalTransform(uint32_t index) const
    {
        auto result = std::get<1>(mBones[index]);
        for (int p = std::get<0>(mBones[index]); p >= 0; p = std::get<0>(mBones[p]))
            result = Transform3f::Combine(std::get<1>(mBones[p]), result);
        return result;
    }
    Transform3f Pose::operator[](uint32_t index)
    {
        return GetGlobalTransform(index);
    }

    std::vector<Matrix4f> Pose::GetMatrixPalette() const
    {
        std::vector<Matrix4f> result;
        for (uint32_t i = 0; i < BoneSize(); ++i)
        {
            Transform3f t = GetGlobalTransform(i);
            result.emplace_back(Transform3f::ToMatrix4(t));
        }
        return result;
    }

    std::vector<DualQuaternionf> Pose::GetDualQuaternionPalette() const
    {
        std::vector<DualQuaternionf> result;

        for (uint32_t i = 0; i < BoneSize(); ++i)
            result.emplace_back(GetGlobalDualQuaternion(i));

        return result;
    }

    DualQuaternionf Pose::GetGlobalDualQuaternion(uint32_t index) const
    {
        DualQuaternionf result = Transform3f::ToDualQuaternion(std::get<1>(mBones[index]));
        for (int p = std::get<0>(mBones[index]); p >= 0; p = std::get<0>(mBones[p]))
        {
            DualQuaternionf parent = Transform3f::ToDualQuaternion(std::get<1>(mBones[p]));
            result = result * parent;
        }
        return result;
    }

    bool Pose::operator==(const Pose &other)
    {
        if (mBones.size() != other.BoneSize())
            return false;

        uint32_t size = mBones.size();

        for (uint32_t i = 0; i < size; ++i)
        {
            Transform3f thisLocal = std::get<1>(mBones[i]);
            Transform3f otherLocal = std::get<1>(other.mBones[i]);
            int thisParent = std::get<0>(mBones[i]);
            int otherParent = std::get<0>(other.mBones[i]);

            if (thisParent != otherParent)
                return false;
            if (thisLocal.position != otherLocal.position)
                return false;
            if (thisLocal.rotation != otherLocal.rotation)
                return false;
            if (thisLocal.scale != otherLocal.scale)
                return false;
        }
        return true;
    }
    bool Pose::operator!=(const Pose &other)
    {
        return !(*this == other);
    }
}