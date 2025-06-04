#include "Blending.hpp"
namespace RealSix
{
    bool IsInHierarchy(const Pose &pose, uint32_t parent, uint32_t search)
    {
        if (search == parent)
            return true;
        int p = pose.GetParent(search);
        while (p >= 0)
        {
            if (p == (int32_t)parent)
                return true;
            p = pose.GetParent(p);
        }
        return false;
    }

    Pose Blend(const Pose &a, const Pose &b, float t, int root)
    {
        Pose result = a;
        for (uint32_t i = 0; i < a.BoneSize(); ++i)
        {
            if (root >= 0)
                if (!IsInHierarchy(result, root, i))
                    continue;
            result.SetLocalTransform(i, Transform3f::Interpolate(a.GetLocalTransform(i), b.GetLocalTransform(i), t));
        }
        return result;
    }

    Pose MakeAdditivePose(const Skeleton &skeleton, Clip &clip)
    {
        Pose result = skeleton.GetRestPose();
        clip.Sample(result, clip.GetStartTime());
        return result;
    }
    Pose AddAdditivePose(const Pose &inPose, const Pose &addPose, const Pose &additiveBasePose, int blendRoot)
    {
        Pose resultPose = inPose;

        for (uint32_t i = 0; i < addPose.BoneSize(); ++i)
        {
            Transform3f input = resultPose.GetLocalTransform(i);
            Transform3f additive = addPose.GetLocalTransform(i);
            Transform3f additiveBase = additiveBasePose.GetLocalTransform(i);

            if (blendRoot >= 0 && !IsInHierarchy(addPose, blendRoot, i))
                continue;
            //outPose=inPose+(addPose-additiveBasePose)
            Transform3f result(input.position + (additive.position - additiveBase.position),
                               Quaternionf::Normalize(input.rotation * (Quaternionf::Inverse(additive.rotation) * additiveBase.rotation)),
                               input.scale + (additive.scale - additiveBase.scale));
            resultPose.SetLocalTransform(i, result);
        }
        return resultPose;
    }
}