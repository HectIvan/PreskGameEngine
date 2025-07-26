Texture2D skyboxMap : register(t0);
Texture2D depthMap : register(t1);

SamplerState samState : register(s0);

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

cbuffer VPI : register(b2)
{
  float4x4 InvViewProj;
}

struct PS_INPUT
{
  float4 Position : SV_Position;
  float2 TexCoord : TEXCOORD0;
};

float2 getSkyBoxUV(float3 dir)
{
  float u = -atan2(dir.z, dir.x) * 0.1591549431f + 0.5f;
  float v = acos(dir.y) * 0.3183098862f;
  return float2(u, v);
}

float4 PS(PS_INPUT input) : SV_Target
{
  float2 ndc = input.TexCoord * 2.0f - 1.0f;
  ndc.y *= -1.0f;
  
  float4 viewPos = mul(float4(ndc, 1.0f, 1.0f), InvViewProj);
  viewPos /= viewPos.w;
  
  // view direction
  float3 viewDir = normalize(viewPos.xyz);
  float2 skyboxUV = getSkyBoxUV(viewDir);
  // float3 color = skyboxMap.Sample(samState, skyboxUV).rgb;
  float3 color = float3(skyboxUV, 0); // just visualize UVs
  
  return float4(color, 1.0f);
}