Texture2D hdrTexture : register(t0);
SamplerState samState : register(s0);

#define PI 3.14159265359

cbuffer FaceIndex : register(b0)
{
  float faceID;
  float3 unused;
};

struct PS_INPUT
{
  float4 Position : SV_Position;
  float2 TexCoord : TEXCOORD0;
};

float2 SampleEquirectangular(float3 dir)
{
  float2 uv;
  uv.x = atan2(dir.z, dir.x) / (2.0 * PI) + 0.5;
  uv.y = acos(dir.y) / PI;
  uv.y = 1.0f - uv.y;
  return uv;
}

float3 GetCubemapDirection(uint face, float2 uv)
{
    // Transform UV from [0,1] to [-1,1]
  float2 xy = uv * 2.0 - 1.0;

  float3 forward, right, up;

    // Forward vectors for faces
  float3 faceForward[6] =
  {
    float3(1, 0, 0),
    float3(-1, 0, 0),
    float3(0, 1, 0),
    float3(0, -1, 0),
    float3(0, 0, 1),
    float3(0, 0, -1),
  };

  float3 faceUp[6] =
  {
    float3(0, 1, 0),
    float3(0, 1, 0),
    float3(0, 0, -1),
    float3(0, 0, 1),
    float3(0, 1, 0),
    float3(0, 1, 0),
  };

  forward = faceForward[face];
  up = faceUp[face];
  right = normalize(cross(up, forward));
  up = cross(forward, right);

  return normalize(xy.x * right + xy.y * up + forward);
}

float4 PS(PS_INPUT input) : SV_Target
{
  float3 dir = GetCubemapDirection(faceID, input.TexCoord);
  float2 eqUV = SampleEquirectangular(dir);
  float3 color = hdrTexture.Sample(samState, eqUV).rgb;
  return float4(color, 1.0);
}