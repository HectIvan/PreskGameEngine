Texture2D inputMap : register(t0);
RWTexture2D<uint> outputMap : register(t1);

cbuffer viewportSize : register(b0)
{
    float2 VPSize;
    float2 _unused;
}

[numThreads(32, 32, 1)]
void GS(uint3 id : SV_DispatchThreadID)
{
    
}