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

struct PS_OUTPUT
{
  float4 posWS : SV_Target;
};

PS_OUTPUT PS(PS_INPUT input)
{
  PS_OUTPUT output = (PS_OUTPUT) 0;
  
  float alpha = diffuseTex.Sample(samLinear, input.Tex).a;
  
  if (alpha != 0.0f) {
    output.posWS = float4(input.PosWS, 1.0f);
  }
  
  return output;
}