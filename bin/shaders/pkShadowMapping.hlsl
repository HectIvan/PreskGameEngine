SamplerState samLinear : register(s0);

// Depth texture from the shadow pass
Texture2D shadowMap : register(t0);
// Depth texture from the base render
Texture2D depthMap : register(t1);
// normal texture of the scene
Texture2D normalMap : register(t2);

cbuffer Light : register(b0)
{
    float LightType;
    float SpotCutoff;
    float SpotExponent;
    float3 LightDir;
    float3 LightPos;
    float3 LightColor;
    float2 _unusedLight0;
    float2 _unusedLight1;
}

cbuffer Camera : register(b1)
{
    float4 Eye;
    float3 ForwardCam;
    float4x4 ViewCam;
    float4x4 ProjectionCam;
    float _unusedCam0;
}

cbuffer LightCamera : register(b2)
{
    float4 EyeLight;
    float3 ForwardLight;
    float4x4 ViewLight;
    float4x4 ProjectionLight;
    float _unusedLightCam0;
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

float ShadowCalculation(float4 fragPosLightSpace)
{
    // perspective divide
    float3 projCoord = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to 1,0 range
    projCoord *= (0.5f + 0.5f);
    // get closest depth value from the lights perspective
    float4 shadowTex = shadowMap.Sample(samLinear, projCoord.xy);
    float closestDepth = shadowTex.r;
    // get the depth of the current pixel from the lights perspective
    float currentDepth = projCoord.z;
    // check wether current texture position is in shadow
    float shadow = currentDepth > closestDepth ? 1.0f : 0.0f;
    // return the shadow value
    return shadow;
}

PS_OUTPUT PS(PS_INPUT input) : SV_Target0
{
    PS_OUTPUT output = (PS_OUTPUT) 0;
    float4 shadowTex = shadowMap.Sample(samLinear, input.Tex);
    float4 depthTex = depthMap.Sample(samLinear, input.Tex);
    float4 normalTex = normalMap.Sample(samLinear, input.Tex);
    
    float3 normal = normalize(normalTex.xyz);
    
    // generate the light or shadow first pass
    float3 lightDir = normalize(LightPos - input.Position.xyz);
    // utilize the normal map to generate the shadows in normal maps
    float surfaceNormalShadow = max(dot(LightDir, normal), 0.0f);
    float3 diffuse = float3(1, 1, 1) * surfaceNormalShadow;
    
    // specular
    float3 viewDir = normalize(Eye - input.Position);
    float spec = 0.0f;
    float3 halfWayDir = normalize(Eye.xyz + ForwardCam);
    spec = pow(max(dot(normal, halfWayDir), 0.0f), 64.0f);
    float3 specular = spec * LightColor;
    
    // calculate shadows
    float4x4 lightVP = mul(ViewLight, ProjectionLight);
    float shadow = ShadowCalculation(mul(lightVP, input.Position));
    float3 lighting = (LightColor + (1.0f - shadow) * (diffuse + specular)) * float3(1, 1, 1);
    
    output.diffuse = float4(diffuse, 1.0f); // float4(lighting, 1.0f);

    return output;
}
