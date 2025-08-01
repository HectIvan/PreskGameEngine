// Resources
Texture2D albedoTex : register(t0);
Texture2D luminanceTex : register(t1);
Texture2D shadowTex : register(t2);
Texture2D specularTex : register(t3);
Texture2D specularBlurTex : register(t4);
Texture2D skyboxTex : register(t5);

// sampler
SamplerState samState : register(s0);

struct PS_INPUT
{
    float4 Position : SV_Position;
    float2 TexCoord : TEXCOORD0;
};  

float4 PS(PS_INPUT input) : SV_Target0
{
  // Get texture resources
  float4 albedoSample = albedoTex.Sample(samState, input.TexCoord);
  float4 luminanceSample = luminanceTex.Sample(samState, input.TexCoord);
  float4 shadowSample = shadowTex.Sample(samState, input.TexCoord);
  float4 specularSample = specularTex.Sample(samState, input.TexCoord);
  float4 specBlurSample = specularBlurTex.Sample(samState, input.TexCoord);
  float4 skyboxSample = skyboxTex.Sample(samState, input.TexCoord);
  
  // base specular
  float4 specular = (specularSample * albedoSample) * shadowSample;
  // blured specular
  float4 specularBlur = (specBlurSample * albedoSample) * shadowSample;
  // mix shadows, color and both specular and blured specular
  float4 finalColor = (albedoSample * shadowSample) + specular + specularBlur;
  
  // check for a skybox position
  if (albedoSample.a == 0)
  {
    finalColor = float4(skyboxSample.rgb, 1.0f);
  }
  // color += (1.0f - color.a) * float4(skyboxSample.rgb, 1.0f);

  return finalColor;
}