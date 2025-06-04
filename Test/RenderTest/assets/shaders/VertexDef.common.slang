struct VertexInput
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

struct VSOutput
{
    float4 position : SV_Position;
    float4 color : COLOR0;
    float2 texcoord0 : TEXCOORD0;
};
