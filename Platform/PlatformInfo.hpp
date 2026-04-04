#pragma once
#include <vector>
#include <vulkan/vulkan.h>
#include "Core/Common.hpp"
#include "Core/Marco.hpp"
#include "Window.hpp"
namespace RealSix
{
    struct DisplayInfo
    {
        uint32_t width{0};
        uint32_t height{0};
        float pixelDensity{0};
        float refreshRate{0};
        StringView format{};
        bool isPrimary{false};
        bool isHDR{false};
    };

    enum class CpuManufacturer
    {
        UNKNOWN,
        INTEL,
        AMD,
        ARM,
        APPLE,
    };

    struct CpuInfo
    {
        String name;
        CpuManufacturer manufacturer{CpuManufacturer::UNKNOWN};
        uint8_t logicCoreCount{0};
        uint8_t physicalCoreCount{0};
    };

    struct MemoryInfo
    {
        size_t totalMemory{0};
        size_t freeMemory{0};
        size_t usedMemory{0};
    };

    class HardwareInfo
    {
    public:
        HardwareInfo() = default;
        virtual ~HardwareInfo()
        {
            mDisplayInfos.clear();
        }

        static HardwareInfo *Create();

        const std::vector<DisplayInfo> &GetDisplayInfos() const
        {
            return mDisplayInfos;
        }

    protected:
        virtual void ObtainDisplayInfo() = 0;

        //TODO: Implement these methods in derived classes
        // virtual void ObtainCpuInfo() = 0;
        // virtual void ObtainMemoryInfo() = 0;

        std::vector<DisplayInfo> mDisplayInfos;
        CpuInfo mCpuInfo;
        MemoryInfo mMemoryInfo;
    };

#if defined(PLATFORM_SUPPORT_VULKAN)
    class VulkanPlatformInfo
    {
    public:
        VulkanPlatformInfo() = default;
        virtual ~VulkanPlatformInfo() = default;

        static VulkanPlatformInfo *Create();

        virtual std::vector<const char *> GetInstanceExtensionList() { return {}; }
        virtual std::vector<const char *> GetDeviceExtensionList() { return {}; }
        virtual VkSurfaceKHR CreateSurface(const Window *window, VkInstance instance) = 0;
    };
#endif

    class PlatformInfo : public Singleton<PlatformInfo>
    {
    public:
        void Init()
        {
            mHardwareInfo.reset(HardwareInfo::Create());
            mVulkanPlatformInfo.reset(VulkanPlatformInfo::Create());
        }

        void CleanUp()
        {
            mHardwareInfo.reset();
        }

        HardwareInfo *GetHardwareInfo() const
        {
            return mHardwareInfo.get();
        }

    private:
        std::unique_ptr<HardwareInfo> mHardwareInfo;

#if defined(PLATFORM_SUPPORT_VULKAN)
    public:
        VulkanPlatformInfo *GetVulkanPlatformInfo() const
        {
            return mVulkanPlatformInfo.get();
        }

    private:
        std::unique_ptr<VulkanPlatformInfo> mVulkanPlatformInfo;
#endif
    };
}