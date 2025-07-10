Texture2D inputTexture : register(t0);
SamplerState samState : register(s0);

float4 PS(float2 texCoord : TEXCOORD0) : SV_Target0
{
  float4 color = float4(0.0f, 0.0f, 0.0f, 0.0f);
  for (int i = -3; i <= 3; ++i) {
    color += inputTexture.Sample(samState, texCoord + float2(i, 0));
  }
  return color / 7.0f;

}