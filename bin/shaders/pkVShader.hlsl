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

cbuffer cbLight : register(b3)
{
    float type;
    float Attenuation;
    float SpotExponent; // 12
    float3 LightDir; // 24
    float3 LightPos; // 36
    float3 LightColor; // 48
    float2 unused1; // 56
    float2 unused2; // 64
}

cbuffer cbCamera : register(b4)
{
    float4 camPosition;
    float3 camForward;
    float4x4 view;
    float4x4 projection;
    float _unused;
}

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
    output.Tex = input.Tex;
    output.Depth = mul(float4(input.Position.xyz, 1.0f), World);
    
    // light rotates with object
    // output.Normal = input.Normal;
    
    output.Normal = normalize(mul(input.Normal, (float3x3)World));
    output.Tangent = normalize(mul(input.Tangent, (float3x3)World));
    output.Bitangent = normalize(mul(input.Bitangent, (float3x3)World));
    
    return output;
}