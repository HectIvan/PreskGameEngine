Texture2D luminanceTex : register(t0);
SamplerState samState : register(s0);

cbuffer CBSize
{
    float2 targetSize;
    float2 unised;
};

struct PS_INPUT
{
    float4 Position : SV_Position;
    float2 TexCoord : TEXCOORD0;
};

struct PS_OUTPUT
{
    float4 diffuse : SV_Target0;
};

PS_OUTPUT HBlur(PS_INPUT input) : SV_Target0
{
    PS_OUTPUT output = (PS_OUTPUT) 0;
    if (input.TexCoord.x >= targetSize.x || input.TexCoord.y >= targetSize.y)
    {
        return output;
    }

    float2 coord = input.TexCoord / targetSize;
    float2 texelSize = float2(1.0f / targetSize.x, 1.0f / targetSize.y);

    float2 offsets[5] =
    {
        float2(-2.0f, 0.0f) * texelSize,
        float2(1.0f, 0.0f) * texelSize,
        float2(0.0f, 0.0f),
        float2(1.0f, 0.0f) * texelSize,
        float2(2.0f, 0.0f) * texelSize
    };

    float kernel[5] = { 0.1f, 0.2f, 0.4f, 0.2f, 0.1f };

    float3 sum = float3(0.0f, 0.0f, 0.0f);

    for (int i = 0; i < 5; ++i)
    {
        float3 color = luminanceTex.Sample(samState, input.TexCoord).rgb;
        sum += color * kernel[i];
    }

    output.diffuse = float4(sum, 1.0f);
}

PS_OUTPUT VBlur(PS_INPUT input) : SV_Target0
{
    PS_OUTPUT output = (PS_OUTPUT) 0;
    if (input.TexCoord.x >= targetSize.x || input.TexCoord.y >= targetSize.y)
    {
        return output;
    }

    float2 coord = input.TexCoord / targetSize;
    float2 texelSize = float2(1.0f / targetSize.x, 1.0f / targetSize.y);

    float2 offsets[5] =
    {
        float2(0.0f, -2.0f) * texelSize,
        float2(0.0f, 1.0f) * texelSize,
        float2(0.0f, 0.0f),
        float2(0.0f, 1.0f) * texelSize,
        float2(0.0f, 2.0f) * texelSize
    };

    float kernel[5] = { 0.1f, 0.2f, 0.4f, 0.2f, 0.1f };

    float3 sum = float3(0.0f, 0.0f, 0.0f);

    for (int i = 0; i < 5; ++i)
    {
        float3 color = luminanceTex.Sample(samState, input.TexCoord).rgb;
        sum += color * kernel[i];
    }

    output.diffuse = float4(sum, 1.0f);
}