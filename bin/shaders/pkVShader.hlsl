/***************************************************************************************
* Constant Buffer Structures
***************************************************************************************/

cbuffer cbView : register(b0)
{
    matrix View;
};

cbuffer cbProjection : register(b1)
{
    matrix Projection;
};

cbuffer cbWorld : register(b2)
{
    matrix World;
};

struct VS_INPUT
{
  float3 Position : POSITION;
  float3 Normal : NORMAL0;
  float2 Tex : TEXCOORD0;
  float3 Tangent : TANGENT0;
  float3 Bitangent : BINORMAL0;
};

struct PS_INPUT
{
  float4 Position : SV_POSITION;
  float2 Tex : TEXCOORD0;
  float3 Normal : TEXCOORD1;
  float3 Tangent : TEXCOORD2;
  float3 Bitangent : TEXCOORD3;
  float3 Depth : TEXCOORD4;
};

PS_INPUT VS(VS_INPUT input)
{
  PS_INPUT output = (PS_INPUT) 0;

  float4x4 wvp = mul(World, mul(View, Projection));

  output.Position = mul(float4(input.Position.xyz, 1.0f), wvp);
  
  float4 localPos = float4(input.Position.xyz, 1.0f);
  // position of pixel in local position
  float4 viewPos = mul(localPos, World);
  output.Depth = viewPos.xyz;
  output.Tex = input.Tex;
  
  output.Normal = normalize(mul(float4(input.Normal, 0.0f), World).xyz);
  output.Tangent = normalize(mul(float4(input.Tangent, 0.0f), World).xyz);
  output.Bitangent = normalize(cross(output.Normal, output.Tangent));
  
  return output;
}