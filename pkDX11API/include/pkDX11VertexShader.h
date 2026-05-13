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
  DX11VertexShader() {
    m_shaderType = PK_SHADER_TYPE::kVertex;
  }
  ~DX11VertexShader() override {
    safeRelease(m_pShader);
  }

  // deletes the possibility of the constructor and operator in the class.
  DX11VertexShader(const DX11VertexShader&) = delete;
  DX11VertexShader& operator=(const DX11VertexShader&) = delete;

  // movement functions deleted.
  DX11VertexShader(DX11VertexShader&&) noexcept = delete;
  DX11VertexShader& operator=(DX11VertexShader&&) noexcept = delete;

 public:
  ID3D11VertexShader* m_pShader = nullptr;
};
}
