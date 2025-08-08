/***************************************************************************************
* Constant Buffer Structures
***************************************************************************************/

#pragma kernel CS_Main

// unordered texture for reading/writing.
RWTexture2D<float4> specTexture : register(u0);

// resources
Texture2D<float4> normalMap : register(t0);
Texture2D<float4> metallicMap : register(t1);
Texture2D<float4> depthMap : register(t2);
Texture2D<float4> positionsMap : register(t3);
// sampler state
SamplerState samState : register(s0);

#define PI 3.14159265359

/*******************************************/
/*      CONSTANT BUFEFRS                   */
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

float fresnelSchlick(float refractionIndex, float3 lightVec, float3 normal)
{
  float helperFunct = pow(1 - dot(lightVec, normal), 5);
  float pSchlick = (refractionIndex + (1 - refractionIndex)) * helperFunct;
  return pSchlick;
}


float GeometrySchlickGGX(float NoV, float roughness)
{
  float k = (roughness * roughness) * 0.5f;
  float denom = NoV * (1.0f - k) + k;
  return NoV / denom;
}

float GeometrySmith(float NoV, float NoL, float roughness)
{
  float ggx2 = GeometrySchlickGGX(NoV, roughness);
  float ggx1 = GeometrySchlickGGX(NoL, roughness);
  
  return ggx1 * ggx2;
}

float ndf_GGX(float NoH, float alpha)
{
  float a = NoH * alpha;
  float k = alpha / (1.0f - NoH * NoH + a * a);
  return k * k * PI;  
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
  
  float3 normal = normalize(normalTex.xyz);
  
  // get world position
  float3 worldPos = positionsMap.Load(int3(DTid.xy, 0));

  // specular
  float spec = 0.0;
  float3 lightDir = normalize(mul(float4(-LightDir, 0.0f), lightTransform).xyz);
  // if the angle between the world to light vector and light direction is greater than the tolerance
  float angle = dot(lightDir, LightPos - worldPos);
  
  float3 viewDir = normalize(Eye.xyz - worldPos);
  float3 halfwayDir = normalize(lightDir + viewDir);
  spec = pow(max(dot(normal, halfwayDir), 0.0f), SpotExponent);
  
  //      float NoV = saturate(dot(normal, ))
  // Fresnel Schlic specular calculation
  float lightVecFromWorld = normalize(LightPos - worldPos);
  float FS = fresnelSchlick(0.3f, lightVecFromWorld, normal);
  // float D = ndf_GGX()
  float3 specular = (lightColor * (spec * SpecIntensity));
  
  // set to opaque
  float alpha = 1.0f;

  // if its the max depth value
  if (depthTex.r == 1) 
  {
    alpha = 0.0f;
  }
  
  specTexture[DTid.xy] = float4(specular, alpha);
}