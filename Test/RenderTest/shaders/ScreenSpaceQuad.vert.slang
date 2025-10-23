#include "VertexDef.common.slang"

[shader("vertex")]
VSOutput main(uint vertexId: SV_VertexID)
{
    VSOutput output;
    output.texcoord0 = float2(((vertexId << 1) & 2), ((vertexId)&2));
    output.position = float4(output.texcoord0 * 2.0f - 1.0f, 0.0f, 1.0f);
    return output;
}