TextureCube skyboxMap : register(t0);
Texture2D depthMap : register(t1);

SamplerState samState : register(s0);

cbuffer CameraData : register(b0)
{
  float4 Eye; // 16
  float3 ForwardCam; // 28
  float4x4 ViewCam; // 92
  float4x4 ProjectionCam; // 156
  float _unusedCam0; // 160
  float4 _padding; // 172
};

cbuffer Transform : register(b1)
{
  float4x4 transformCam;
};

struct VS_INPUT
{
  float3 pos : POSITION;
};

struct VS_OUTPUT
{
  float4 pos : SV_POSITION;
  float3 texCoord : TEXCOORD0;
  float3 posW : TEXCOORD1;
};

VS_OUTPUT VS(VS_INPUT input)
{
  VS_OUTPUT output;

  float4x4 viewNoTranslation = ViewCam;
  viewNoTranslation._41 = 0;
  viewNoTranslation._42 = 0;
  viewNoTranslation._43 = 0;

  float4 posWorld = mul(float4(input.pos, 1.0f), transformCam);
  output.posW = posWorld.xyz;
  float4 posView = mul(posWorld, viewNoTranslation);
  output.pos = mul(posView, ProjectionCam);

    // Aquí debes pasar la dirección para el cubemap:
  output.texCoord = mul(input.pos, (float3x3) transformCam);

  return output;
}

float4x4 InverseAffine(float4x4 m)
{
  float3 right = m[0].xyz;
  float3 up = m[1].xyz;
  float3 forward = m[2].xyz;
  float3 trans = m[3].xyz;

  float3x3 rot = float3x3(right, up, forward);
  float3x3 rotInv = transpose(rot); // only if orthographic

  float3 transInv = -mul(rotInv, trans);

  float4x4 result = float4x4(
        float4(rotInv[0], 0),
        float4(rotInv[1], 0),
        float4(rotInv[2], 0),
        float4(transInv, 1)
    );

  return result;
}

float3 GetCameraPositionFromViewMatrix(float4x4 viewMatrix)
{
  // extract rotation axis and translation
  float3 right = viewMatrix[0].xyz;
  float3 up = viewMatrix[1].xyz;
  float3 forward = viewMatrix[2].xyz;
  float3 trans = viewMatrix[3].xyz;

  // inverse of the rotation matrix (transposed) and translation
  float3x3 rotInv = transpose(float3x3(right, up, forward));
  float3 camPos = -mul(rotInv, trans);

  return camPos;
}


float4 PS(VS_OUTPUT input) : SV_Target
{
  // float2 ndc = input.Texcoord * 2.0f - 1.0f;
  // ndc.y *= -1.0f;
  // 
  // float4x4 invVP = mul(invCamProj, invCamView);
  // float4 viewPos = mul(float4(ndc, 1.0f, 1.0f), invVP);
  // viewPos /= viewPos.w;
  // 
  // float3 viewDir = normalize(Eye.xyz);
  // float2 skyboxUV = getSkyBoxUV(viewDir);
  // float3 color = skyboxMap.Sample(samState, skyboxUV).rgb;
  // float4 depth = depthMap.Sample(samState, input.Texcoord);
  // float4 final = float4(0.0f, 0.0f, 0.0f, 1.0f);
  // if (depth.r >= 1.0f) {
  //   final = float4(color, 1.0f);
  // }
  // return final;
  float3 camPos = GetCameraPositionFromViewMatrix(ViewCam);
    
  float3 viewDir = normalize(input.posW - camPos);
  return skyboxMap.Sample(samState, viewDir);
}