#include "EditorUIPassVulkanImpl.hpp"
#ifndef IMGUI_IMPL_VULKAN_HAS_DYNAMIC_RENDERING
#define IMGUI_IMPL_VULKAN_HAS_DYNAMIC_RENDERING
#endif
#include <imgui.h>
#include <imgui_impl_vulkan.h>
#include <imgui_impl_sdl3.h>
#include <vulkan/vulkan.h>
#include <vector>
#include "EditorApp.hpp"
#include "Render/Renderer.hpp"
#include "Gfx/VK/GfxVulkanDevice.hpp"
#include "Platform/SDL3/SDL3Window.hpp"
#include "Platform/SDL3/SDL3InputSystem.hpp"
namespace RealSix
{
    static void check_vk_result(VkResult err)
    {
        if (err == 0)
            return;
        fprintf(stderr, "[vulkan] Error: VkResult = %d\n", err);
        if (err < 0)
            abort();
    }

    EditorUIPassVulkanImpl::EditorUIPassVulkanImpl(EditorApp *editorApp)
        : mEditorApp(editorApp)
    {
        auto sdlInputSystem = static_cast<SDL3InputSystem *>(mEditorApp->GetInputSystem());
        sdlInputSystem->RegisterEventCallback([](SDL_Event event)
                                              { ImGui_ImplSDL3_ProcessEvent(&event); });
    }

    EditorUIPassVulkanImpl::~EditorUIPassVulkanImpl()
    {
        GfxVulkanDevice *vulkanDevice = static_cast<GfxVulkanDevice *>(Renderer::GetGfxDevice());
        vulkanDevice->WaitIdle();

        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();

        vkDestroyDescriptorPool(vulkanDevice->GetLogicDevice(), mDescriptorPool, nullptr);
    }

    void EditorUIPassVulkanImpl::Init()
    {
        std::vector<VkDescriptorPoolSize> poolSizes = {
            {VK_DESCRIPTOR_TYPE_SAMPLER, 1000},
            {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000},
            {VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000},
            {VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000},
            {VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000},
            {VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000},
            {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000},
            {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000},
            {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000},
            {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000},
            {VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000},
        };

        VkDescriptorPoolCreateInfo poolInfo{};
        ZeroVulkanStruct(poolInfo, VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO);
        poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
        poolInfo.pPoolSizes = poolSizes.data();
        poolInfo.maxSets = 256 * poolSizes.size();

        GfxVulkanDevice *vulkanDevice = static_cast<GfxVulkanDevice *>(Renderer::GetGfxDevice());

        VK_CHECK(vkCreateDescriptorPool(vulkanDevice->GetLogicDevice(), &poolInfo, nullptr, &mDescriptorPool));

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        // ImGui::StyleColorsClassic();

        // Setup scaling
        float main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
        ImGuiStyle &style = ImGui::GetStyle();
        style.ScaleAllSizes(main_scale); // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
        style.FontScaleDpi = main_scale; // Set initial font scale. (using io.ConfigDpiScaleFonts=true makes this unnecessary. We leave both here for documentation purpose)

        auto swapChain = static_cast<GfxVulkanSwapChain *>(Renderer::GetGfxDevice()->GetSwapChain());
        auto swapChainColorFormat = swapChain->GetColorTextureFormat();
        auto swapChainDepthFormat = swapChain->GetDepthTextureFormat();

        VkPipelineRenderingCreateInfoKHR pipelineRenderingInfo;
        ZeroVulkanStruct(pipelineRenderingInfo, VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO);
        pipelineRenderingInfo.pNext = nullptr;
        pipelineRenderingInfo.colorAttachmentCount = 1;
        pipelineRenderingInfo.pColorAttachmentFormats = &swapChainColorFormat;
        pipelineRenderingInfo.depthAttachmentFormat = swapChainDepthFormat;
        pipelineRenderingInfo.stencilAttachmentFormat = VK_FORMAT_UNDEFINED;

        // Setup Platform/Renderer backends
        ImGui_ImplSDL3_InitForVulkan(static_cast<SDL3Window *>(Renderer::GetGfxDevice()->GetSwapChain()->GetWindow())->GetHandle());
        ImGui_ImplVulkan_InitInfo init_info = {};
        init_info.Instance = vulkanDevice->GetInstance();
        init_info.PhysicalDevice = vulkanDevice->GetPhysicalDevice();
        init_info.Device = vulkanDevice->GetLogicDevice();
        init_info.QueueFamily = vulkanDevice->GetPhysicalDeviceSpec().queueFamilyIndices.graphicsFamilyIdx.value();
        init_info.Queue = vulkanDevice->GetGraphicsQueue();
        init_info.PipelineCache = VK_NULL_HANDLE;
        init_info.DescriptorPool = mDescriptorPool;
        init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
        init_info.Allocator = nullptr;
        init_info.MinImageCount = vulkanDevice->GetSwapChain()->GetBackBufferCount();
        init_info.ImageCount = vulkanDevice->GetSwapChain()->GetBackBufferCount();
        init_info.CheckVkResultFn = check_vk_result;
        init_info.UseDynamicRendering = true;
        init_info.PipelineRenderingCreateInfo = pipelineRenderingInfo;
        init_info.MSAASamples = static_cast<VkSampleCountFlagBits>(GfxConfig::GetInstance().GetMsaa());
        ImGui_ImplVulkan_Init(&init_info);

        // Load Fonts
        // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
        // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
        // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
        // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
        // - Read 'docs/FONTS.md' for more instructions and details.
        // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
        // io.Fonts->AddFontDefault();
        // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
        // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
        // io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
        // io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
        // ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
        // IM_ASSERT(font != NULL);
    }

    void EditorUIPassVulkanImpl::BeginRender()
    {
        ImGui_ImplSDL3_NewFrame();
        ImGui_ImplVulkan_NewFrame();
        ImGui::NewFrame();
    }

    void EditorUIPassVulkanImpl::EndRender(bool onlyEditorPass)
    {
        // Rendering
        ImGui::Render();
        ImDrawData *draw_data = ImGui::GetDrawData();
        const bool is_minimized = (draw_data->DisplaySize.x <= 0.0f || draw_data->DisplaySize.y <= 0.0f);
        if (!is_minimized)
        {
            auto swapChain = Renderer::GetGfxDevice()->GetSwapChain();
            if (onlyEditorPass)
            {
                swapChain->GetColorAttachment().loadOp = GfxAttachmentLoadOp::CLEAR;
                swapChain->GetColorAttachment().storeOp = GfxAttachmentStoreOp::STORE;

                swapChain->GetDepthAttachment().loadOp = GfxAttachmentLoadOp::CLEAR;
                swapChain->GetDepthAttachment().storeOp = GfxAttachmentStoreOp::STORE;
            }
            else
            {
                swapChain->GetColorAttachment().loadOp = GfxAttachmentLoadOp::LOAD;
                swapChain->GetColorAttachment().storeOp = GfxAttachmentStoreOp::STORE;
                
                swapChain->GetDepthAttachment().loadOp = GfxAttachmentLoadOp::LOAD;
                swapChain->GetDepthAttachment().storeOp = GfxAttachmentStoreOp::STORE;
            }

            auto vulkanCommandBuffer = static_cast<GfxVulkanCommandBuffer *>(swapChain->GetCurrentBackCommandBuffer());

            vulkanCommandBuffer->BeginRenderPass(swapChain);
            ImGui_ImplVulkan_RenderDrawData(draw_data, vulkanCommandBuffer->GetHandle());
            vulkanCommandBuffer->EndRenderPass();
        }
    }
}