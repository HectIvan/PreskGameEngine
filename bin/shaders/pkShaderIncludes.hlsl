/***************************************************************************************
* Shader includes
***************************************************************************************/

/*******************************************/
/*         CONSTANT BUFEFRS                */
/*******************************************/

cbuffer mainCB : register(b0)
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
  
  float4 Eye; // 16
  float3 ForwardCam; // 28
  float _unusedCam0; // 160
  
  float4 EyeLight; // 16
  float3 ForwardLight; // 28
  float _unusedLightCam0; // 160
  
  float4x4 View;
  float4x4 Proj;
  float4x4 InvView;
  float4x4 InvProj;
  float4x4 TranspView;
  float4x4 TranspProj;
  float4x4 ViewProj;
  
  float4x4 LightView;
  float4x4 LightProj;
  float4x4 LightTranspView;
  float4x4 LightTranspProj;
  float4x4 LightInvView;
  float4x4 LightInvProj;
  float4x4 LightViewProj;
  
  float2 WinSize;
  float2 _unusedWinSize;
}
