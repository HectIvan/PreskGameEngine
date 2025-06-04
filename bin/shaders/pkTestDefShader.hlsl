// 
struct PS_INPUT
{
    float4 Position : SV_Position;
    float2 TexCoord : TEXCOORD0;
};  

float4 PS(PS_INPUT input) : SV_Target0
{
    // return float4(input.TexCoord, 0, 1);
    return float4(1, 0, 1, 1);
}