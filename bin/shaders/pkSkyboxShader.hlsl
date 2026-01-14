Texture2D skyboxMap : register(t0);

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

float4
sRGBToLinear(float4 color)
{
  // Convert sRGB to linear color space
  return float4(pow(abs(color.rgb), 1.0f / 2.2f), color.a);
}

float4 PS(PS_INPUT input) : SV_Target
{
  // Reconstruct view-space direction
  float2 ndc = input.TexCoord * 2.0f - 1.0f;
  ndc.y = -ndc.y;
  
  float4 clipPos = float4(ndc, 1.0f, 1.0f);
  
  matrix viewMat = ViewTransp;
  viewMat[3] = float4(0.0f, 0.0f, 0.0f, 1.0f);
  
  float4 viewPos = mul(clipPos, viewMat);
  viewPos.w = 1.0f;
  viewPos = mul(viewPos, ProjTransp);
  viewPos.z = viewPos.w;
  
  float3 viewDir = normalize(viewPos.xyz);
  float2 skyboxUV = getSkyBoxUV(viewDir);
  float3 color = skyboxMap.SampleLevel(samState, skyboxUV, 0).rgb;
  
  return float4(color, 1.0f);
}