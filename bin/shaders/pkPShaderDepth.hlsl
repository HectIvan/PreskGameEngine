Texture2D diffuseTex : register(t0);

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

cbuffer AlphaThreshold : register(b5)
{
    float fAlphaThreshold;
    float3 _unusedAlpha;
}

struct VS_INPUT
{
    float3 Position : SV_POSITION;
    float2 Tex : TEXCOORD0;
    float3 Normal : TEXCOORD1;
    float3 Tangent : TEXCOORD2;
    float3 Bitangent : TEXCOORD3;
    float3 Depth : TEXCOORD4;
};

struct PS_INPUT
{
    float4 position : SV_Position;
    float2 tex : TEXCOORD0;
};

PS_INPUT VS(VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT) 0;
    float4x4 viewProj = mul(projection, view);
    output.position = mul(mul(float4(input.Position, 1.0f), World), viewProj);
    output.tex = input.Tex;
    return output;
}

float4 PS(PS_INPUT input) : SV_Target0
{
    float4 color = diffuseTex.Sample(samLinear, input.tex);
    if (color.a <= fAlphaThreshold)
    {
        clip(-1);
    }
    float depth = input.position.z / input.position.w;
    return float4(depth.xxx, 1.0f);
}