// Resources
Texture2D finalRT : register(t0);

// sampler
SamplerState samState : register(s0);

cbuffer Exposure : register(b0)
{
  float exposure;
  float3 _expUnused;
}

struct PS_INPUT
{
  float4 Position : SV_Position;
  float2 TexCoord : TEXCOORD0;
};

float4
sRGBToLinear(float4 color)
{
  // Convert sRGB to linear color space
  return float4(pow(abs(color.rgb), 1.0f / 2.2f), color.a);
}

float4 PS(PS_INPUT input) : SV_Target0
{
  float4 finalColor = finalRT.Sample(samState, input.TexCoord);
  
  // exposure tone mapping
  float3 mapped = float3(1.0f.xxx) - exp(-finalColor.rgb * exposure);

  // gamma correction
  return sRGBToLinear(float4(mapped, 1.0f));
}