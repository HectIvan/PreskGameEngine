/***************************************************************************************
* Light Compute shader
***************************************************************************************/
// #pragma CS_Main

#define PI 3.14159265359f
#define SMALL_NUMBER 1.0f

RWTexture2D<float4> brdfTexture : register(u0);

// resources depth textures
Texture2D<float4> depthMap : register(t0); // basic R32 float depth
Texture2D<float4> depthLightMap : register(t1); // basic R32 float depth
Texture2D<float4> posMap : register(t2); // world positions RGB32 float
Texture2D<float4> lightPosMap : register(t3); // light world positions RGB32 float
// resources textures
Texture2D<float4> albedoMap : register(t4); // albedo
Texture2D<float4> normalMap : register(t5); // normals
Texture2D<float4> ormMap : register(t6); // occlusion (R), roughness (G), metallic (B)
Texture2D<float4> skybox : register(t7); // skybox texture
Texture2D<float4> cubeMap : register(t8); // skybox texture
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
  row_major float4x4 lightTransform; // 128
}

cbuffer Camera : register(b1)
{
  float4 Eye; // 16
  float3 ForwardCam; // 28
  row_major float4x4 ViewCam; // 92
  row_major float4x4 ProjectionCam; // 156
  float _unusedCam0; // 160
}

cbuffer LightViewProj : register(b2)
{
  row_major float4x4 LightViewProj; // 64
}

cbuffer ShadowParam : register(b3)
{
  float2 winSize; // 8
  float2 farNear; // 16
}

cbuffer IBLIntensity : register(b4)
{
  float IBLIntensity;
  float3 _unusedIBL;
}

float2 getSkyBoxUV(float3 dir)
{
  // https://en.wikipedia.org/wiki/UV_mapping#Finding_UV_on_a_sphere
  float u = 0.5f + atan2(-dir.z, dir.x) / (PI * 2.0f);
  float v = 0.5f - asin(dir.y) / PI;
  return float2(u, v);
}

float fresnelForLut(float VoH)
{
  return pow(1.0f - VoH, 5.0f);
}

float GeometrySchlickGGX(float NoV, float roughness)
{
  float r = roughness + 1.0f;
  float k = (r * r) / 8.0f;
  float G =  NoV * (1.0f - k) + k;
  return NoV / G;
}

float GeometrySmith(float NoV, float NoL, float roughness)
{
  float ggx2 = GeometrySchlickGGX(NoV, roughness);
  float ggx1 = GeometrySchlickGGX(NoL, roughness);
  
  return ggx1 * ggx2;
}

