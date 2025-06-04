
#define WORKGROUP_SIZE 32

struct Uniform {
    uint width;
    uint height;
};
[[vk::binding(0, 0)]]
ConstantBuffer<Uniform> uniform;

[[vk::binding(1, 0)]]
RWStructuredBuffer<float4> imageData;

[shader("compute")]
[numthreads(WORKGROUP_SIZE, WORKGROUP_SIZE, 1)]
void main(uint3 threadId: SV_DispatchThreadID)
{
    if (threadId.x >= uniform.width || threadId.y >= uniform.height)
    {
        return;
    }

    float2 uv = float2(0,0);

    uv.x = (float(threadId.x) / uniform.width);
    uv.y = (float(threadId.y) / uniform.height);

    float n = 0.0;
    float2 c = float2(-0.445, 0.0) + (uv - 0.5) * (2.0 + 1.7 * 0.2);
    float2 z = float2(0.0);
    const int M = 128;
    for (int i = 0; i < M; i++)
    {
        z = float2(z.x * z.x - z.y * z.y, 2.0 * z.x * z.y) + c;
        if (dot(z, z) > 2)
            break;
        ++n;
    }

    float t = float(n) / float(M);
    float3 d = float3(0.3, 0.3, 0.5);
    float3 e = float3(-0.2, -0.3, -0.5);
    float3 f = float3(2.1, 2.0, 3.0);
    float3 g = float3(0.0, 0.1, 0.0);
    float4 color = float4(d + e * cos(6.28318 * (f * t + g)), 1.0);

    imageData[(uint)uniform.width * threadId.y + threadId.x] = color;
}