Texture2D posMap : register(t0);
Texture2D normalMap : register(t1);

SamplerState samState : register(s0);

cbuffer ssao : register(b0)
{
  float sampleRadius;
  float scale;
  float bias;
  float intensity;
}

cbuffer TexSize : register(b1)
{
  float2 screen_size;
  float2 unused;
}

struct PS_INPUT
{
  float4 Position : SV_Position;
  float2 TextCoord : TEXCOORD0;
};

struct PS_OUTPUT
{
  float4 ssaoOut : SV_Target0;
};

float4 getPosition(in float2 uv)
{
  return posMap.Sample(samState, uv);
}

float4 getNormal(in float2 uv)
{
  float4 normal = normalMap.Sample(samState, uv);
  normal.xyz = normal.xyz * 2.0f - 1.0f;
  return normal;
}

float2 getRandom(in float2 uv)
{
  float noiseX = (frac(sin(dot(uv, float2(15.8989f, 76.132f) * 1.0f)) * 46336.23745f));
  float noiseY = (frac(sin(dot(uv, float2(11.9899f, 62.223f) * 2.0f)) * 34748.34744f));
  float noiseZ = (frac(sin(dot(uv, float2(13.3238f, 63.122f) * 3.0f)) * 59998.47362f));
    
  return normalize(float3(noiseX, noiseY, noiseZ));
}

float computeAO(in float2 tcood, in float2 uv, in float3 p, in float3 cnorm)
{
  float3 diff = getPosition(tcood + uv).xyz - p;
  const float d = length(diff) * scale;
  const float v = normalize(diff);
  return max(0.0f, dot(cnorm, v) - bias) * (1.0f / (1.0f + d)) * intensity;
}

PS_OUTPUT PS(PS_INPUT input) : SV_TARGET
{
  float2 screenUV = input.Position.xy / screen_size;
  PS_OUTPUT output = (PS_OUTPUT) 0;
  
  float4 normal = getNormal(screenUV);
  if (normal.w == 0.0f)
  {
    clip(-1);
  }
  
  float4 pos = getPosition(screenUV);
  
  if (pos.w == 0.0f)
  {
    output.ssaoOut = 1.0f;
    return output;
  }
  
  float3 n = normal.xyz;
  float2 rand = getRandom(screenUV);
  
  float ao = 0.0f;
  float rad = sampleRadius / -pos.x;
  
  float2 vec[4] =
  {
    float2(1.0f, 0.0f),
   float2(-1.0f, 0.0f),
   float2(0.0f, 1.0f),
   float2(0.0f, -1.0f)
  };

  int iter = 4;
  for (int j = 0; j < iter; ++j)
  {
    float2 coord1 = reflect(vec[j], rand) * rad;
    float2 coord2 = float2(coord1.x * 0.707 - coord1.y * 0.707,
                           coord1.x * 0.707 - coord1.y * 0.707);
     
    ao += computeAO(screenUV, coord1 * 0.25f, pos.xyz, n);
    ao += computeAO(screenUV, coord2 * 0.5f, pos.xyz, n);
    ao += computeAO(screenUV, coord2 * 0.75f, pos.xyz, n);
    ao += computeAO(screenUV, coord2, pos.xyz, n);
  }
  
  ao /= (iter * 4);
  output.ssaoOut.r = max(1.0f - ao, 0.2f);
  
  return output;
}