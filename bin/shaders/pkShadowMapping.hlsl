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
Texture2D brdfLUT : register(t7);
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

cbuffer LightViewProj : register(b2)
{
  float4x4 LightViewProj; // 64
}

cbuffer ShadowParam : register(b3)
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
  float4 diffuseBRDF : SV_Target0;
  float4 specularBRDF : SV_Target1;
};

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
  float4 normalTex = normalMap.Sample(samState, input.TexCoord);
  float3 colorTex = colorMap.Sample(samState, input.TexCoord).rgb;
  float ao = orm.Sample(samState, input.TexCoord).r;
  float metallic = orm.Sample(samState, input.TexCoord).b;
  float roughness = orm.Sample(samState, input.TexCoord).g;
  float3 worldPos = positionsMap.Sample(samState, input.TexCoord).xyz;
  
  /**
   * alpha values
   */
  // if its the max depth value
  float alpha = 1.0f;
  if (depthTex.r == 1) {
    alpha = 0.0f;
  }
  
  float3 viewDir = normalize(Eye.xyz - worldPos);
  float3 normal = normalize(normalTex.xyz);
  // float NoV = saturate(dot(normal, viewDir));
  // float2 brdfSample = brdfLUT.Sample(samState, float2(NoV, roughness)).rg;
  
  // Diffuse BRDF
  float shadowColor = 1.0f - ShadowIntensity;
  float3 lightDir = normalize(mul(float4(-LightDir, 0.0f), lightTransform).xyz);
  float orenNaya = OrenNayarDiffuse(normal, lightDir, viewDir, roughness);
  
  float dotOfLight = orenNaya; // dot(lightDir, normal); // 
  float lamb = max(dotOfLight, shadowColor);
  float3 diffuseBRDF = lerp(lamb, shadowColor, 1.0f - lamb);
  
  // Compute F0
  float3 F0 = lerp(float3(0.04f, 0.04f, 0.04f), colorTex, metallic.xxx);

  // fresnel value
  float3 Half = normalize(viewDir + lightDir);
  float VoH = saturate(dot(viewDir, Half));
  float3 F = Fresnel(F0, VoH);

  // Diffuse energy weight
  float3 kD = (1.0f - F) * (1.0f - metallic);
  
  float3 specCookTorrance = cookTorranceSpecular(normal,
                                                 viewDir,
                                                 lightDir,
                                                 roughness,
                                                 metallic,
                                                 F0);
  
  float3 specular = (specCookTorrance) * SpecIntensity * ao;
  
  output.diffuseBRDF = float4(diffuseBRDF * kD, alpha);
  output.specularBRDF = float4(specular, alpha);
  
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
    // output.diffuseBRDF = float4(diffuseBRDF * kD * shadowColor.xxx, alpha);
  }
  
  /**
   * Works, but its not correct
   */
  // float3 worldToLight = normalize(worldPos - lightPos);
  // float angle = dot(-lightDir, worldToLight);
  // if (angle < SpotCutoff)
  // {
  //   output.shdwSpec = float4(shadowColor, 0.0f, 0.0f, 1.0f);
  //   return output;
  // }
  
  return output;
}
