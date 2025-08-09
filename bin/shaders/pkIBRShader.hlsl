Texture2D skyboxMap : register(t0);
Texture2D normalMap : register(t1);
Texture2D posMap : register(t2);
Texture2D metallicMap : register(t3);
Texture2D roughnessMap : register(t4);
Texture2D albedoMap : register(t5);

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

float4 PS(PS_INPUT input) : SV_Target
{
  // get the world normals
  float4 normalTex = normalMap.Sample(samState, input.TexCoord);
  float4 position = posMap.Sample(samState, input.TexCoord);
  float4 roughTex = roughnessMap.Sample(samState, input.TexCoord);
  float4 albedoTex = albedoMap.Sample(samState, input.TexCoord);
  float metallicTex = metallicMap.Sample(samState, input.TexCoord).b;
  
  // normalize normals and view
  float3 N = normalize(normalTex.xyz);
  float3 V = normalize(position.xyz - ViewPosition);
  
  // reflect the view direction on the normal to get the view reflected
  float3 view = reflect(V, N);
  
  // sample the skybox with the new direction
  float2 skyboxUV = getSkyBoxUV(view);
  float3 IBL = skyboxMap.Sample(samState, skyboxUV).rgb;
  return float4(IBL * metallicTex * Intensity, 1.0f);
  
  // get how much of the surounding light will be reflected.
  float3 metallicColor = IBL * metallicTex;
  // how much of the object color will seep through the IBL
  float3 roughColor = albedoTex.rgb * IBL * (1 - roughTex.g);
  
  return float4((metallicColor + roughColor) * Intensity, 1.0f);
}