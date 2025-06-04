#include "AnimationTexture.hpp"
#include <fstream>
#include <iostream>
#include "Render/Renderer.hpp"
namespace RealSix
{

    AnimationTexture::AnimationTexture()
    {
        mSize = 0;

        mAnimationTexture = nullptr;
    }

    AnimationTexture::~AnimationTexture()
    {
    }

    void AnimationTexture::Load(StringView path)
    {
        std::ifstream file;
        file.open(path.CString(), std::ios::in | std::ios::binary);

        if (!file.is_open())
            std::cout << "Couldn't open" << path << "\n";

        file >> mSize;

        mData.resize(mSize * mSize * 4);

        file.read((char *)mData.data(), sizeof(float) * mData.size());

        file.close();

        DataToGPUTexture();
    }
    void AnimationTexture::Save(StringView path)
    {
        std::ofstream file;
        file.open(path.CString(), std::ios::out | std::ios::binary);

        if (!file.is_open())
            std::cout << "Couldn't open " << path << "\n";
        file << mSize;
        if (mSize != 0)
            file.write((char *)mData.data(), sizeof(float) * mData.size());

        file.close();
    }

    void AnimationTexture::Bake(Skeleton &skeleton, Clip &clip)
    {
        Pose pose = skeleton.GetBindPose();
        for (uint32_t x = 0; x < mSize; ++x)
        {
            float t = (float)x / (float)(mSize - 1);
            float startTime = clip.GetStartTime();
            float time = startTime + clip.GetDuration() * t;
            clip.Sample(pose, time);

            for (uint32_t y = 0; y < pose.BoneSize() * 3; y += 3)
            {
                Transform3f node = pose.GetGlobalTransform(y / 3);
                SetTexel(x, y + 0, node.position);
                SetTexel(x, y + 1, node.rotation);
                SetTexel(x, y + 2, node.scale);
            }
        }
        DataToGPUTexture();
    }

    uint32_t AnimationTexture::TextureSize() const
    {
        return mSize;
    }

    void AnimationTexture::Resize(uint32_t size)
    {
        mSize = size;
        mData.resize(mSize * mSize * 4);
    }

    const std::vector<float> &AnimationTexture::GetData() const
    {
        return mData;
    }

    void AnimationTexture::SetTexel(uint32_t x, uint32_t y, const Vector3f &v)
    {
        uint32_t index = (y * mSize + x) * 4;
        mData[index + 0] = v.x;
        mData[index + 1] = v.y;
        mData[index + 2] = v.z;
        mData[index + 3] = 0.0f;
    }

    void AnimationTexture::SetTexel(uint32_t x, uint32_t y, const Quaternionf &v)
    {
        uint32_t index = (y * mSize + x) * 4;
        mData[index + 0] = v.x;
        mData[index + 1] = v.y;
        mData[index + 2] = v.z;
        mData[index + 3] = v.w;
    }

    Vector4f AnimationTexture::GetTexel(uint32_t x, uint32_t y) const
    {
        uint32_t index = (y * mSize + x) * 4;
        return Vector4f(mData[index + 0], mData[index + 1], mData[index + 2], mData[index + 3]);
    }

    void AnimationTexture::DataToGPUTexture()
    {
        // GL::Texture2DCreateInfo info;
        // info.wrapS = GL::WrapMode::CLAMP_TO_EDGE;
        // info.wrapT = GL::WrapMode::CLAMP_TO_EDGE;
        // info.magFilter = GL::FilterMode::LINEAR;
        // info.minFilter = GL::FilterMode::LINEAR;
        // info.needMipMap = false;
        // info.channelMode = GL::ChannelMode::RGBA32F;
        // info.extent = Vector2u32(mSize, mSize);
        // info.data = mData.data();

        // TODO:Not Implement yet!
        GfxTextureDesc textureDesc;

        mAnimationTexture.reset(IGfxTexture::Create(Renderer::GetGfxDevice(), textureDesc));
    }
}