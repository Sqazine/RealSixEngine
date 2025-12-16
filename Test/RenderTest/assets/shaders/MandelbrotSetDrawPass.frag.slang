#include "VertexDef.common.slang"

struct PixelOutput
{
    float4 color : SV_TARGET;
};

struct Uniform {
    uint width;
    uint height;
};
[[vk::binding(0, 0)]]
ConstantBuffer<Uniform> uniform;

[[vk::binding(1, 0)]]
StructuredBuffer<float4> imageData;

[shader("pixel")]
PixelOutput main(VertexShaderParameter vertexShaderParameter)
{
    int2 actualUV = int2(int(vertexShaderParameter.uv0.x * uniform.width), int(vertexShaderParameter.uv0.y * uniform.height));

    PixelOutput output;
    output.color = imageData[int(uniform.width) * actualUV.y + actualUV.x];
    return output;
}