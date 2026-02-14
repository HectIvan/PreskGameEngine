/***************************************************************************************
* Constant Buffer Structures
***************************************************************************************/

cbuffer cbView : register(b0)
{
  row_major matrix View;
};

cbuffer cbProjection : register(b1)
{
  row_major matrix Projection;
};

cbuffer cbWorld : register(b2)
{
  row_major matrix World;
};

cbuffer cbWorldInvTransp : register(b4)
{
  row_major matrix worldInvTransp;
};

struct VS_INPUT
{
  float3 Position : POSITION;
  float3 Normal : NORMAL0;
  float2 Tex : TEXCOORD0;
  float4 Tangent : TANGENT0;
  float3 Bitangent : BINORMAL0;
};

struct PS_INPUT
{
  float4 PosSS : SV_POSITION;
  float2 Tex : TEXCOORD0;
  float3 Normal : TEXCOORD1;
  float3 Tangent : TEXCOORD2;
  float3 Bitangent : TEXCOORD3;
  float3 posWS : TEXCOORD4;
};

PS_INPUT VS(VS_INPUT input)
{
  PS_INPUT output = (PS_INPUT) 0;
  // world-view-projection transform matrix
  float4x4 wvp = mul(mul(World, View), Projection);
  
  // position of the mesh vertex in local position.
  float4 vertexLocalPos = float4(input.Position.xyz, 1.0f);
  // position  of the vertex in screen-space
  output.PosSS = mul(vertexLocalPos, wvp);
  
  // convert the position to world space.
  float4 vertexWorldPos = mul(vertexLocalPos, World);
  output.posWS = vertexWorldPos.xyz;
  
  output.Tex = input.Tex;
  
  // float3 normal = normalize(input.Normal);
  // float3 tangent = normalize(input.Tangent.xyz);
  // float3 bitangent = normalize(cross(normal, tangent) * input.Tangent.w);
  
  // normal fix
  // float4x4 transform = worldInvTransp;
  
  output.Normal = normalize(mul(float4(input.Normal, 0.0f), worldInvTransp).xyz);
  output.Tangent = normalize(mul(float4(input.Tangent.xyz, 0.0f), worldInvTransp).xyz);
  // output.Bitangent = normalize(mul(float4(bitangent, 0.0f), World).xyz) * input.Tangent.w;
  output.Bitangent = normalize(cross(output.Normal, output.Tangent)) * input.Tangent.w;
  
  return output;
}