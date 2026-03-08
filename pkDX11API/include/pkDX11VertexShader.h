/*****************************************************************************/
/**
 * @file    pkDX11VertexShader.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    11/11/2024
 * @brief   Vertex Shader file for the Presk Game Engine.
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

class DX11VertexShader : public DX11Shader
{
 public:
  DX11VertexShader() = default;
  ~DX11VertexShader() override {
    safeRelease(m_pShader);
  }

 public:
  ID3D11VertexShader* m_pShader = nullptr;
};
}
