SamplerState samLinear : register(s0);

// Depth texture from the shadow pass
Texture2D shadowMap : register(t0);
// Depth texture from the base render
Texture2D depthMap : register(t1);
// normal texture of the scene
Texture2D normalMap : register(t2);
// metallic texture of the scene
Texture2D metallicMap : register(t3);

// #ifndef PCF_KERNEL_SIZE
// #define PCF_KERNEL_SIZE 10
// #endif

cbuffer Light : register(b0)
{
    float LightType;
    float SpotCutoff;
    float SpotExponent;
    float3 LightDir;
    float3 LightPos;
    float3 LightColor;
    float3 ShadowColor;
    float _unusedLight1;
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

cbuffer CamInvProj : register(b3)
{
    float4x4 camInvProj;
}

cbuffer CamInvView : register(b4)
{
    float4x4 camInvView;
}

cbuffer windowSize : register(b5)
{
    float2 winSize;
    float2 _unusedWin;
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
    projCoord = projCoord * 0.5f + 0.5f;
    
    if (projCoord.x < 0.0f || projCoord.x > 1.0f ||
        projCoord.y < 0.0f || projCoord.y > 1.0f ||
        projCoord.z < 0.0f || projCoord.z > 1.0f)
    {
        return 1.0f; // fully lit if outside shadow map
    }
    
    // get closest depth value from the lights perspective
    float4 closestDepth = shadowMap.Sample(samLinear, projCoord.xy).r;
    // get the depth of the current pixel from the lights perspective
    float currentDepth = projCoord.z;
    // check wether current texture position is in shadow
    float shadow = currentDepth > closestDepth ? 0.0f : 1.0f;
    // return the shadow value
    return shadow;
}

float3 getWorldPos(PS_INPUT input)
{
    float depth = depthMap.Sample(samLinear, input.Tex).r;
    
    // SOLUTION 1 (seems to be the best result so far)
    
    // 1. Clip space to view space (using inverse projection)
    float4 viewSpacePositionHomogeneous = mul(camInvProj, float4(input.Tex, 1.0f, 1.0f));
    float3 viewSpacePosition = viewSpacePositionHomogeneous.xyz / viewSpacePositionHomogeneous.w;
    
    // 2. Reconstruct view space position from depth
    // (This part is highly dependent on your specific setup and depth buffer)
    float viewZ = depth;  // Get Z from depth buffer
    float3 viewRay = normalize(viewSpacePosition);
    float3 viewPosition = viewRay * viewZ;
    
    // 3. View space to world space (using inverse view)
    float4 worldSpacePositionHomogeneous = mul(camInvView, Eye);
    float3 worldSpacePosition = worldSpacePositionHomogeneous.xyz / worldSpacePositionHomogeneous.w;
    
    return worldSpacePosition, 1.0f;
    
    // SOLUTION 2
    /*
    float z = depth * 2.0 - 1.0;

    float4 clipSpacePosition = float4(input.Tex * 2.0 - 1.0, z, 1.0);
    float4 viewSpacePosition = mul(camInvProj, clipSpacePosition);

    // Perspective division
    viewSpacePosition /= viewSpacePosition.w;

    float4 worldSpacePosition = mul(camInvView, viewSpacePosition);

    return worldSpacePosition.xyz;*/
    
    // SOLUTION 3
    /*
    float4 coord = float4(winSize.x, winSize.y, depth, 1.0f);
    coord = mul(coord, camInvProj);
    float3 camera_space = coord.xyz / coord.w;
    float3 world_space = mul(float4(camera_space, 1.0f), camInvView).xyz; // if needed
    return world_space;*/
}

PS_OUTPUT PS(PS_INPUT input) : SV_Target0
{
    PS_OUTPUT output = (PS_OUTPUT) 0;
    float4 shadowTex = shadowMap.Sample(samLinear, input.Tex);
    float4 depthTex = depthMap.Sample(samLinear, input.Tex);
    float4 normalTex = normalMap.Sample(samLinear, input.Tex);
    float4 metallicTex = metallicMap.Sample(samLinear, input.Tex);
    
    // remap normal from 0,1 to -1,1
    float3 normal = normalize(normalTex.xyz);
    
    // generate the light or shadow first pass
    // in this part, replacing getWorldPos with input.position will make the shine move with the camera
    float3 lightDir = normalize(LightPos - getWorldPos(input)); // input.Position.xyz);
    // utilize the normal map to generate the shadows in normal maps
    float surfaceNormalShadow = max(dot(lightDir, normal), 0.0f);
    float3 diffuse = LightColor * surfaceNormalShadow;
    
    // specular
    float3 viewDir = normalize(Eye - input.Position);
    float spec = 0.0f;
    float3 halfWayDir = normalize(lightDir + viewDir);
    spec = pow(max(dot(normal, halfWayDir), 0.0f), 64.0f);
    float3 specular = spec * LightColor;
    
    // calculate shadows
    float4x4 lightVP = mul(ViewLight, ProjectionLight);
    float shadow = ShadowCalculation(mul(lightVP, float4(getWorldPos(input), 1.0f))); // input.Position));
    float3 ambient = 0.1f * LightColor;
    float3 lighting = ambient + shadow * (diffuse + specular);// * metallicTex.r));
    
    output.diffuse = float4(lighting, 1.0f); // 

    return output;
}
