#include "meshdrawPass.common.slang"

struct VSInput
{
    [[vk::location(0)]]
    float3 inPosition : POSITION;
    [[vk::location(1)]]
    float3 inNormal : NORMAL;
    [[vk::location(2)]]
    float3 inTangent : TANGENT;
    [[vk::location(3)]]
    float3 inBinormal : BINORMAL;
    [[vk::location(4)]]
    float4 inColor : COLOR;
    [[vk::location(5)]]
    float2 inTexCoord : TEXCOORD0;
};

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
VSOutput main(VSInput vsInput)
{
    VSOutput vsOutput;
    vsOutput.position = mul(cameraData.proj, mul(cameraData.view, mul(meshUBO.model, float4(vsInput.inPosition, 1.0))));
    vsOutput.fragColor = vsInput.inColor;
    vsOutput.fragTexCoord = vsInput.inTexCoord;

    return vsOutput;
}
