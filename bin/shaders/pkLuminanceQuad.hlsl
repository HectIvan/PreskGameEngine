Texture2D inputTexture : register(t0);
SamplerState samState : register(s0);

cbuffer parameters : register(b0)
{
  float2 viewSize;
  float2 threshold;
};

struct PS_INPUT
{
  float4 Position : SV_POSITION;
  float2 Tex : TEXCOORD0;
};

struct PS_Output
{
  float4 diffuse : SV_Target0;
};

float4 PS(PS_INPUT input) : SV_Target0
{
  float2 uv = input.Position.xy / viewSize;
  
  float3 color = inputTexture.Sample(samState, uv).rgb;
  
  float luminance = pow(abs(dot(color, float3(0.2126f, 0.7152f, 0.0722f))), threshold.x);
  
  return float4(luminance.xxx, 1.0f);
}