// source: https://graphicscompendium.com/gamedev/15-pbr
// Dblinn = (1/PI alpha^2)(HoN^(2/alpha^2 - 2))
float NormalDistribution(float3 halfView, float3 normal, float roughness)
{
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
float3 Fresnel(float3 F0, float VoH) // float refraction
{
  float3 F = F0 + (1.0f - F0) * pow((1 - VoH), 5.0f);
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
  
  float NDF = NormalDistribution(Half, normal, rough);
  float G = GeometrySmith(NoV, NoL, rough);
  float3 F = Fresnel(F0, max(VoH, 0.0f));
  
  float3 numerator = NDF * G * F;
  float denominator = 4.0f * max(NoV, 0.0f) * max(NoL, 0.0f) + 1e-5f;
  float3 specular = numerator / denominator;
  
  lo += (specular * radiance * NoL);
  
  return lo;
}

float OrenNayarDiffuse(float3 N, float3 L, float3 V, float roughness)
{
  float NoL = saturate(dot(N, L));
  float NoV = saturate(dot(N, V));
  
  float thetaR = acos(NoV);
  float thetaI = acos(NoL);
  
  float3 NVNoV = normalize(N - V * NoV);
  float3 NLNoL = normalize(N - L * NoL);
  
  float cosPhiDiff = saturate(dot(NVNoV, NLNoL));
  
  float alpha = max(thetaI, thetaR);
  float beta = min(thetaI, thetaR);
  
  float r2 = roughness * roughness;
  
  float A = 1.0f - 0.5f * r2 / (r2 + 0.33f);
  float B = 0.45f * r2 / (r2 + 0.09f);

  return saturate(NoL * (A + B * sin(alpha) * tan(beta)));
}

[numthreads(16, 16, 1)]
void CSMain(uint3 DTid : SV_DispatchThreadID)
{
  uint width, height;
  brdfTexture.GetDimensions(width, height);

  if (DTid.x >= width || DTid.y >= height) {
    return;
  }
  
  /**
   * light data
   */
  float3 lightColor = LightColor.xyz;
  float3 lightPos = LightPos.xyz;
  
  /**
   * texture data
   */
  float4 depthTex = depthMap.Load(int3(DTid.xy, 0));
  float4 depthLightTex = depthLightMap.Load(int3(DTid.xy, 0));
  float4 normalTex = normalMap.Load(int3(DTid.xy, 0));
  float4 albedo = albedoMap.Load(int3(DTid.xy, 0));
  
  // check if the alpha is below threshold
  if (albedo.a <= 0.05f) {
    return;
  }
  
  float3 ormValues = ormMap.Load(int3(DTid.xy, 0)).rgb;
  float ao = ormValues.r;
  float metallic = ormValues.b;
  float roughness = ormValues.g;
  float3 worldPos = posMap.Load(int3(DTid.xy, 0)).xyz;
  
  float3 viewDir = normalize(worldPos - Eye.xyz);
  float3 lightDir = normalize(mul(float4(-LightDir, 0.0f), lightTransform).xyz);
  float3 normal = normalize(normalTex.xyz * 2.0f - 1.0f);
  float3 F0 = lerp(float3(0.04f, 0.04f, 0.04f), albedo.rgb, metallic.rrr);
  float3 Half = normalize(viewDir + lightDir);
  float VoH = saturate(dot(viewDir, Half));
  float3 F = Fresnel(F0, max(VoH, 0.0f));

  // Diffuse BRDF
  float shadowColor = 1.0f - ShadowIntensity;
  float orenNaya = OrenNayarDiffuse(normal, lightDir, viewDir, roughness);
  
  // Diffuse energy weight
  float3 kD = (1.0f - F) * (1.0f - metallic);
  float3 fLambert = albedo.rgb / PI;
  
  float3 specCookTorrance = cookTorranceSpecular(normal,
                                                 viewDir,
                                                 lightDir,
                                                 roughness,
                                                 metallic,
                                                 F0);
  
  float3 specularBRDF = (specCookTorrance * LightColor);
  float3 diffuseBRDF = orenNaya * fLambert * kD;
  
  /**
   * IBL calculations
   */
  
  float3 V = normalize(worldPos - Eye.xyz);
  float3 view = reflect(V, normal);
  view.z *= -1.0f;
  float2 skyboxUV = getSkyBoxUV(view);
  
  // get the skybox sample depending on roughness levels.
  float2 dimensions = 0.0f.xx;
  skybox.GetDimensions(dimensions.x, dimensions.y);
  float mipCount = log2(max(dimensions.x, dimensions.y)) + 1;
  float targetMip = lerp(0.0, mipCount - 1.0, roughness);
  // specular IBL
  float3 IBL = skybox.SampleLevel(samState, skyboxUV, targetMip).rgb * IBLIntensity;
  
  float3 diffuseIBL = IBL * fLambert;
  
  float3 finalColor = diffuseBRDF + specularBRDF + diffuseIBL;
  /**
  * shadow mapping;
  */
  float4 lightSpacePos = mul(float4(worldPos, 1.0f), LightViewProj);
  float3 lightNDC = lightSpacePos.xyz / lightSpacePos.w;
  float2 lightUV = lightNDC.xy * 0.5f + 0.5f;
  
  float currentDepth = lightNDC.z;
  float shadowDepth = lightPosMap.SampleLevel(samState, lightUV, 0).r;
  
  float bias = 0.001f;
  
  if (lightNDC.z < -1 || lightNDC.z > 1) {
    // brdfTexture[DTid.xy] = float4(finalColor, albedo.a);
    // return;
  }

  if (currentDepth > shadowDepth + bias) {
    finalColor *= shadowColor.xxx;
  }
  
  brdfTexture[DTid.xy] = float4(finalColor * ao, albedo.a);
}
