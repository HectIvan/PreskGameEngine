// Resources
Texture2D actorsRender : register(t0);
Texture2D actorsTransparentRender : register(t1);
Texture2D skyboxRender : register(t2);

// sampler
SamplerState samState : register(s0);

struct PS_INPUT
{
  float4 Position : SV_Position;
  float2 TexCoord : TEXCOORD0;
};

float4 PS(PS_INPUT input) : SV_Target0
{
  float4 actorSample = actorsRender.Sample(samState, input.TexCoord);
  float4 actorTransparentSample = actorsTransparentRender.Sample(samState, input.TexCoord);
  float3 skyboxSample = skyboxRender.Sample(samState, input.TexCoord).rgb;
  
  float3 finalColor = float3(0, 0, 0);
  
  if (actorSample.a <= 0.0f)
  {
    return float4(skyboxSample, 1.0f);
  }
  else
  {
    finalColor = lerp(actorSample.rgb, actorTransparentSample.rgb, actorTransparentSample.a);
  }
  
  return float4(finalColor, 1.0f);
}