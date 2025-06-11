Texture2D diffuseTex : register(t0);
Texture2D normalTex : register(t1);
Texture2D heightTex : register(t2);
Texture2D metallicTex : register(t3);
Texture2D occlusionTex : register(t4);
Texture2D testDepthLight : register(t5);

SamplerState samLinear : register(s0);

#define M_PI 3.14159f
#define DELTA 1.0f

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

struct PS_INPUT
{
    float4 Position : SV_POSITION;
    float2 Tex : TEXCOORD0;
    float3 Normal : TEXCOORD1;
    float3 Tangent : TEXCOORD2;
    float3 Bitangent : TEXCOORD3;
    float3 Depth : TEXCOORD4;
};

struct PS_OUTPUT
{
    float4 diffuse : VS_Target0;
    float4 normal : SV_Target1;
  // float4 depth : COLOR0;
};

PS_OUTPUT PS(PS_INPUT input) : SV_Target0
{
    PS_OUTPUT output = (PS_OUTPUT) 0;
    // get the base color
    float4 diffuseSam = diffuseTex.Sample(samLinear, input.Tex);
    
    output.diffuse = diffuseSam;
    return output;
}