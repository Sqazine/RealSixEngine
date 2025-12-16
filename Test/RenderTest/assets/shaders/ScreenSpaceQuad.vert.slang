#include "VertexDef.common.slang"

[shader("vertex")]
VertexShaderParameter main(uint vertexId: SV_VertexID)
{
    VertexShaderParameter output;
    output.uv0 = float2(((vertexId << 1) & 2), ((vertexId)&2));
    output.positionWS = float4(output.uv0 * 2.0f - 1.0f, 0.0f, 1.0f);
    return output;
}