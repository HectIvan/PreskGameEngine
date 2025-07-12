/***************************************************************************************
* Constant Buffer Structures
***************************************************************************************/

#pragma kernel CS_Main

// unordered texture for reading/writing.
RWTexture2D<float4> outputTexture : register(u0);

// resources
Texture2D<float4> normalMap : register(t0);
Texture2D<float4> metallicMap : register(t1);
Texture2D<float4> shadowMap : register(t2);
Texture2D<float4> depthMap : register(t3);
// sampler state
SamplerState samState : register(s0);

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
  float spotExponent; // 56
  float2 lightPadd4; // 64
}
cbuffer Camera : register(b1)
{
  float4 Eye; // 16
  float3 ForwardCam; // 28
  float4x4 ViewCam; // 92
  float4x4 ProjectionCam; // 156
  float _unusedCam0; // 160
  float4 _paddingC;
}

cbuffer LightCamera : register(b2)
{
  float4 EyeLight; // 16
  float3 ForwardLight; // 28
  float4x4 ViewLight; // 92
  float4x4 ProjectionLight; // 156
  float _unusedLightCam0; // 160
  float4 _paddingL2;
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

/*-------------------------------------------------------------------------------------------*/

// get linear depth from a base depth map
float LinearDepth(float z)
{
  return (farNear.x * farNear.y) / (farNear.y - z * (farNear.y - farNear.x));
}

// convert from depth to world position
float3 WorldPosFromDepth(float2 TexCoord, float DepthSample)
{
  float z = DepthSample * 2.0f - 1.0f;

  float4 clipSpacePosition = float4(TexCoord * 2.0f - 1.0f, z, 1.0f);
  float4 viewSpacePosition = mul(camInvProj, clipSpacePosition);

  // Perspective division
  float3 VSP = viewSpacePosition.xyz /= viewSpacePosition.w;

  float4 worldSpacePosition = mul(camInvView, float4(VSP, 1.0f));

  return worldSpacePosition.xyz;
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
  float4 shadowTex = shadowMap.Load(int3(DTid.xy, 0));
  float4 depthTex = depthMap.Load(int3(DTid.xy, 0));
  float4 normalTex = normalMap.Load(int3(DTid.xy, 0));
  float4 metallicTex = metallicMap.Load(int3(DTid.xy, 0));
  
  float3 normal = normalize(normalTex.xyz);
  
  // get world position from depth map
  float3 worldPos = WorldPosFromDepth(DTid.xy, depthTex.r);
  
  // diffuse
  float shadowColor = 1.0f - ShadowIntensity;
  float3 lightDir = normalize(lightPos - worldPos);
  float diff = max(dot(lightDir, normal), shadowColor);
  float3 diffuse = lightColor * diff;
  // specular
  float3 viewDir = normalize(ForwardCam);
  float spec = 0.0;
  float3 halfwayDir = normalize(lightDir + viewDir);
  spec = pow(max(dot(normal, halfwayDir), 0.0f), spotExponent);
  float3 specular = lightColor * spec;
  
  float3 finalColor = diffuse + specular;
  
  float alpha = 1.0f;

  // if its the max depth value
  if (depthTex.r == 1) 
  {
    alpha = 0.0f;
  }
  
  outputTexture[DTid.xy] = float4(finalColor, alpha);
}