// Resources
Texture2D albedoMap : register(t0);
Texture2D specularMap: register(t1);
Texture2D shadowMap : register(t2);
Texture2D skyboxMap : register(t3);
Texture2D IBRMap : register(t4);
Texture2D emissiveMap : register(t5);
Texture2D emissiveBlurMap : register(t6);
Texture2D metallicMap : register(t7);

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
  float4 shadowSample = shadowMap.Sample(samState, input.TexCoord);
  float4 specularSample = specularMap.Sample(samState, input.TexCoord);
  float4 skyboxSample = skyboxMap.Sample(samState, input.TexCoord);
  float4 IBRSample = IBRMap.Sample(samState, input.TexCoord);
  float4 emissiveSample = emissiveMap.Sample(samState, input.TexCoord);
  float4 emissBlurSample = emissiveBlurMap.Sample(samState, input.TexCoord);
  float metallicValue = metallicMap.Sample(samState, input.TexCoord).b;
  
  // metallic logic: if fully metallic, none of the original color will show,
  //                 if not metallic at all, all of the color shous through.
  
  // this will give us the color of the metallic reflection
  float4 metallicColor = albedoSample * (1 - metallicValue);
  float4 IBRMetallic = IBRSample * specularSample;
  
  float4 diffuse = albedoSample * shadowSample;
  
  diffuse += IBRMetallic;
  
  // check for a skybox position.
  
  // transparency
  float alpha = albedoSample.a;
  float4 skyboxFinal = float4(skyboxSample.rgb * (1.0f- alpha), 1.0f);
  /**
   * why this doesnt work i have no idea, i've tested diffuse and skybox final separately
     and they both work just fine, but when mixing them both, only the skybox turns black.
   */
  diffuse = diffuse * alpha + skyboxFinal;
  
  // temporary placeholder for the issue above.
  if (alpha == 0.0f) {
    diffuse = skyboxFinal;
  }
  
  float4 fullEmissive = emissiveSample + emissBlurSample;

  return diffuse + fullEmissive;
}