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
   DX11GeometryShader() {
     m_shaderType = PK_SHADER_TYPE::kGeometry;
   }
  ~DX11GeometryShader() override {
    safeRelease(m_pShader);
  }

  // deletes the possibility of the constructor and operator in the class.
  DX11GeometryShader(const DX11GeometryShader&) = delete;
  DX11GeometryShader& operator=(const DX11GeometryShader&) = delete;

  // movement functions deleted.
  DX11GeometryShader(DX11GeometryShader&&) noexcept = delete;
  DX11GeometryShader& operator=(DX11GeometryShader&&) noexcept = delete;

 public:
  ID3D11GeometryShader* m_pShader = nullptr;
};
}