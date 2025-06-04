#pragma once
#include <vector>
#include <string_view>
#include <memory>
#include "Gfx/IGfxTexture.h"
#include "Math/Vector3.h"
#include "Math/Quaternion.h"
#include "Math/Vector4.h"
#include "Skeleton.h"
#include "Clip.h"

namespace RealSix
{
    class AnimationTexture
    {
    public:
        AnimationTexture();
        ~AnimationTexture();

        void Load(std::string_view path);
        void Save(std::string_view path);

        void Bake(Skeleton &skeleton, Clip &clip);

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