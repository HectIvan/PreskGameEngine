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


/*
PS_OUTPUT PS(PS_INPUT input) : SV_Target
{
    PS_OUTPUT output = (PS_OUTPUT) 0;
    
    float4 diffusePix = diffuseTex.Sample(samLinear, input.Tex);
    float4 normalPix = normalTex.Sample(samLinear, input.Tex);
    float4 neightPix = heightTex.Sample(samLinear, input.Tex);
    float4 metallicPix = metallicTex.Sample(samLinear, input.Tex);
    float4 occlusionPix = occlusionTex.Sample(samLinear, input.Tex);
    
    if (diffusePix.a < 0.1f)
    {
        discard;
    }
    
    float3 lightDir = normalize(float3(0.5f, -1.0f, -0.3f)); // Example light direction
    float3 lightColor = float3(1.0f, 1.0f, 1.0f); // White light
    
    output.diffuse = diffusePix;
    float3 normal = normalTex.Sample(samLinear, input.Tex).xyz * 2.0f - 1.0f;
    float3x3 TBN = float3x3(input.Tangent, input.Bitangent, input.Normal);
    normal = normalize(mul(normal.xyz, TBN));
    float diff = max(dot(normal, lightDir), 0.0f);
    float3 diffuse = diff * lightColor * diffusePix.rgb;
    output.diffuse = float4(diffuse, diffusePix.a);
    // output.diffuse = diffusePix;
    // output.normal = float4(normal * 0.5f + 0.5f, 1.0f);
    // output.depth = float4(input.Depth.xyz, 1.0f);
  
    return output;
}*/