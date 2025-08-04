Texture2D skyboxMap : register(t0);
Texture2D normalMap : register(t1);

SamplerState samState : register(s0);

#define PI 3.14159265f

cbuffer ViewTransposed : register(b0)
{
  matrix ViewTransp;
}

cbuffer ProjTransposed : register(b1)
{
  matrix ProjTransp;
}

struct PS_INPUT
{
  float4 Position : SV_Position;
  float2 TexCoord : TEXCOORD0;
};

float2 getSkyBoxUV(float3 dir)
{
  // https://en.wikipedia.org/wiki/UV_mapping#Finding_UV_on_a_sphere
  float u = 0.5f + atan2(-dir.z, dir.x) / (PI * 2.0f);
  float v = 0.5f - asin(dir.y) / PI;
  return float2(u, v);
}

float4 PS(PS_INPUT input) : SV_Target
{
  // Reconstruct view-space direction
  float4 normalTex = normalMap.Sample(samState, input.TexCoord);
  
  float3 viewDir = normalize(normalTex.xyz);
  float2 skyboxUV = getSkyBoxUV(viewDir);
  float3 color = skyboxMap.SampleLevel(samState, skyboxUV, 0.0f).rgb;
  
  return float4(color, 1.0f);
}