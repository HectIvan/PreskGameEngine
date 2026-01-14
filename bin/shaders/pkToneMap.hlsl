// Resources
Texture2D baseTexture : register(t0);
Texture2D luminance : register(t1);
Texture2D emissive : register(t2);
Texture2D emissiveBlur : register(t3);
Texture2D skybox : register(t4);
Texture2D depth : register(t5);
Texture2D transpDepth : register(t6);
Texture2D transpAlbedo : register(t7);

// sampler
SamplerState samState : register(s0);

cbuffer Exposure : register(b0)
{
  float exposure;
  float3 _expUnused;
}

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
  float4 emissiveBlurSample = emissiveBlur.Sample(samState, input.TexCoord);
  float4 emissiveSample = emissive.Sample(samState, input.TexCoord);
  float4 skyboxSample = skybox.Sample(samState, input.TexCoord);
  float depthSample = depth.Sample(samState, input.TexCoord).r;
  
  float transpDepthSample = transpDepth.Sample(samState, input.TexCoord).r;
  float4 transpAlbedoSample = transpAlbedo.Sample(samState, input.TexCoord);
  
  finalColor = baseSample;
  
  if (depthSample >= 1.0f) {
    finalColor = skyboxSample;
  }
  
  finalColor += lumBlurSample + emissiveSample + emissiveBlurSample;
  
  // transparency blending
  if (depthSample > transpDepthSample) {
    finalColor = lerp(finalColor, transpAlbedoSample, transpAlbedoSample.a);
  }
  
  const float gamma = 2.2f;
  // exposure tone mapping
  float3 mapped = float3(1.0f.xxx) - exp(-finalColor.rgb * exposure);
  // gamma correction 
  mapped = pow(abs(mapped), float3((1.0f / gamma).xxx));
  
  finalColor = float4(mapped, 1.0f);

  return finalColor;
}