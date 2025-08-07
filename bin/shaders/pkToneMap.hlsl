// Resources
Texture2D baseTexture : register(t0);
Texture2D luminance : register(t1);

// sampler
SamplerState samState : register(s0);

struct PS_INPUT
{
    float4 Position : SV_Position;
    float2 TexCoord : TEXCOORD0;
};  

float4 PS(PS_INPUT input) : SV_Target0
{
  float4 finalColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
  // Get texture resources
  float4 baseSample = baseTexture.Sample(samState, input.TexCoord);
  float4 lumBlurSample = luminance.Sample(samState, input.TexCoord);
  
  finalColor = baseSample + lumBlurSample;

  return finalColor;
}