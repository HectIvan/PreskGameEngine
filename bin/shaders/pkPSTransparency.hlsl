Texture2D albedoTex : register(t0);
Texture2D normalTex : register(t1);
Texture2D occlusionTex : register(t2);
Texture2D roughnessTex : register(t3);
Texture2D metallicTex : register(t4);
Texture2D emissiveTex : register(t5);

SamplerState samLinear : register(s0);

cbuffer MaterialProps : register(b3)
{
  float3 colorMultiplier; // base color multiplier
  float roughnessFactor; // roughness factor
  float3 emissiveMultiplier; // emissive Multiplier
  float metallicFactor; // metallic factor
};

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
  float4 albedo : SV_Target0; // object color
  float4 normal : SV_Target1; // normal map
  float4 orm : SV_Target2; // occlusion, roughness, metallic
  float4 emissive : SV_Target3; // emissive
  float4 posWS : SV_Target4; // world space position
};

PS_OUTPUT PS(PS_INPUT input)
{
  PS_OUTPUT output = (PS_OUTPUT) 0;
  
  float4 colorSam = albedoTex.Sample(samLinear, input.Tex);
  
  if (colorSam.a > 0.95f) {
    clip(-1.0f);
  }
  
  // texture sampling
  float4 AO = occlusionTex.Sample(samLinear, input.Tex);
  float4 metallicSam = metallicTex.Sample(samLinear, input.Tex);
  float3 normalSam = normalTex.Sample(samLinear, input.Tex).rgb * 2.0f - 1.0f;
  float4 roughSam = roughnessTex.Sample(samLinear, input.Tex);
  float4 emissSam = emissiveTex.Sample(samLinear, input.Tex);
  
  // modify the normal vector 
  float3x3 TBN = float3x3(input.Tangent, input.Bitangent, input.Normal);
  normalSam = normalize(mul(normalSam, TBN));
  output.emissive = float4(emissSam.rgb * emissiveMultiplier, 1.0f);
  output.normal = float4(normalSam, 1.0f);
  output.albedo = float4(colorSam.rgb * colorMultiplier, colorSam.a);
  output.orm = float4(AO.r, roughSam.g * roughnessFactor, metallicSam.b * metallicFactor, 1.0f);
  output.posWS = float4(input.PosWS, 1.0f);
  
  return output;
}