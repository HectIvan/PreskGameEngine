Texture2D textureResource : register(t0);
SamplerState samState : register(s0);

#define PI 3.14159265f
#define E 2.71828f

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

uniform float weight[5] = { 0.227027f, 0.1945946f, 0.1216216f, 0.054054f, 0.016216f };

// exponent of (x2 + y2) / (2sigma^2)
float gaussian(float2 offset, float sigma)
{
  float magn = pow(offset.x, 2.0f) + pow(offset.y, 2.0f);
  return exp(-(magn / (2.0 * pow(sigma, 2.0f))));
}

// w(x) = exp(-(x-μ)² / (2σ²))
float gaussian2D(float2 distance, float sigma)
{
  float val2 = exp(-(pow(distance.x + distance.y, 2.0f) / (2.0f * pow(sigma, 2.0f))));
  return val2;
}

// 1 / sqrt(2 * PI * o) * e^(x^2 / (2 * o^2))
float gaussianDistribution(float x, float sigma)
{
  // float o2 = o * o;
  // float exponent = -((x * x) / (2.0f * o2));
  // float mP = (1.0f / sqrt(2.0f * PI * o2));
  // float y = mP * exp(exponent);
  // return y;
  return exp(-0.5f * (x * x) / (sigma * sigma)) / (sigma * sqrt(2 * PI));
  // return exp(-(x * x) / (2.0 * sigma * sigma));

}

// Gaussian Blur
float4 PS(PS_INPUT input) : SV_Target0
{
  // uint width;
  // uint height;
  // textureResource.GetDimensions(width, height);
  // float2 textureSize = float2(width, height);
  // 
  // float2 tex_offset = 1.0f / textureSize; // gets size of single texel
  // float3 result = textureResource.Sample(samState, input.TexCoord).rgb * weight[0]; // current fragment's contribution
  // if (Direction.x != 0.0f)
  // {
  //   for (int i = 1; i < 5; ++i)
  //   {
  //     result += textureResource.Sample(samState, input.TexCoord + float2(tex_offset.x * i, 0.0)).rgb * weight[i];
  //     result += textureResource.Sample(samState, input.TexCoord - float2(tex_offset.x * i, 0.0)).rgb * weight[i];
  //   }
  // }
  // else
  // {
  //   for (int i = 1; i < 5; ++i)
  //   {
  //     result += textureResource.Sample(samState, input.TexCoord + float2(0.0, tex_offset.y * i)).rgb * weight[i];
  //     result += textureResource.Sample(samState, input.TexCoord - float2(0.0, tex_offset.y * i)).rgb * weight[i];
  //   }
  // }
  // return float4(result, 1.0);
  // size of a pixel in the UVs.
  uint width;
  uint height;
  textureResource.GetDimensions(width, height);
  float2 textureSize = float2(width, height);
  float2 texelSize = 1.0f / textureSize;
  
  // blur color
  float4 BlurColor = float4(0, 0, 0, 0);
  
  float sumWeights = 0.0f;
  
  // for each pixel in the loop
  for (int i = -Radius; i <= Radius; ++i) {
    float gauss = gaussianDistribution(i, Strength);
    float2 texelOffset = Direction / textureSize;
    float2 uv = input.TexCoord + texelOffset * i;
    float4 texSample = textureResource.Sample(samState, uv);
    BlurColor += texSample * gauss;
    sumWeights += gauss;
  }
  BlurColor /= sumWeights;
  
  return saturate(BlurColor);
}

// Box Blur
float4 PSBox(PS_INPUT input) : SV_Target0
{
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
  for (float i = -Radius; i <= Radius; ++i)
  {
    for (float j = -Radius; j <= Radius; ++j)
    {
      // get the new uv sample
      float2 uv = input.TexCoord + float2(i, j) * texelSize;
      float4 texSample = textureResource.Sample(samState, uv);
      BlurColor += texSample * Strength;
    }
  }
  BlurColor /= blurDivisor;
  
  return saturate(BlurColor);
}
