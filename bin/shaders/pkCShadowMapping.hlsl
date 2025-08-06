/***************************************************************************************
* Shadows Compute shader
***************************************************************************************/

#pragma kernel CS_Main

// unordered texture for reading/writing.
RWTexture2D<float4>shadowTexture : register(u0);

#define PI 3.14159265359

// resources
Texture2D<float4> normalMap : register(t0);
Texture2D<float4> metallicMap : register(t1);
Texture2D<float4> shadowMap : register(t2);
Texture2D<float4> depthMap : register(t3);
Texture2D<float4> positionsMap : register(t4);
// sampler state
SamplerState samState : register(s0);

/*******************************************/
/*         CONSTANT BUFEFRS                */
/*******************************************/

cbuffer cbLight : register(b0)
{
  float3 LightDir; // 12
  float lightPadd1; // 16
  float3 LightPos; // 28
  float lightPadd2; // 32
  float3 LightColor; // 44
  float lightPadd3; // 48
  float ShadowIntensity; // 52
  float SpotExponent; // 56
  float SpotCutoff; // 60
  float SpecIntensity; // 64
  float4x4 lightTransform; // 128
}

cbuffer Camera : register(b1)
{
  float4 Eye; // 16
  float3 ForwardCam; // 28
  float4x4 ViewCam; // 92
  float4x4 ProjectionCam; // 156
  float _unusedCam0; // 160
}

cbuffer LightCamera : register(b2)
{
  float4 EyeLight; // 16
  float3 ForwardLight; // 28
  float4x4 ViewLight; // 92
  float4x4 ProjectionLight; // 156
  float _unusedLightCam0; // 160
  float2 cbLightCamPadd1;
}

cbuffer CamInvProj : register(b3)
{
  float4x4 camInvProj; // 64
}

cbuffer CamInvView : register(b4)
{
  float4x4 camInvView; // 64
}

cbuffer ShadowParam : register(b5)
{
  float2 winSize; // 8
  float2 farNear; // 16
}

// write directly onto the texture.
[numthreads(16, 16, 1)]
void CSMain(uint3 DTid : SV_DispatchThreadID)
{
  /**
   * light data
   */
  float3 lightColor = LightColor.xyz;
  float3 lightPos = LightPos.xyz;
  
  /**
   * texture data
   */
  float4 depthTex = depthMap.Load(int3(DTid.xy, 0));
  float4 normalTex = normalMap.Load(int3(DTid.xy, 0));
  float4 metallicTex = metallicMap.Load(int3(DTid.xy, 0));
  float3 worldPos = positionsMap.Load(int3(DTid.xy, 0)).xyz;
  
  /**
   * alpha values
   */
  // if its the max depth value
  float alpha = 1.0f;
  if (depthTex.r == 1) {
    alpha = 0.0f;
  }
  
  float3 normal = normalize(normalTex.xyz);
  
  // diffuse
  float shadowColor = 1.0f - ShadowIntensity;
  float3 lightDir = normalize(mul(float4(-LightDir, 0.0f), lightTransform).xyz);
  
  float diff = max(dot(lightDir, normal), shadowColor);
  diff = lerp(diff, shadowColor, 1.0f - diff);
  float3 diffuse = lightColor * diff;
  
  shadowTexture[DTid.xy] = float4(diffuse, alpha);
}