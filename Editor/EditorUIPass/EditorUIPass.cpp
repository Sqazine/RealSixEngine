#include "EditorUIPass.hpp"
#include "Core/Config.hpp"
#include "Core/Logger.hpp"
#include "EditorUIPassVulkanImpl.hpp"
#include "EditorApp.hpp"
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
            REALSIX_LOG_ERROR("Not implemented D3D12 device creation yet");
            break;
        default:
            REALSIX_LOG_ERROR("Unreachable GfxBackend: {}", static_cast<int>(gfxConfig.GetBackend()));
            break;
        }

        REALSIX_LOG_ERROR("Unreachable GfxBackend: {}", static_cast<int>(gfxConfig.GetBackend()));
        return nullptr; // for avoiding compiler warning
    }
}