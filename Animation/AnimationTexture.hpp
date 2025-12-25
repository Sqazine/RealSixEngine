#pragma once
#include <vector>

#include <memory>
#include "Gfx/IGfxTexture.hpp"
#include "Math/Vector3.hpp"
#include "Math/Quaternion.hpp"
#include "Math/Vector4.hpp"
#include "Skeleton.hpp"
#include "AnimationClip.hpp"

namespace RealSix
{
    class AnimationTexture
    {
    public:
        AnimationTexture();
        ~AnimationTexture();

        void Load(StringView path);
        void Save(StringView path);

        void Bake(Skeleton &skeleton, AnimationTransformClip &clip);

        uint32_t TextureSize() const;

        void Resize(uint32_t size);

        const std::vector<float> &GetData() const;

        void SetTexel(uint32_t x, uint32_t y, const Vector3f &v);
        void SetTexel(uint32_t x, uint32_t y, const Quaternionf &v);
        Vector4f GetTexel(uint32_t x, uint32_t y) const;

    protected:
        void DataToGPUTexture();

        std::vector<float> mData;
        uint32_t mSize;

        std::unique_ptr<IGfxTexture> mAnimationTexture;
    };
}