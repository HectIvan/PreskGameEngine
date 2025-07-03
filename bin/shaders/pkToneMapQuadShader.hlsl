Texture2D albedoTex : register(t0);
Texture2D luminanceTex : register(t1);
Texture2D shadowTex : register(t2);

SamplerState samState : register(s0);

struct PS_INPUT
{
    float4 Position : SV_Position;
    float2 TexCoord : TEXCOORD0;
};  

float4 PS(PS_INPUT input) : SV_Target0
{
    float4 albedoSample = albedoTex.Sample(samState, input.TexCoord);
    float4 luminanceSample = luminanceTex.Sample(samState, input.TexCoord);
    float4 shadowSample = shadowTex.Sample(samState, input.TexCoord);
    // float2 uv = input.Position.xy / float2(1920, 1080);
    
    // float4 shadow = saturate(shadowSample);

    return albedoSample * shadowSample;
}