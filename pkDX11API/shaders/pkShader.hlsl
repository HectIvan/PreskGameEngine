//--------------------------------------------------------------------------------------
// File: Tutorial07.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

/***************************************************************************************
* Constant Buffer Variables
***************************************************************************************/
Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

#define M_PI 3.14159265383
#define GAMMA 2.2f

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
    float4 ViewPos;
    float TimeData;
};

cbuffer cbLight : register(b3)
{
    float Attenuation;
    float SpotExponent;
    float3 LightDir;
    float3 LightPos;
    float3 LightColor;
}

cbuffer BoneTransform : register(b4)
{
    float4x4 Transform[100];
}

/***********************
* Inputs
***********************/
struct VS_INPUT
{
    float3 Pos : POSITION0;
    float3 Normal : NORMAL0;
    float2 Tex : TEXCOORD0;
};

struct PS_INPUT
{
    float4 Pos : SV_Position;
    float3 Normal : TEXCOORD0;
    float2 Tex : TEXCOORD1;
};

struct PS_Output
{
    float4 diffuse : SV_Target0;
};

float2 UnpackUV(float2 uv)
{
    return uv * 8.0f / 32767.0;
}

/***********************
* Lambert for multiple lights
***********************/
float lambertValue(float3 normal, float3 lightDir)
{
    return max(0.0f, dot(normal, lightDir));
}

/***********************
* Model parts
***********************/
struct MODEL_VERTEX
{
    float x;
    float y;
    float z;
    float u;
    float v;
};

/***********************
* Vertex Shader
***********************/
PS_INPUT VS(VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT) 0;
    output.Pos = mul(float4(input.Pos, 1.0f), World);
    output.Pos = mul(output.Pos, View);
    output.Pos = mul(output.Pos, Projection);
    // output.Tex = UnpackUV(input.Tex.xy);
    
    // Light rotates with model
    // output.Normal = input.Normal;
    
    //Static directional light
    output.Normal = mul(float4(input.Normal, 0.0f), World);
    output.Tex = input.Tex;
    
    return output;
}


/***********************
* Pixel Shader
***********************/

float4 PS( PS_INPUT input) : SV_Target0
{
    // PS_Output output;
    // output.diffuse = float4(1.0f, 1.0f, 1.0f, 1.0f);
    // return output;
    return txDiffuse.Sample( samLinear, input.Tex );
} 

// float4 PS(PS_INPUT input) : SV_Target
// {
//     float4 lightColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
//     // Lambert Component
//     float3 LightPos = LightDir;
//     
//     /************************************************************
//     *              LIGHT DIRECTION
//     ************************************************************/
//     float3 lightDir = LightPos;// focus
//     float distToLight = length(lightDir);
//     float sqrDistToLight = distToLight * distToLight;
//     lightDir = LightDir / distToLight;
//     
//     
//     float3 normal = normalize(input.Normal);
//     float lambert = max(0.0f, dot(normal, lightDir)); // lambertValue(normal, lightDir); // 
//     
//     /************************************************************
//     * calculate light attenuation
//     ************************************************************/
//     float AttenuationConstant = 1.0f;
//     float AttenuationLinear = 0.27f;
//     float AttenuationQuadratic = 0.22f;
//     
//     float Attenuation = 1.0f / (AttenuationConstant +
//                                 AttenuationLinear * distToLight
//                                 + AttenuationQuadratic * sqrDistToLight);
//     
//     
//     float4 DifColor = float4(txDiffuse.Sample(samLinear, input.Tex).xyz, 1.0f);
//     
//     
//     float4 DiffuseValue = lambert * Attenuation * float4(lightColor) * DifColor;
//     
//     /************************************************************
//     * Calculate Specular Reflection Component
//     ************************************************************/
//     float3 viewDir = normalize(ViewPos - input.Pos);
//     float3 halfDir = normalize(LightDir + viewDir);
//     float3 specular = pow(max(0.0f, dot(normal, halfDir)), 2.0f);
//     
//     float3 AmbientColor = float3(0.2f, 0.2f, 0.3f);
//     
//     // BRDF (Bi-Directional Reflectance Distribution Function)
//     
//     // kD + kS + kA
//     // k = Absorbtion Constant - Diffuse (Difusa) = 80%
//     // k = Reflection Specular Constant = 20%
//     // k = Ambien Reflecion Constant = 3-10%
//     
//     float3 fColor = DiffuseValue.xyz + ((1.0f - DifColor.w) * specular) + AmbientColor;
//     
//     return float4(fColor.xyz, GAMMA);
// }
