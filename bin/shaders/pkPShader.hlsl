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
    float4 metallic : SV_Target2;
    // float4 depth : COLOR0;
};

// surface shape (bumps)
float ndf_Beckmann(float Ndh, float roughnessSquared)
{
    float alphaSqr = roughnessSquared * roughnessSquared;
    float cosThetaHSqr = Ndh * Ndh;
    
    if (cosThetaHSqr <= 0.0f) {
        return 0.0f;
    }
    
    float tanThetaHSqr = (1.0f - cosThetaHSqr) / cosThetaHSqr;
    float e = exp(-tanThetaHSqr / alphaSqr);

    return e / (M_PI * alphaSqr * cosThetaHSqr * Ndh);
}

float ndf_GGX(float NdH, float roughness, float squaredRoughness)
{
    float alphaSq = squaredRoughness * squaredRoughness;
    float NdotHSq = NdH * NdH;
    
    // standard GGX distribution
    float denom = (NdotHSq * (alphaSq - 1.0f) + 1.0f);
    denom = M_PI * denom * denom;
    
    // standard GGX output
    float D = alphaSq / max(denom, DELTA);
    
    // multi scattering compensation factor
    float multiScatterFactor = 1.0f + roughness;
    return min(alphaSq / denom * multiScatterFactor, 1.0f);
}

// L dot N
// L = light direction
// N = normal surface
float Lambert(float3 normal, float3 lightDir)
{
    return max(0.0f, dot(lightDir, normal));
}

// diffuse + Ks * (R dot V)^n
// ks = specular component
// R = reflected light direction on surface
// V = vector between the point and the view direction
// n = intensity of the specular bump
float3 PhongSpecular(float3 normal, float3 lightDir, float3 viewDir, float intensity)
{
    // light reflected on the surface
    float3 lightReflect = reflect(-normalize(LightDir), normal);
    // specular intensity
    float3 spec = max(dot(lightReflect, viewDir), 0.0f);
    // multiplier with the light intensity
    float3 finalVal = pow(spec, intensity);
    return finalVal;
}

// Blinn-Phong
float3 BlinnPhong(float3 normal, float3 lightDir, float3 viewDir, float intensity)
{
    float3 halfVector = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfVector), 0.0f), intensity);
    return spec * float3(1.0f, 1.0f, 1.0f);
}

float3 TangentToWorld(float3 normalTS, float3x3 TBN)
{
    return normalize(mul(normalTS, TBN));
}

PS_OUTPUT PS(PS_INPUT input) : SV_Target0
{
    PS_OUTPUT output = (PS_OUTPUT) 0;
    float4 AO = occlusionTex.Sample(samLinear, input.Tex);

    float4 colorSam = diffuseTex.Sample(samLinear, input.Tex);
    if (colorSam.a <= 0.1f)
    {
        clip(-1);
    }
    output.diffuse = colorSam * AO;
    float3 metallicSam = metallicTex.Sample(samLinear, input.Tex);
    float3 normalSam = normalTex.Sample(samLinear, input.Tex) * 2.0f - 1.0f;
    float3x3 TBN = float3x3(input.Tangent, input.Bitangent, input.Normal);
    normalSam = normalize(mul(normalSam.xyz, TBN));
    output.normal = float4(normalSam, 1.0f);
    // output.depth = float4(input.Depth.xyz, 1.0f);
    output.metallic = float4(metallicSam, 1.0f);
    
    return output;
}