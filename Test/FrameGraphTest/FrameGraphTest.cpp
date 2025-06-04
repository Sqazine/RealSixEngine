#include "Render/FrameGraph/FrameGraph.hpp"
#include "Render/FrameGraph/FrameGraphResource.hpp"
#include "Render/FrameGraph/FrameGraphRenderTaskBuilder.hpp"
#include <array>
#include <memory>
#include <cassert>

namespace GL
{
    using Buffer = std::size_t;
    using Texture1D = std::size_t;
    using Texture2D = std::size_t;
    using Texture3D = std::size_t;
}

namespace GLResource
{
    struct BufferDesc
    {
        std::size_t size;
    };
    struct TextureDescription
    {
        std::size_t levels;
        std::size_t format;
        std::array<std::size_t, 3> size;
    };

    using BufferResource = RealSix::FrameGraphResource<BufferDesc, GL::Buffer>;
    using Texture1DResource = RealSix::FrameGraphResource<TextureDescription, GL::Texture1D>;
    using Texture2DResource = RealSix::FrameGraphResource<TextureDescription, GL::Texture2D>;
    using Texture3DResource = RealSix::FrameGraphResource<TextureDescription, GL::Texture3D>;
}

namespace RealSix
{

    template <>
    std::unique_ptr<GL::Buffer> Realize(const GLResource::BufferDesc &description)
    {
        return std::make_unique<GL::Buffer>(description.size);
    }

    template <>
    std::unique_ptr<GL::Texture2D> Realize(const GLResource::TextureDescription &description)
    {
        return std::make_unique<GL::Buffer>(description.levels);
    }
}
int main(int argc, char *argv[])
{
    RealSix::FrameGraph frameGraph;

    auto retainedResource = frameGraph.AddRetainedResource("SwapChain Image", GLResource::TextureDescription(), static_cast<GL::Texture2D *>(nullptr));

    struct RenderTask1Pass
    {
        GLResource::Texture2DResource *output1;
        GLResource::Texture2DResource *output2;
        GLResource::Texture2DResource *output3;
        GLResource::Texture2DResource *output4;
    };

    auto RenderTask1 = frameGraph.AddRenderTask<RenderTask1Pass>(
        false,
        [&](RenderTask1Pass *pass, RealSix::FrameGraphRenderTaskBuilder &builder)
        {
            pass->output1 = builder.CreateResource<GLResource::Texture2DResource>("GBuffer Albedo", GLResource::TextureDescription());
            pass->output2 = builder.CreateResource<GLResource::Texture2DResource>("GBuffer Normal", GLResource::TextureDescription());
            pass->output3 = builder.CreateResource<GLResource::Texture2DResource>("GBuffer MetalRoughness", GLResource::TextureDescription());
            pass->output4 = builder.Write<GLResource::Texture2DResource>(retainedResource);
        },
        [=](const RenderTask1Pass *pass)
        {
            // Perform actual rendering. You may load resources from CPU by capturing them.
            auto actual1 = pass->output1->GetActualData();
            auto actual2 = pass->output2->GetActualData();
            auto actual3 = pass->output3->GetActualData();
            auto actual4 = pass->output4->GetActualData();
        });

    auto pass1 = RenderTask1->GetPass();
    assert(pass1->output1->GetId() == 1);
    assert(pass1->output2->GetId() == 2);
    assert(pass1->output3->GetId() == 3);

    struct RenderTask2Pass
    {
        GLResource::Texture2DResource *input1;
        GLResource::Texture2DResource *input2;
        GLResource::Texture2DResource *output1;
        GLResource::Texture2DResource *output2;
    };

    auto RenderTask2 = frameGraph.AddRenderTask<RenderTask2Pass>(
        false,
        [&](RenderTask2Pass *pass, RealSix::FrameGraphRenderTaskBuilder &builder)
        {
            pass->input1 = builder.Read(pass1->output1);
            pass->input2 = builder.Read(pass1->output2);
            pass->output1 = builder.Write(pass1->output3);
            pass->output2 = builder.CreateResource<GLResource::Texture2DResource>("SSAO Map", GLResource::TextureDescription());
        },
        [=](const RenderTask2Pass *pass)
        {
            // Perform actual rendering. You may load resources from CPU by capturing them.
            auto actual1 = pass->input1->GetActualData();
            auto actual2 = pass->input2->GetActualData();
            auto actual3 = pass->output1->GetActualData();
            auto actual4 = pass->output2->GetActualData();
        });

    auto pass2 = RenderTask2->GetPass();
    assert(pass2->input1->GetId() == 1);
    assert(pass2->input2->GetId() == 2);
    assert(pass2->output1->GetId() == 3);
    assert(pass2->output2->GetId() == 4);

    struct RenderTask3Pass
    {
        GLResource::Texture2DResource *input1;
        GLResource::Texture2DResource *input2;
        GLResource::Texture2DResource *output;
    };

    auto RenderTask3 = frameGraph.AddRenderTask<RenderTask3Pass>(
        false,
        [&](RenderTask3Pass *pass, RealSix::FrameGraphRenderTaskBuilder &builder)
        {
           pass->input1 = builder.Read(pass2->output1);
           pass->input2 = builder.Read(pass2->output2);
           pass->output = builder.Write(retainedResource);
        },
        [=](const RenderTask3Pass *pass)
        {
            // Perform actual rendering. You may load resources from CPU by capturing them.
            auto actual1 = pass->input1->GetActualData();
            auto actual2 = pass->input2->GetActualData();
            auto actual3 = pass->output->GetActualData();
        });

    frameGraph.Compile();
    for (auto i = 0; i < 100; i++)
        frameGraph.Execute();
    frameGraph.ExportGraphviz("frameGraph.gv");
    frameGraph.Clear();

    return 0;
}