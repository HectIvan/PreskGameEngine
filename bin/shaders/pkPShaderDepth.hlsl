Texture2D diffuseTex : register(t0);
Texture2D normalTex : register(t1);
Texture2D heightTex : register(t2);
Texture2D metallicTex : register(t3);
Texture2D occlusionTex : register(t4);
Texture2D roughnessTex : register(t5);
Texture2D emissiveTex : register(t6);

SamplerState samLinear : register(s0);

#define M_PI 3.14159f
#define DELTA 1.0f

struct PS_INPUT
{
  float4 PosSS : SV_POSITION;
  float2 Tex : TEXCOORD0;
  float3 Normal : TEXCOORD1;
  float3 Tangent : TEXCOORD2;
  float3 Bitangent : TEXCOORD3;
  float3 PosWS : TEXCOORD4;
};

float4 PS(PS_INPUT input) : SV_Target0
{
  return float4(input.PosWS, 0.0f);
}