#include "TestApp.hpp"
#include "Core/Logger.hpp"
#include "Config/Config.hpp"
#include "Platform/PlatformInfo.hpp"
#include "Editor/EditorUIPass/EditorUIPass.hpp"
#include "BasicMeshPass.hpp"
#include "MandelbrotSetPass.hpp"
#include <imgui.h>

namespace RealSix
{
    void TestApp::SetupConfig()
    {
        GfxConfig::GetInstance().SetMsaa(Msaa::X4);
    }

    void TestApp::Init()
    {
        App::Init();

        mWindow->SetTitle("TestApp");

        FrameGraph &frameGraph = mRenderer->GetFrameGraph();

        AddPasses(frameGraph);
    }

    void TestApp::Tick()
    {
        App::Tick();

        FrameGraph &frameGraph = mRenderer->GetFrameGraph();
        if (mRemoveBasicMeshPass)
        {
            frameGraph.RemoveRenderTask<BasicMeshPass>();
        }
        else
        {
            InsertBasicMeshPass(frameGraph, 1);
        }

        if (mRemoveMandelbrotSetPass)
        {
            frameGraph.RemoveRenderTask<MandelbrotSetPass>();
        }
        else
        {
            InsertMandelbrotSetPass(frameGraph, 0);
        }
    }

    void TestApp::Render()
    {
        App::Render();

        Renderer::GetGfxDevice()->GetSwapChain()->GetColorAttachment().clearValue = mClearColor;
    }

    void TestApp::RenderGizmo()
    {
    }

    void TestApp::RenderUI()
    {
    }

    void TestApp::RenderEditorUI()
    {
        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (mShowDemoWindow)
            ImGui::ShowDemoWindow(&mShowDemoWindow);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");         // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &mShowDemoWindow); // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &mShowAnotherWindow);
            ImGui::Checkbox("Remove Mesh Draw Pass", &mRemoveBasicMeshPass);
            ImGui::Checkbox("Remove Mandelbrot Set Pass", &mRemoveMandelbrotSetPass);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);             // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float *)&mClearColor); // Edit 3 floats representing a color

            if (ImGui::Button("Button")) // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        // 3. Show another simple window.
        if (mShowAnotherWindow)
        {
            ImGui::Begin("Another Window", &mShowAnotherWindow); // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                mShowAnotherWindow = false;
            ImGui::End();
        }
    }

    void TestApp::Destroy()
    {
        App::Destroy();
    }

    void TestApp::PreTick()
    {
        App::PreTick();
    }

    void TestApp::PostTick()
    {
        App::PostTick();
    }

    void TestApp::AddPasses(FrameGraph &frameGraph)
    {
        InsertMandelbrotSetPass(frameGraph, 0);
        InsertBasicMeshPass(frameGraph, 1);
        AddEditorUIPass(frameGraph);
    }
}