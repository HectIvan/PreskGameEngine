Texture2D inputTexture : register(t0);
SamplerState samState : register(s0);

cbuffer parameters
{
    float tolerance;
    float3 unused;
};

struct PS_INPUT
{
  float4 Position : SV_POSITION;
  float2 Tex : TEXCOORD0;
};

struct PS_Output
{
    float4 diffuse : SV_Target0;
};

PS_Output PS(PS_INPUT input) : SV_Target0
{
    PS_Output output = (PS_Output) 0;
    
    float4 texPix = inputTexture.Sample(samState, input.Tex);
    output.diffuse = float4(0.0f, 0.0f, 0.0f, 1.0f);
    if (texPix.r > tolerance || 
        texPix.g > tolerance ||
        texPix.b > tolerance)
    {
        output.diffuse = texPix;// float4(1.0f, 1.0f, 1.0f, 1.0f);
    }
    return output;
}