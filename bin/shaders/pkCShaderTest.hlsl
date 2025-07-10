/***************************************************************************************
* Constant Buffer Structures
***************************************************************************************/

#pragma kernel CS_Main

// unordered texture for reading/writing.
RWTexture2D<float4> outputTexture : register(u0);

// resources
Texture2D<float4> baseMap : register(t0);
// sampler state
SamplerState samState : register(s0);

// write directly onto the texture.
[numthreads(16, 16, 1)]
void CSMain(uint3 DTid : SV_DispatchThreadID)
{
  // get the pixel in the position of the thread
  float4 color = baseMap.Load(int3(DTid.xy, 0));
  // write white for testing
  outputTexture[DTid.xy] = color;
}