//--------------------------------------------------------------------------------------
// File: pkVShader.hlsl
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

// Pixel Shader input
struct PS_INPUT
{
    float4 Position : SV_Position;
    float2 TexCoord : TEXCOORD0;
};

struct PS_OUTPUT
{
    float4 diffuse : SV_Target0;
};

// deferred pass
PS_INPUT VS(uint vertexID : SV_VertexID)
{
  PS_INPUT output;
  float2 positions[3] = {
    float2(-1.0f, -1.0f),
    float2(3.0f, -1.0f),
    float2(-1.0f, 3.0f)
  };
  
  output.Position = float4(positions[vertexID].xy, 0.0f, 1.0f);
  output.TexCoord = (positions[vertexID].xy + float2(1.0f, 1.0f)) * 0.5f;
  output.TexCoord.y = 1.0f - output.TexCoord.y;

  return output;
}
