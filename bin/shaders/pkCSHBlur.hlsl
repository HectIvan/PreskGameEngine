
#pragma kernel CS_Main

// unordered texture for reading/writing.
RWTexture2D<float4> specTexture : register(u0);
// resources
Texture2D specularTex : register(t0);
//sampler
SamplerState samState : register(s0);

cbuffer WinSize : register(b0)
{
    float2 WinSize;
    float2 unused;
};

struct PS_INPUT
{
    float4 Position : SV_Position;
    float2 TexCoord : TEXCOORD0;
};

struct PS_OUTPUT
{
    float4 luminance : SV_Target0;
};

[numthreads(16, 16, 1)]
void
CSMain(uint3 dtID : SV_DispatchThreadID)
{
  if (dtID.x >= WinSize.x || dtID.y >= WinSize.y)
  {
    return;
  }

  float2 coord = dtID.xy / WinSize;
  float2 texelSize = float2(1.0f / WinSize.x, 1.0f / WinSize.y);
  
  float factor = 50.0f;

  float2 offsets[5] =
  {
    float2(-2.0f, 0.0f) * factor * texelSize,
    float2(1.0f, 0.0f) * factor * texelSize,
    float2(0.0f, 0.0f),
    float2(1.0f, 0.0f) * factor * texelSize,
    float2(2.0f, 0.0f) * factor * texelSize
  };

  float kernel[5] = { 0.1f, 0.2f, 0.4f, 0.2f, 0.1f };

  float3 sum = float3(0.0f, 0.0f, 0.0f);

  for (int i = 0; i < 5; ++i)
  {
    float3 color = specularTex.Load(int3(dtID.xy, 0)).rgb;
    sum += color * kernel[i];
  }

  specTexture[dtID.xy] = float4(sum, 1.0f);
}