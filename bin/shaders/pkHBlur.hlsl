Texture2D textureResource : register(t0);
SamplerState samState : register(s0);

// constants
float KernelOffsets[3] = { 0.0f, 1.3846153846f, 3.2307692308f };
float BlurWeights[3] = { 0.2270270270f, 0.3162162162f, 0.0702702703f };

cbuffer WinSize : register(b0)
{
  float2 WinSize;
  float blurXOffset;
  float unused;
};

struct PS_INPUT
{
  float4 Position : SV_Position;
  float2 TexCoord : TEXCOORD0;
};

struct PS_OUTPUT
{
  float4 hblur : SV_Target0;
};

float4 PS(PS_INPUT input) : SV_Target0
{
  float4 texSample = textureResource.Sample(samState, input.TexCoord);
  return texSample;
}
