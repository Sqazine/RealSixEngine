#include "EditorUIPass.h"
#include "Config/Config.h"
#include "Logger/Logger.h"
#include "EditorUIPassVulkanImpl.h"
#include "EditorApp.h"
namespace RealSix
{
    EditorUIPass *EditorUIPass::Create(EditorApp* editorApp)
    {
        const GfxConfig &gfxConfig = GfxConfig::GetInstance();
        switch (gfxConfig.GetBackend())
        {
        case GfxBackend::VULKAN:
        {
            return new EditorUIPassVulkanImpl(editorApp);
        }
        case GfxBackend::D3D12:
            REALSIX_LOG_ERROR(TEXT("Not implemented D3D12 device creation yet"));
            break;
        default:
            REALSIX_LOG_ERROR(TEXT("Unreachable GfxBackend: {}"), static_cast<int>(gfxConfig.GetBackend()));
            break;
        }

        REALSIX_LOG_ERROR(TEXT("Unreachable GfxBackend: {}"), static_cast<int>(gfxConfig.GetBackend()));
        return nullptr; // for avoiding compiler warning
    }
}