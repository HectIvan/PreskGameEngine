Texture2D textureResource : register(t0);
SamplerState samState : register(s0);

cbuffer CBSize
{
    float2 targetSize;
    float2 unused;
};

struct PS_INPUT
{
    float4 Position : SV_Position;
    float2 TexCoord : TEXCOORD0;
};

struct PS_OUTPUT
{
    float4 luminance : SV_Target0;
};

PS_OUTPUT PS(PS_INPUT input) : SV_Target0
{
    PS_OUTPUT output = (PS_OUTPUT) 0;
    float2 texelSize = 1.0 / targetSize;

    // Horizontal offsets (X direction)
    float kernel[2] = { 0.1f, 0.1f };
    float offsetX[2] = { -2.0f, 2.0f };

    float3 sum = float3(0.0f, 0.0f, 0.0f);

    for (int i = 0; i < 2; ++i)
    {
        float2 offsetCoord = input.TexCoord + float2(offsetX[i], 0.0f) * texelSize;
        sum += textureResource.Sample(samState, offsetCoord).rgb * kernel[i];
    }

    output.luminance = float4(sum, 1.0f);
    return output;
}
