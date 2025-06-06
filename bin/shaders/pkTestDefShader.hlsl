Texture2D albedoTex : register(t0);

SamplerState samState : register(s0);

struct PS_INPUT
{
    float4 Position : SV_Position;
    float2 TexCoord : TEXCOORD0;
};  

float4 PS(PS_INPUT input) : SV_Target0
{
    float4 albedoSample = albedoTex.Sample(samState, input.TexCoord);

    return albedoSample;
}