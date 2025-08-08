Texture2D textureResource : register(t0);
SamplerState samState : register(s0);

cbuffer cBlur : register(b0)
{
  float2 WinSize;
  float2 Direction;
  float Radius;
  float Strength;
  float2 _unused;
};

struct PS_INPUT
{
  float4 Position : SV_Position;
  float2 TexCoord : TEXCOORD0;
};

struct PS_OUTPUT
{
  float4 blur : SV_Target0;
};

float gaussian(float x, float sigma)
{
  return exp(-(x * x) / (2.0 * sigma * sigma));
}

float4 PS(PS_INPUT input) : SV_Target0
{
  //        float2 texelSize = 1.0 / WinSize;
  //        float2 direction = Direction * texelSize;
            
  //        float sigma = Strength;
  //        int radius = int(ceil(Radius * sigma));
            
  //        float totalWeight = 0.0;
  //        float4 result = float4(0, 0, 0, 0);
            
  //        for (int i = -radius; i <= radius; ++i)
  //        {
  //          float weight = gaussian(i, sigma);
  //          float2 offset = float(i) * direction;
  //          result += textureResource.Sample(samState, input.TexCoord + offset) * weight;
  //          totalWeight += weight;
  //        }
            
  //        return result / totalWeight;
  
  
  
  //        float2 texelSize = 1.0f / WinSize;
  //        float2 direction = Direction * texelSize;
  //        
  //        float sigma = Strength;
  //        int radius = int(ceil(Radius * sigma)); // 99% of Gaussian is within ±3σ
  //        
  //        float totalWeight = 0.0f;
  //        float4 result = float4(0.0f, 0.0f, 0.0f, 0.0f);
  //        
  //        for (int i = -radius; i <= radius; ++i)
  //        {
  //          float weight = gaussian(i, sigma);
  //          float2 offset = float(i) * direction;
  //          result += textureResource.Sample(samState, input.TexCoord + offset) * weight;
  //          totalWeight += weight;
  //        }
  //        
  //        return result / totalWeight;
  
  // size of a pixel in the UVs.
  float2 texelSize = 1.0f / WinSize;
  // swap between directions.
  float2 direction = Direction * texelSize;
  
  // get the diameter of the sampling (if the radius is 3, the diameter is 3 * 2 + 1) +1 is for the current pixel.
  float diameter = 2.0f * Radius + 1.0f;
  // box area of the blur
  float blurDivisor = pow(diameter, 2.0f);
  // blur color
  float4 BlurColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
  
  // for each pixel in the loop
  for (float i = -Radius; i <= Radius; ++i) {
    for (float j = -Radius; j <= Radius; ++j)
    {
      // get the new uv sample
      // i really struggle to understand why cant i just use direction instead of a for loop.
      float2 uv = input.TexCoord + float2(i, j) * texelSize;
      float4 texSample = textureResource.Sample(samState, uv);
      BlurColor += texSample * Strength;
    }
  }
  BlurColor /= blurDivisor;
  
  return saturate(BlurColor);
}
