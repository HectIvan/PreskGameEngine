/*****************************************************************************/
/**
 * @file    pkDX11GeometryShader.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    06/03/2026
 * @brief   Geometry Shader file for the Presk Game Engine.
 *
 * @bug    No known bugs.
 */
 /*****************************************************************************/
#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkDX11Shader.h"

namespace pkEngineSDK {

class DX11GeometryShader : public DX11Shader
{
public:
  DX11GeometryShader() = default;
  ~DX11GeometryShader() override {
    safeRelease(m_pShader);
  }

public:
  ID3D11GeometryShader* m_pShader = nullptr;
};
}