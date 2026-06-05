/***************************************************************************************
* PBR Pixel shader
***************************************************************************************/

#define PI 3.14159265359f
#define SMALL_NUMBER 1.0f

// resources depth textures
Texture2D depthMap : register(t0); // basic R32 float depth
Texture2D posMap : register(t1); // world positions RGB32 float
Texture2D lightPosMap : register(t2); // light world positions RGB32 float
// resources textures
Texture2D albedoMap : register(t3); // albedo
Texture2D normalMap : register(t4); // normals
Texture2D ormMap : register(t5); // occlusion (R), roughness (G), metallic (B)
Texture2D skybox : register(t6); // skybox texture
Texture2D cubeMap : register(t7); // cube map texture
TextureCube irradianceMap : register(t8); // irradiance map texture
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
  float4x4 LightViewProj; // 64
}

cbuffer Camera : register(b1)
{
  float4 Eye; // 16
  float3 ForwardCam; // 28
  float4x4 ViewCam; // 92
  float4x4 ProjectionCam; // 156
  float _unusedCam0; // 160
}

struct PS_INPUT
{
  float4 Position : SV_Position;
  float2 TexCoord : TEXCOORD0;
};

struct PS_OUTPUT
{
  float4 objResult : SV_Target0; // object result
};

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
  return F0 + (1.0f - F0) * pow((1 - VoH), 5.0f);
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

float magnitude(float3 v)
{
  return sqrt((v.x * v.x) + (v.y * v.y) * (v.z * v.z));
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

PS_OUTPUT PS(PS_INPUT input)
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
  float3 normalTex = normalMap.Sample(samState, input.TexCoord).xyz;
  float3 normal = normalize(normalTex);
  float4 albedo = albedoMap.Sample(samState, input.TexCoord);
  
  float3 ormValues = ormMap.Sample(samState, input.TexCoord).rgb;
  float ao = ormValues.r;
  float roughness = ormValues.g;
  float metallic = ormValues.b;
  float3 worldPos = posMap.Sample(samState, input.TexCoord).xyz;
  
  float3 viewDir = normalize(worldPos - Eye.xyz);
  float3 lightDir = normalize(mul(float4(-LightDir, 0.0f), lightTransform).xyz);
  float3 F0 = lerp(float3(0.04f, 0.04f, 0.04f), albedo.rgb, float3(metallic, metallic, metallic));
  float3 Half = normalize(viewDir + lightDir);
  float VoH = saturate(dot(viewDir, Half));
  float NoV = saturate(dot(normal, viewDir));
  float3 F = Fresnel(F0, max(VoH, 0.0f));

  // Diffuse BRDF
  float shadowColor = 1.0f - ShadowIntensity;
  float orenNaya = OrenNayarDiffuse(normal, lightDir, viewDir, roughness);
  
  // Diffuse energy weight
  float3 kS = Fresnel(F0, VoH);
  float3 kD = (1.0f - kS) * (1.0f - metallic);
  
  float3 specCookTorrance = cookTorranceSpecular(normal,
                                                 viewDir,
                                                 lightDir,
                                                 roughness,
                                                 metallic,
                                                 F0);
  
  float3 fLambert = albedo.rgb / PI;
  float3 specularBRDF = (specCookTorrance * LightColor);
  float3 diffuseBRDF = orenNaya * fLambert * kD;
  
  /**
   * IBL calculations
   */
  
  float3 view = reflect(-viewDir, normal);
  
  // get the skybox sample depending on roughness levels.
  float2 dimensions = 0.0f.xx;
  skybox.GetDimensions(dimensions.x, dimensions.y);
  
  float mipCount = log2(max(dimensions.x, dimensions.y)) + 1;
  float targetMip = lerp(0.0, mipCount - 1.0, roughness);
  float2 skyboxUV = getSkyBoxUV(view);
  float3 reflective = skybox.SampleLevel(samState, skyboxUV, targetMip).rgb;
  float3 irradiance = irradianceMap.Sample(samState, normal).rgb;
  
  // diffuse ibl should use the irradiance map, and use the normal vector, not the reflected vector.
  float3 diffuseIBL = irradiance * albedo.rgb * kD;
  float3 specularIBL = reflective * F;

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
  
  float3 finalColor = diffuseBRDF + specularBRDF + diffuseIBL + specularIBL;
  
  // float NoL = max(dot(normal, lightDir), 0.0f);
  // float bias = SMALL_NUMBER * tan(acos(NoL));
  if (lightHit > worldHit + SMALL_NUMBER) {
    // finalColor *= shadowColor.xxx;
  }
  
  output.objResult = float4(finalColor, albedo.a);
  
  return output;
}
