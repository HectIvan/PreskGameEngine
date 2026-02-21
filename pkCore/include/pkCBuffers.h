/*****************************************************************************/
/**
 * @file    pkCBuffers.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    15/07/2025
 * @brief   Buffers definitions for the game engine.
 *
 * @bug    No known bugs.
 */
/*****************************************************************************/
#pragma once
#include "pkCamera.h"
#include "pkLight.h"
#include "pkMatrix4.h"
#include "pkVector2.h"


namespace pkEngineSDK
{

struct CBMatrix
{
  Matrix4 matrix;
};

struct CBView
{
  Matrix4 view;
};

struct CBProjection
{
  Matrix4 projection;
};

struct CBTransform
{
  Matrix4 transform;
};

struct CBSSAO // 16 bytes
{
  CBSSAO() = default;
  CBSSAO(float _sampleRad,
         float _scale,
         float _bias,
         float _intensity)
    :
    sample_rad(_sampleRad),
    scale(_scale),
    bias(_bias),
    intensity(_intensity) {
  }
  float sample_rad;
  float scale;
  float bias;
  float intensity;
};

struct CBCamera
{
  CBCamera() = default;
  CBCamera(const SPtr<Camera>& _pCam) {
    eye = _pCam->getEye();
    forward = _pCam->getForward();
    view = _pCam->getView();
    projection = _pCam->getProjection();
  }
  Vector4 eye = Vector4(0.0f, 0.0f, 0.0f, 1.0f); // 16
  Vector3 forward = Vector3::FORWARD; // 12
  Matrix4 view = Matrix4::IDENTITY; // 64
  Matrix4 projection = Matrix4::IDENTITY; // 64
  float unused; // 4
}; // 160 bytes

struct CBLight
{
  CBLight() = default;
  CBLight(const SPtr<Light>& _pLight) {
    direction = Vector4(_pLight->m_direction, 9.9999f);
    position = Vector4(_pLight->m_position, 9.9999f);
    color = Vector4(_pLight->m_color, 1.0f);
    shadowIntensity = _pLight->m_shadowIntensity;
    spotExponent = _pLight->m_spotExponent;
    spotCutoff = _pLight->m_spotCutoff;
    specIntensity = _pLight->m_specIntensity;
    transform = _pLight->m_transform;
  }
  Vector4 direction = Vector4(0.0f, -1.0f, 0.0f, 0.0f); // 16
  Vector4 position = Vector4(0.0f, 50.0f, 0.0f, 1.0f); // 32
  Vector4 color = Vector4(1.0f); // 48
  float shadowIntensity = 0.75f; // 52
  float spotExponent = 32.0f; // 56
  float spotCutoff = 0.9f; // 60
  float specIntensity = 1.0f; // 64
  Matrix4 transform = Matrix4::IDENTITY; // 128
};

struct CBAOData
{
  Vector2 screen_size;
  float random_size;

  float sample_rad;
  float scale;
  float bias;
  float intensity;
  float unused;
};

struct CBVector2x2 // 16 bytes
{
  CBVector2x2() = default;
  CBVector2x2(const Vector2& _vec1, const Vector2& _vec2)
    : vec1(_vec1),
      vec2(_vec2) {}
  CBVector2x2(const float& _f1, const float& _f2, const float& _f3, const float& _f4)
    : vec1(_f1, _f2),
      vec2(_f3, _f4) {}

  Vector2 vec1;
  Vector2 vec2;
};

struct CBBlur // 32 bytes
{
  CBBlur() = default;
  CBBlur(const Vector2 _winSize,
         const Vector2 _blurDir,
         const float _radius,
         const float _strength)
    :
    WinSize(_winSize),
    BlurDirection(_blurDir),
    radius(_radius),
    strength(_strength) {}

  Vector2 WinSize;
  Vector2 BlurDirection;
  float radius;
  float strength;
  Vector2 _unused;
};

struct CBFloat // 16 bytes
{
  CBFloat() = default;
  CBFloat(const float _value) : value(_value) {}

  float value;
  Vector3 unused;
};

struct CBVector3 // 16 bytes
{
  CBVector3() = default;
  CBVector3(const Vector3& _vec) : vec1(_vec) {}

  Vector3 vec1;
  float unused;
};

struct CBMaterialProps
{
  Vector3 ColorMultiply = Vector3(1.0f);
  float roughnessMultiply = 1.0f;
  Vector3 EmissiveMultiply = Vector3(1.0f);
  float metallicMultiply = 1.0f;
};
}
