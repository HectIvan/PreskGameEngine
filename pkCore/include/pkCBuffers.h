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

struct CBCamera
{
  Vector4 eye; // 16
  Vector3 forward; // 12
  Matrix4 view; // 64
  Matrix4 projection; // 64
  float unused; // 4
}; // 176 bytes

struct CBLight
{
  Vector4 direction; // 16
  Vector4 position; // 32
  Vector4 color; // 48
  float shadowIntensity; // 52
  float spotExponent; // 56
  float spotCutoff; // 60
  float specIntensity; // 64
  Matrix4 transform; // 128
};

/* to do: find a better way of creating the buffers from the object, i feel like they'll have
          my head for doing this. */
struct CreateCBLight
{
  static void
  create(CBLight& _lightDesc, SPtr<Light> _pLight) {
    _lightDesc.direction = Vector4(_pLight->m_direction, 9.9999f);
    _lightDesc.position = Vector4(_pLight->m_position, 9.9999f);
    _lightDesc.color = Vector4(_pLight->m_color, 1.0f);
    _lightDesc.shadowIntensity = _pLight->m_shadowIntensity;
    _lightDesc.spotExponent = _pLight->m_spotExponent;
    _lightDesc.spotCutoff = _pLight->m_spotCutoff;
    _lightDesc.specIntensity = _pLight->m_specIntensity;
    _lightDesc.transform = _pLight->m_transform;
  }
};

struct CreateCBCamera
{
  static void
    create(CBCamera& _camDesc, SPtr<Camera> _pCam) {
    _camDesc.eye = _pCam->m_eye;
    _camDesc.forward = _pCam->getForward();
    _camDesc.view = _pCam->m_view;
    _camDesc.projection = _pCam->m_projection;
  }
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

struct CBBlur
{
  Vector2 winSize;
  float blurXOffset;
  float unused;
};

struct CBVector2x2
{
  Vector2 vec1;
  Vector2 vec2;
};

struct CBFloat
{
  float value;
  Vector3 unused;
};

struct CBVector3
{
  Vector3 vec1;
  float unused;
};
}
