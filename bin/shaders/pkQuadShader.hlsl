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

// quad pass
PS_INPUT VS(uint vertexID : SV_VertexID)
{
  PS_INPUT output;
  float2 positions[3] = {
    float2(-1.0f, 1.0f),
    float2(3.0f, 1.0f),
    float2(-1.0f, -3.0f)
  };
    
  float2 uv[3] = {
    float2(0, 0),
    float2(2, 0),
    float2(0, 2)
  };
  
  output.Position = float4(positions[vertexID], 0.0f, 1.0f);
  output.TexCoord = uv[vertexID]; // (positions[vertexID] + float2(1.0f, 1.0f)) * 0.5f;
  // output.TexCoord.y = 1.0f - output.TexCoord.y;
  
  return output;
}
