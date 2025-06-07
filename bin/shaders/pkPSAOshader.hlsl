Texture2D posMap : register(t0);
Texture2D normalMap : register(t1);
Texture2D Random : register(t2);

SamplerState samState : register(s0);

float2 g_screen_size;
float random_size;

float g_sample_rad;
float g_scale;
float g_bias;
float g_intensity;

struct PS_INPUT
{
  float4 Position : SV_Position;
  float2 TextCoord : TEXCOORD0;
};

struct PS_OUTPUT
{
  float4 color : SV_Target0;
};

float4 getPosition(in float2 uv)
{
  return posMap.Sample(samState, uv);
}

float3 getNormal(in float2 uv)
{
  return normalize(normalMap.Sample(samState, uv));
}

float2 getRandom(float2 uv)
{
    return normalize(Random.Sample(samState, g_screen_size * uv / random_size).xy);
}

float3 randomVector(float2 uv)
{
  float z = 1.0f - 2.0f * dot(uv, uv);
  float a = 6.28318530718f * uv.x;
  float s = sqrt(1.0f - z * z);
  return float3(cos(a) * s, sin(a) * s, z);
}

float computeAO(in float2 tcoord, in float2 uv, in float3 p, in float3 cnorm)
{
  float3 diff = getPosition(tcoord + uv).xyz - p;
  const float d = length(diff) * g_scale;
  const float3 v = normalize(diff);
  return max(0.0f, dot(cnorm, v) - g_bias) * (1.0f / (1.0f + d)) * g_intensity;
}

float4 PS(PS_INPUT input) : SV_Target0
{
  float4 normalTexture = normalMap.Sample(samState, input.TextCoord);

  // check if the pixel can be drawn in (done in the normal pass)
  if (normalTexture.w == 0.0f)
  {
    clip(-1);
  }

  float4 pos = getPosition(input.TextCoord);
  float3 n = normalTexture.xyz;
  float2 rand = getRandom(input.TextCoord);

  float ao = 0.0f;
  float rad = g_sample_rad / -pos.x;

  float2 vec[4] = {
    float2(1,0), 
    float2(-1, 0),
    float2(0,1),
    float2(0,-1)
  };

  // sampling
  int iterations = 4;
  for (int j = 0; j < iterations; ++j)
  {
    float2 coord1 = reflect(vec[j], rand) * rad;
    // for variation, rotation of 45°
    float2 coord2 = float2(coord1.x * 0.707f - coord1.y * 0.707,
                           coord1.x * 0.707f + coord1.y * 0.707);

    ao += computeAO(input.TextCoord, coord1 * 0.25f, pos.xyz, n);
    ao += computeAO(input.TextCoord, coord2 * 0.50f, pos.xyz, n);
    ao += computeAO(input.TextCoord, coord1 * 0.75f, pos.xyz, n);
    ao += computeAO(input.TextCoord, coord2        , pos.xyz, n);
  }

  ao /= (iterations * 4);
  
  return 1.0f - ao;
}