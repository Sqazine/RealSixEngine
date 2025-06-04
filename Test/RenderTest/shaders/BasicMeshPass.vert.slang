#include "VertexDef.common.slang"

struct CameraData
{
    float4x4 view;
    float4x4 proj;
};

struct MeshUBO
{
    float4x4 model;
};

[[vk::binding(0, 0)]]
ConstantBuffer<CameraData> cameraData : register(b0, space0);

[[vk::binding(1, 0)]]
ConstantBuffer<MeshUBO> meshUBO : register(b1, space0);

[shader("vertex")]
VSOutput main(VertexInput vsInput)
{
    VSOutput vsOutput;
    vsOutput.position = mul(cameraData.proj, mul(cameraData.view, mul(meshUBO.model, float4(vsInput.inPosition, 1.0))));
    vsOutput.color = vsInput.inColor;
    vsOutput.texcoord0 = vsInput.inTexCoord;

    return vsOutput;
}
