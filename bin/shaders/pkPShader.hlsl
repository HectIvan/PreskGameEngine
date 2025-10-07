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
  float4 diffuse : SV_Target0;
  float4 normal : SV_Target1;
  float4 orm : SV_Target2; // occlusion, roughness, metallic
  float4 emissive : SV_Target4;
  float4 posWS : SV_Target5; // world space position
};

PS_OUTPUT PS(PS_INPUT input)
{
  PS_OUTPUT output = (PS_OUTPUT) 0;
  // texture sampling
  float4 AO = occlusionTex.Sample(samLinear, input.Tex);
  float4 colorSam = diffuseTex.Sample(samLinear, input.Tex);
  float4 metallicSam = metallicTex.Sample(samLinear, input.Tex);
  float3 normalSam = normalTex.Sample(samLinear, input.Tex).rgb * 2.0f - 1.0f;
  float4 roughSam = roughnessTex.Sample(samLinear, input.Tex);
  float4 emissSam = emissiveTex.Sample(samLinear, input.Tex);

  if (colorSam.a <= 0.1f) {
    clip(-1);
  }
  // modify the normal vector 
  float3x3 TBN = float3x3(input.Tangent, input.Bitangent, input.Normal);
  normalSam = normalize(mul(normalSam, TBN));
  // fill up all outputs with their respective values.
  output.normal = float4(normalSam, 1.0f);
  output.diffuse = float4(colorSam.rgb, 1.0f);
  output.orm = float4(AO.r, roughSam.g, metallicSam.b, 1.0f);
  output.emissive = emissSam;
  output.posWS = float4(input.PosWS, 1.0f);
  
  return output;
}