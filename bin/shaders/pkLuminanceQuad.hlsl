Texture2D inputTexture : register(t0);
SamplerState samState : register(s0);

cbuffer parameters : register(b0)
{
  float2 viewSize;
  float2 unused;
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
  if (texPix.r > 0.95f ||
      texPix.g > 0.95f ||
      texPix.b > 0.95f)
  {
    output.diffuse = texPix; // float4(1.0f, 1.0f, 1.0f, 1.0f);
  }
  return output;
  
  // float2 uv = input.Position.xy / viewSize;
  // 
  // float3 color = inputTexture.Sample(samState, uv).rgb;
  // 
  // float luminance = dot(color, float3(0.2126, 0.7152, 0.0722));
  // 
  // return float4(luminance.xxx, 1.0f);
}