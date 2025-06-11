SamplerState samLinear : register(s0);

// Depth texture from the shadow pass
Texture2D shadowMap : register(t0);
// Depth texture from the base render
Texture2D depthMap : register(t1);

cbuffer cbLightCam : register(b0)
{
    float4 lightPosition;
    float3 lightForward;
    float4x4 lightView;
    float4x4 lightProj;
    float _lightUnused;
}

cbuffer cbCamera : register(b1)
{
    float4 camPosition;
    float3 camForward;
    float4x4 view;
    float4x4 projection;
    float _unused;
}

cbuffer cbLightTransform : register(b2)
{
    float4x4 lightTransform;
}

// potential bug when moving or rotating camera, as rotation of camera is managed by the view matrix
cbuffer cbCamTransform : register(b3)
{
    float4x4 camTransform;
}

struct PS_INPUT
{
    float4 Depth : SV_Position;
    float2 Tex : TEXCOORD0;
};

struct PS_OUTPUT
{
    float4 diffuse : SV_Target0;
};

float SampleShadow(float3 worldPos)
{
    return 0.0f;
}

PS_OUTPUT PS(PS_INPUT input) : SV_Target0
{
    PS_OUTPUT output = (PS_OUTPUT) 0;
    float4 shadowTex = shadowMap.Sample(samLinear, input.Tex);
    float4 depthTex = depthMap.Sample(samLinear, input.Tex);
    
    output.diffuse = float4(1, 1, 1, 1) * shadowTex.r;
    output.diffuse.a = 1;

    return output;
}
