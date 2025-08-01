Texture2D skyboxMap : register(t0);
Texture2D depthMap : register(t1);

SamplerState samState : register(s0);

#define PI 3.14159265f

cbuffer CameraData : register(b0)
{
  float4 Eye; // 16
  float3 ForwardCam; // 28
  float4x4 ViewCam; // 92
  float4x4 ProjectionCam; // 156
  float _unusedCam0; // 160
  float4 _padding; // 172
};

cbuffer Transform : register(b1)
{
  float4x4 transformCam;
};

cbuffer VI : register(b2)
{
  float4x4 InvView;
}

cbuffer ProjI : register(b3)
{
  float4x4 InvProj;
}

cbuffer ViewPort : register(b4)
{
  float2 ViewPortSize;
  float2 _VPPadding;
}

cbuffer InViewProj : register(b5)
{
  matrix InvViewProj;
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
  float2 ndc = input.TexCoord * 2.0f - 1.0f;
  ndc.y = -ndc.y;
  
  float3 clipPos = normalize(float3(ndc.x, ndc.y, 1.0f));
  
  float3 viewPos = mul(clipPos, (float3x3) InvView);
  
  float3 viewDir = normalize(viewPos);
  float2 skyboxUV = getSkyBoxUV(viewDir);
  float3 color = skyboxMap.Sample(samState, skyboxUV).rgb;
  
  return float4(color, 1.0f);
}