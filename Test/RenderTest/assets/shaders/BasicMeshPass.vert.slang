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
VertexShaderParameter main(VertexInput vsInput)
{
    VertexShaderParameter vsOutput;
    vsOutput.positionWS = mul(cameraData.proj, mul(cameraData.view, mul(meshUBO.model, float4(vsInput.position, 1.0))));
    vsOutput.color = vsInput.color;
    vsOutput.uv0 = vsInput.uv0;

    return vsOutput;
}
