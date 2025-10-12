// Resources
Texture2D albedoMap : register(t0);
Texture2D shdowSpecMap : register(t1);
Texture2D skyboxMap : register(t2);
Texture2D IBLMap : register(t3);
Texture2D emissiveMap : register(t4);
Texture2D emissiveBlurMap : register(t5);
Texture2D ssaoMap : register(t6); // temporary texture to show ambient occlusion effect.

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
  float2 shdwSpecSample = shdowSpecMap.Sample(samState, input.TexCoord).rg;
  float4 skyboxSample = skyboxMap.Sample(samState, input.TexCoord);
  float4 IBLSample = IBLMap.Sample(samState, input.TexCoord);
  float4 emissiveSample = emissiveMap.Sample(samState, input.TexCoord);
  float4 emissBlurSample = emissiveBlurMap.Sample(samState, input.TexCoord);
  float ssaoSample = ssaoMap.Sample(samState, input.TexCoord).r;
  
  // float4 diffuse = albedoSample * shdwSpecSample.r;
  // get the base color 
  float3 color = albedoSample.rgb;
  
  color = color / (color + float3(1.0f.xxx));
  float3 IBL = IBLSample.rgb;
  
  // color = IBL * shdwSpecSample.g * albedoSample.rgb;
  //  * ssaoSample
  color = (shdwSpecSample.g * IBL) + (albedoSample.rgb * shdwSpecSample.r);
  
  // check for a skybox position.
  
  // transparency
  float alpha = albedoSample.a;
  float4 skyboxFinal = float4(skyboxSample.rgb, 1.0f);
  
  // temporary placeholder for the issue above.
  if (alpha == 0.0f) {
    color = skyboxFinal.rgb;
  }
  
  float4 fullEmissive = emissiveSample;
  
  return float4(color, 1.0f) + fullEmissive;
}