SamplerState samLinear : register(s0);

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