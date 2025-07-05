SamplerState samLinear : register(s0);

// Depth texture from the shadow pass
Texture2D shadowMap : register(t0);
// Depth texture from the base render
Texture2D depthMap : register(t1);
// normal texture of the scene
Texture2D normalMap : register(t2);
// metallic texture of the scene
Texture2D metallicMap : register(t3);

cbuffer cbLight : register(b0)
{
    float3 LightDir; // 12
    float padd1;
    float3 LightPos; // 24
    float padd2;
    float3 LightColor; // 36
    float padd3;
    float3 ShadowColor; // 48
    float padd4;
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

cbuffer windowSize : register(b5)
{
    float2 winSize; // 8
    float2 _unusedWin; // 8
}

struct PS_INPUT
{
    float4 Position : SV_Position;
    float2 Tex : TEXCOORD0;
};

struct PS_OUTPUT
{
    float4 diffuse : SV_Target0;
};

float pcfFilter(float2 uv, float depth, float texelSize)
{
    float shadow = 0.0f;
    uint sampleCount = 0;
    
    for (int y = -10; y <= 10; ++y)
    {
        for (int x = -10; x <= 10; ++x)
        {
            float2 offset = float2(x, y) * texelSize;
            float sampleDepth = shadowMap.Sample(samLinear, uv + offset).r;
            
            if (depth > sampleDepth)
            {
                shadow = 1.0f;
            }
            sampleCount++;
        }
    }
    return 1.0f - (shadow / sampleCount);
}

float ShadowCalculation(float4 fragPosLightSpace)
{
    return 1.0f;
}

float3 getWorldPos(float2 clipPos)
{
    return float3(1.0f, 1.0f, 1.0f);
}

float4 PS(PS_INPUT input) : SV_Target0
{
    PS_OUTPUT output = (PS_OUTPUT) 0;
    // light data
    float3 lightColor = LightColor.xyz;
    float3 lightDir = LightDir.xyz;
    float3 lightPos = LightPos.xyz;
    float3 shadowColor = ShadowColor.xyz;
    
    
    // output.diffuse = ;
    return float4(shadowColor, 1.0f);
}
