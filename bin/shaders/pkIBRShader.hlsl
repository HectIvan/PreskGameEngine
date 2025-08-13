Texture2D skyboxMap : register(t0);
Texture2D normalMap : register(t1);
Texture2D posMap : register(t2);
Texture2D metallicMap : register(t3);
Texture2D roughnessMap : register(t4);
Texture2D colorMap : register(t5);

SamplerState samState : register(s0);

#define PI 3.14159265f

cbuffer Intensity : register(b0)
{
  float Intensity;
  float3 _unused;
}

cbuffer ViewPos : register(b1)
{
  float3 ViewPosition;
  float _unusedView;
}

struct PS_INPUT
{
  float4 Position : SV_Position;
  float2 TexCoord : TEXCOORD0;
};

float2 getSkyBoxUV(float3 dir)
{
  // https://en.wikipedia.org/wiki/UV_mapping#Finding_UV_on_a_sphere
  float u = 0.5f + atan2(-dir.z, dir.x) / (PI * 2.0f);
  float v = 0.5f - asin(dir.y) / PI;
  return float2(u, v);
}

float4
sRGBToLinear(float4 color)
{
  // Convert sRGB to linear color space
  return float4(pow(color.rgb, 1.0f / 2.2f), color.a);
}

// METALLIC
// From my understanding, metalness affects how much of the base color will seep through the reflection,
// meaning that if the metallic value is 0, 100% of the base color will show through, while if its 1, 0%
// of the base color shows through, meaning only reflection.

// ROUGHNESS
// On the other hand, as far as i know, roughness affects how blurry the reflection is, meaning that
// if roughness is 0, aka, smooth as hell, the reflection will be 1:1, while if it is 1, aka, fully rough,
// the reflection will be the blurriest possible, since the surface is scattering the light in all directions.

float4 PS(PS_INPUT input) : SV_Target
{
  // get the world normals
  float4 normalTex = normalMap.Sample(samState, input.TexCoord);
  float4 position = posMap.Sample(samState, input.TexCoord);
  float metallicTex = metallicMap.Sample(samState, input.TexCoord).b;
  float roughSample = roughnessMap.Sample(samState, input.TexCoord).g;
  float3 colorTex = colorMap.Sample(samState, input.TexCoord).rgb;
  
  // normalize normals and view
  float3 N = normalize(normalTex.xyz);
  float3 V = normalize(position.xyz - ViewPosition);
  
  // reflect the view direction on the normal to get the view reflected
  float3 view = reflect(V, N);
  
  // sample the skybox with the new direction
  float2 skyboxUV = getSkyBoxUV(view);
  
  // get the skybox dimensions
  float2 dimensions = 0.0f.xx;
  skyboxMap.GetDimensions(dimensions.x, dimensions.y);
  // get the ammount of mip levels in the skybox.
  float mipCount = log2(max(dimensions.x, dimensions.y)) + 1;
  // get the target mip level based on the roughness value.
  float targetMip = lerp(0.0, mipCount - 1.0, roughSample);
  // sample the skybox with the target mip level.
  float3 IBL = skyboxMap.SampleLevel(samState, skyboxUV, targetMip).rgb;
  
  // get how much of the surounding light will be reflected.
  float3 metallicColor = sRGBToLinear(float4(IBL, 1.0f)).rgb * metallicTex;
  
  return float4(metallicColor * Intensity, 1.0f);
}