struct StaticVertexInput
{
    [[vk::location(0)]]
    float3 position : POSITION;
    [[vk::location(1)]]
    float3 normal : NORMAL;
    [[vk::location(2)]]
    float3 tangent : TANGENT;
    [[vk::location(3)]]
    float3 binormal : BINORMAL;
    [[vk::location(4)]]
    float4 color : COLOR;
    [[vk::location(5)]]
    float2 uv0 : TEXCOORD0;
};

struct SkeletalVertexInput : StaticVertexInput
{
    [[vk::location(6)]]
    uint4 boneIndices : BONE_INDICES;
    [[vk::location(7)]]
    uint4 boneWeights : BONE_WEIGHTS;
};

#ifdef USE_SKELETAL_VERTEX
    typedef SkeletalVertexInput VertexInput;
#else
    typedef StaticVertexInput VertexInput;
#endif

struct VertexShaderParameter
{
    float4 positionWS : SV_Position;
    float4 color : COLOR0;
    float2 uv0 : TEXCOORD0;
};
