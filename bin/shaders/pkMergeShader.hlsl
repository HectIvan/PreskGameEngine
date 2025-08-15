// Resources
Texture2D albedoMap : register(t0);
Texture2D shdowSpecMap : register(t1);
Texture2D skyboxMap : register(t2);
Texture2D IBRMap : register(t3);
Texture2D emissiveMap : register(t4);
Texture2D emissiveBlurMap : register(t5);

// sampler
SamplerState samState : register(s0);

struct PS_INPUT
{
    float4 Position : SV_Position;
    float2 TexCoord : TEXCOORD0;
};  

float4 PS(PS_INPUT input) : SV_Target0
{
  // Get texture resources.
  float4 albedoSample = albedoMap.Sample(samState, input.TexCoord);
  albedoSample.rgb = pow(albedoSample.rgb, 2.2f);
  float2 shdwSpecSample = shdowSpecMap.Sample(samState, input.TexCoord).rg;
  float4 skyboxSample = skyboxMap.Sample(samState, input.TexCoord);
  float4 IBRSample = IBRMap.Sample(samState, input.TexCoord);
  float4 emissiveSample = emissiveMap.Sample(samState, input.TexCoord);
  float4 emissBlurSample = emissiveBlurMap.Sample(samState, input.TexCoord);
  
  
  float4 diffuse = albedoSample * shdwSpecSample.x;
  float4 IBRMetallic = IBRSample * shdwSpecSample.y;
  
  
  diffuse += IBRMetallic;
  
  // check for a skybox position.
  
  // transparency
  float alpha = albedoSample.a;
  float4 skyboxFinal = float4(skyboxSample.rgb * (1.0f - alpha), 1.0f);
  diffuse = diffuse * alpha + skyboxFinal;
  
  // temporary placeholder for the issue above.
  if (alpha == 0.0f) {
    diffuse = skyboxFinal;
  }
  
  float4 fullEmissive = emissiveSample + emissBlurSample;

  return pow(diffuse + fullEmissive, 1.0f / 2.2f);
}