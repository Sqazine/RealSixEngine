#pragma once
#include <vulkan/vulkan.h>
#include "Gfx/IGfxPipeline.h"
#include "Gfx/IGfxDevice.h"
#include "GfxVulkanObject.h"
namespace RealSix
{
    class GfxVulkanRasterPipeline : public GfxVulkanObject, public IGfxRasterPipeline
    {
    public:
        GfxVulkanRasterPipeline(IGfxDevice *device, const GfxRasterPipelineStateDesc & pipelineState);
        ~GfxVulkanRasterPipeline() override;

        VkPipeline GetHandle() const { return mHandle; }

    private:
        void Create();

        VkPipeline mHandle;
    };

     class GfxVulkanComputePipeline : public GfxVulkanObject, public IGfxComputePipeline
    {
    public:
        GfxVulkanComputePipeline(IGfxDevice *device, IGfxComputeShader *shader);
        ~GfxVulkanComputePipeline() override;

        VkPipeline GetHandle() const { return mHandle; }

    private:
        void Create();

        VkPipeline mHandle;
    };
}