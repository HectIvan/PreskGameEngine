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

float3 VSPositionFromDepth(float2 vTexCoord, float4 DepthSample)
{
    // Get the depth value for this pixel
    float z = DepthSample.r;
    // Get x/w and y/w from the viewport position
    float x = vTexCoord.x * 2 - 1;
    float y = (1 - vTexCoord.y) * 2 - 1;
    float4 vProjectedPos = float4(x, y, z, 1.0f);
    // Transform by the inverse projection matrix
    float4 vPositionVS = mul(vProjectedPos, camInvProj);
    // Divide by w to get the view-space position
    return vPositionVS.xyz / vPositionVS.w;
}

float4 PS(PS_INPUT input) : SV_Target0
{
    PS_OUTPUT output = (PS_OUTPUT) 0;
    /**
     * light data
     */
    float3 lightColor = LightColor.xyz;
    float3 lightDir = LightDir.xyz;
    float3 lightPos = LightPos.xyz;
    float3 shadowColor = ShadowColor.xyz;
    
    /**
     * texture data
     */
    float2 uv = input.Position.xy / winSize;
    
    float4 shadowTex = shadowMap.Sample(samLinear, uv);
    float4 depthTex = depthMap.Sample(samLinear, uv);
    float4 normalTex = normalMap.Sample(samLinear, uv);
    float4 metallicTex = metallicMap.Sample(samLinear, uv);
    
    float normal = normalize(normalTex);
    // diffuse
    float3 worldPos = VSPositionFromDepth(input.Position.xy, depthTex);
    lightDir = normalize(lightPos - worldPos);
    float diff = max(dot(lightDir, normal), 0.0);
    float3 diffuse = diff * lightColor;
    // specular
    float3 viewDir = normalize(Eye.xyz - worldPos);
    float spec = 0.0;
    float3 halfwayDir = normalize(lightDir + viewDir);
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    float3 specular = spec * lightColor;
    
    return float4(diffuse + specular, 1.0);
}
