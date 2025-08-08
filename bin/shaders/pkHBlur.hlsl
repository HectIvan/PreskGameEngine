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

// exponent of (x2 + y2) / (2sigma^2)
float gaussian(float2 offset, float sigma)
{
  float magn = pow(offset.x, 2.0f) + pow(offset.y, 2.0f);
  return exp(-(magn / (2.0 * pow(sigma, 2.0f))));
}

// Gaussian Blur
float4 PS(PS_INPUT input) : SV_Target0
{
  // size of a pixel in the UVs.
  float2 texelSize = 1.0f / WinSize;
  // swap between directions.
  float2 direction = Direction * texelSize;
  
  // blur color
  float4 BlurColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
  
  float sumWeights = 0.0f;
  
  // for each pixel in the loop
  for (float i = -Radius; i <= Radius; ++i) {
    for (float j = -Radius; j <= Radius; ++j)
    {
      // get the new uv sample
      float gauss = gaussian(texelSize, Strength);
      float2 uv = input.TexCoord + float2(i, j) * texelSize;
      float4 texSample = textureResource.Sample(samState, uv);
      BlurColor += texSample * gauss * Strength;
      sumWeights += gauss;
    }
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
