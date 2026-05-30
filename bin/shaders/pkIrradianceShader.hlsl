TextureCube environmentMap : register(t0);
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
  float3 irradiance = float3(0, 0, 0);
  float numSamples = 0;

  float3 N = normalize(GetCubemapDirection(faceID, input.TexCoord));
  float3 up = abs(N.y) < 0.999f ? float3(0, 1, 0) : float3(1, 0, 0);
  float3 right = normalize(cross(up, N));
  up = normalize(cross(N, right));

  for (float phi = 0.0f; phi < 2.0f * PI; phi += 0.05f)
  {
    for (float theta = 0.0f; theta < 0.5f * PI; theta += 0.05f)
    {
      float3 tangentSample;

      tangentSample.x = sin(theta) * cos(phi);
      tangentSample.y = sin(theta) * sin(phi);
      tangentSample.z = cos(theta);

      float3 sampleVec = normalize(tangentSample.x * right +
                                   tangentSample.y * up +
                                   tangentSample.z * N);

      irradiance += environmentMap.Sample( samState, sampleVec).rgb * cos(theta) * sin(theta);
      numSamples += 1.0f;
    }
  }

  irradiance = PI * irradiance * (1.0f / numSamples);

  return float4(irradiance, 1.0f);
}
