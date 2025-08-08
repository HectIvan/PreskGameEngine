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
Texture2D metallicMap : register(t5);
Texture2D roughnessMap : register(t6);
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

struct PS_INPUT
{
  float4 Position : SV_Position;
  float2 TexCoord : TEXCOORD0;
};

struct PS_OUTPUT
{
  float4 shadow : VS_Target0;
  float4 specular : SV_Target1;
};

float3 fresnelSchlick(float3 F0, float VoH)
{
  return F0 + (1.0f - F0) * pow((1.0f - VoH), 5.0f);
}

float fresnelForLut(float VoH)
{
  return pow(1.0f - VoH, 5.0f);
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

float D_Beckmann(float nDotH, float alpha)
{
  float cos2 = nDotH * nDotH;
  float tan2 = (1.0f - cos2) / (cos2 + 1e-5f);
  return exp(-tan2 / alpha) / (PI * alpha * cos2 * cos2 + 1e-5f);
}

float3 cookTorrenceSpecular(float3 normal,
                            float3 viewDir,
                            float3 lightDir,
                            float rough,
                            float metallic,
                            float3 F0)
{
  float3 Half = normalize(viewDir + lightDir);
  float NoL = saturate(dot(normal, lightDir));
  float NoV = saturate(dot(normal, viewDir));
  float NoH = saturate(dot(normal, Half));
  float VoH = saturate(dot(viewDir, Half));
  
  float alpha = rough * rough;
  float D = D_Beckmann(NoH, alpha);
  float G = GeometrySmith(NoV, NoL, rough);
  float3 F = fresnelSchlick(F0, VoH);

  float den = max(4.0f * NoV * NoL, 1e-5f);
  
  float3 specular = (D * G * F) / den;
  
  return specular;
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
  float4 metallicTex = metallicMap.Sample(samState, input.TexCoord);
  float4 roughTex = roughnessMap.Sample(samState, input.TexCoord);
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
  
  // diffuse
  float shadowColor = 1.0f - ShadowIntensity;
  float3 lightDir = normalize(mul(float4(-LightDir, 0.0f), lightTransform).xyz);
  
  float diff = max(dot(lightDir, normal), shadowColor);
  diff = lerp(diff, shadowColor, 1.0f - diff);
  float3 diffuse = lightColor * diff;
  
  // specular
  float spec = 0.0;
  lightDir = normalize(mul(float4(-LightDir, 0.0f), lightTransform).xyz);
  
  float3 viewDir = normalize(Eye.xyz - worldPos);
  float3 halfwayDir = normalize(lightDir + viewDir);
  spec = pow(max(dot(normal, halfwayDir), 0.0f), SpotExponent);
  
  // float3 specular = (lightColor * (spec * SpecIntensity));
  
  float3 F0 = lerp(0.04f, colorTex.rgb, metallicTex.rgb);
  
  float3 specCookTorrence = cookTorrenceSpecular(normal,
                                                 viewDir,
                                                 lightDir,
                                                 roughTex.r,
                                                 metallicTex.r,
                                                 F0);
  
  float3 specular = specCookTorrence * SpecIntensity;
  
  output.shadow = float4(diffuse, alpha);
  output.specular = float4(specular, alpha);
  
  return output;
}