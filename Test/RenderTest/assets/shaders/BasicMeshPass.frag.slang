#include "VertexDef.common.slang"

struct PixelOutput
{
    float4 color : SV_TARGET;
};

[[vk::binding(1, 1)]]
Sampler2D texSampler : register(s1);

[shader("pixel")]
PixelOutput main(VSOutput vertInput)
{
    PixelOutput pixelOutput;
    pixelOutput.color = texSampler.Sample(vertInput.texcoord0);
    return pixelOutput;
}