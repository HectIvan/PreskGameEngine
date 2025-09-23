/***************************************************************************************
* Shadows Pixel shader
***************************************************************************************/

#define PI 3.14159265359

// resources depth textures
Texture2D shadowMap : register(t0);
Texture2D depthMap : register(t1);
// resources textures
Texture2D normalMap : register(t2);
Texture2D colorMap : register(t3);
Texture2D positionsMap : register(t4);
Texture2D orm : register(t5);
Texture2D lightPosMap : register(t6);
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
}

cbuffer LightViewProj : register(b3)
{
  float4x4 LightViewProj; // 64
}

cbuffer ShadowParam : register(b4)
{
  float2 winSize; // 8
  float2 farNear; // 16
}

struct PS_INPUT
{
  float4 Position : SV_Position;
  float2 TexCoord : TEXCOORD0;
};

struct PS_OUTPUT
{
  float3 shdwSpec : VS_Target0;
};

float fresnelSchlick(float F0, float VoH)
{
  return F0 + (1.0f - F0) * pow((1.0f - VoH), 5.0f);
}

float fresnelForLut(float VoH)
{
  return pow(1.0f - VoH, 5.0f);
}

float GeometrySchlickGGX(float NoV, float roughness)
{
  // float k = (roughness * roughness) * 0.5f;
  // float denom = NoV * (1.0f - k) + k;
  // return NoV / denom;
  
  float r = roughness + 1.0f;
  float k = (r * r) / 8.0f;
  float G =  NoV * (1.0f - k) + k;
  return NoV / G;
  
  // float nom = NoV;
  // float denom = NoV * (1.0 - roughness) + roughness;
  // float G = nom / denom;
  // return G;
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

float D_Beckmann(float NoH, float alpha)
{
  float cos2 = NoH * NoH;
  float tan2 = (1.0f - cos2) / (cos2 + 1e-5f);
  return exp(-tan2 / alpha) / (PI * alpha * cos2 * cos2 + 1e-5f);
}

// source: https://graphicscompendium.com/gamedev/15-pbr
// min of 1 | 2(HoN)(NoV) / (VoH) | 2(HoN)(NoL) / (VoL)
float GeometricAttenuation(float3 halfView, float3 normal, float3 view, float3 lightDir)
{
  float HoN = saturate(dot(halfView, normal));
  float NoV = saturate(dot(normal, view));
  float VoH = saturate(dot(view, normal));
  float NoL = saturate(dot(normal, lightDir));
  float VoL = saturate(dot(view, lightDir));
  
  float first = 2.0f * (HoN * NoV) / VoH;
  float second = 2.0f * (HoN * NoL) / VoL;
  float G = min(min(1, first), second);
  
  return G;
}

// source: https://graphicscompendium.com/gamedev/15-pbr
// Dblinn = (1/PI alpha^2)(HoN^(2/alpha^2 - 2))
float NormalDistribution(float3 halfView, float3 normal, float roughness)
{
  // float a = roughness * roughness;
  // float a2 = a * a;
  // float HoN = saturate(dot(halfView, normal));
  // 
  // float first = 1.0f / (PI * a2);
  // float expVal = (2.0f / a2) - 2.0f;
  // float exponent = pow(HoN, expVal);
  // float D = first * exponent;
  // return D;
  
  float a = roughness * roughness;
  float a2 = a * a;
  float NoH = max(dot(normal, halfView), 0.0f);
  float NoH2 = NoH * NoH;

  float denom = NoH2 * (a2 - 1.0f) + 1.0f;
  denom = PI * denom * denom;
  
  return a2 / denom;
}

// source: https://graphicscompendium.com/gamedev/15-pbr
// F0 = (n - 1)^2 / (n + 1)^2
// F = F0 + (1 - F0) * (1 - VoH)^5
float Fresnel(float3 F0, float VoH) // float refraction
{
  // float F0 = pow((refraction - 1), 2.0f) / pow((refraction + 1), 2.0f);
  float F = F0 + (1.0f - F0) * pow((1 - VoH), 5.0f);
  return F;
}

float3 cookTorranceSpecular(float3 normal,
                            float3 viewDir,
                            float3 lightDir,
                            float rough,
                            float metallic,
                            float3 F0)
{
  float3 lo = 0.0f.xxx;
  
  float3 Half = normalize(viewDir + lightDir);
  float NoL = saturate(dot(normal, lightDir));
  float NoV = saturate(dot(normal, viewDir));
  float NoH = saturate(dot(normal, Half));
  float VoH = saturate(dot(viewDir, Half));
  
  float alpha = rough * rough;
  float3 radiance = SpecIntensity.xxx;
  
  // float G = GeometrySmith(NoV, NoL, rough);
  // float D = NormalDistribution(Half, normal, rough);
  // float F = Fresnel(F0, VoH);
  
  // float den = max(4.0f * NoV * NoL, 1e-5f);
  // 
  // float3 specular = (NDF * G * F) / den;
  
  float NDF = NormalDistribution(Half, normal, rough);
  float G = GeometrySmith(NoV, NoL, rough);
  float F = Fresnel(F0, max(VoH, 0.0f));
  
  float3 kD = float3(1.0f, 1.0f, 1.0f) - F;
  kD *= 1.0f - metallic;
  
  float3 numerator = NDF * G * F;
  float denominator = 4.0f * max(NoV, 0.0f) * max(NoL, 0.0f) + 1e-5f;
  float3 specular = numerator / denominator;
  
  lo += ((kD * 1.0f / PI + specular) * radiance * NoL).xxx;
  
  return float4(lo, 1.0);
}

float magnitude(float3 v)
{
  return sqrt((v.x * v.x) + (v.y * v.y) * (v.z * v.z));
}

PS_OUTPUT PS(PS_INPUT input) : SV_Target0
{
  PS_OUTPUT output = (PS_OUTPUT) 0;
  
  /**
   * light data
   */
  float3 lightColor = LightColor.xyz;
  float3 lightPos = LightPos.xyz;
  
  /**
   * texture data
   */
  float4 depthTex = depthMap.Sample(samState, input.TexCoord);
  float4 normalTex = normalMap.Sample(samState, input.TexCoord);
  float4 colorTex = colorMap.Sample(samState, input.TexCoord);
  float ambientOcclusion = orm.Sample(samState, input.TexCoord).r;
  float metallicVal = orm.Sample(samState, input.TexCoord).b;
  float roughVal = orm.Sample(samState, input.TexCoord).g;
  float3 worldPos = positionsMap.Sample(samState, input.TexCoord);
  
  /**
   * alpha values
   */
  // if its the max depth value
  float alpha = 1.0f;
  if (depthTex.r == 1) {
    alpha = 0.0f;
  }
  
  float3 normal = normalize(normalTex.xyz);
  
  // diffuse light
  float shadowColor = 1.0f - ShadowIntensity;
  float3 lightDir = normalize(mul(float4(-LightDir, 0.0f), lightTransform).xyz);
  
  float dotOfLight = dot(lightDir, normal);
  float lamb = max(dotOfLight, shadowColor);
  lamb = lerp(lamb, shadowColor, 1.0f - lamb);
  float3 lambert = lightColor * lamb;
  
  lightDir = normalize(mul(float4(-LightDir, 0.0f), lightTransform).xyz);
  
  /**
   * Works, but its not correct
   */
  //      float3 worldToLight = normalize(worldPos - lightPos);
  //      float angle = dot(-lightDir, worldToLight);
  //      if (angle < SpotCutoff)
  //      {
  //        output.shadow = float4(shadowColor.xxx, 1);
  //        output.specular = float4(0, 0, 0, 1);
  //        return output;
  //      }
  
  // specular light
  float3 viewDir = normalize(Eye.xyz - worldPos);
  
  float3 F0 = float3(0.04f.xxx);
  F0 = lerp(F0, colorTex.rgb, metallicVal.xxx);
  
  float3 specCookTorrance = cookTorranceSpecular(normal,
                                                 viewDir,
                                                 lightDir,
                                                 roughVal,
                                                 metallicVal,
                                                 F0);
  
  float3 specular = (specCookTorrance) * SpecIntensity * ambientOcclusion;
  
  output.shdwSpec = float3(lambert.r, specular.r, alpha);
  
  /**
   * shadow mapping;
   */
  float4 lightSpacePos = mul(float4(worldPos, 1.0f), LightViewProj);
  float3 lightNDC = lightSpacePos.xyz / lightSpacePos.w;
  float2 lightUV = lightNDC.xy * 0.5f + 0.5f;
  lightUV.y = -lightUV.y;
  
  float3 lightWorldPos = lightPosMap.Sample(samState, lightUV).xyz;
  
  float lightHit = magnitude(worldPos - LightPos);
  float worldHit = magnitude(lightWorldPos - LightPos);
  
  float tolerance = 2.0f * dotOfLight;
  
  if (lightHit > worldHit + tolerance) {
    output.shdwSpec = float3(shadowColor, specular.r, 1.0f);
  }
  
  return output;
}