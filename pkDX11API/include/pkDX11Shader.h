/************************************************************************/
/**
* @pkDX11Shader pkDX11Shader.h
* @Hector Ivan Muñoz Ceballos
* @date 11/11/2024
* @DirectX Shader file for the Presk Game Engine.
*
* This file contains the Shader class using DirectX11 of the engine
*
* @bug No bug known.
*/
/************************************************************************/
#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkDX11Prerequisites.h"
#include "pkShader.h"

namespace pkEngineSDK
{

using std::wstring;

class DX11Shaders : public Shader
{
 public:
  DX11Shaders() = default;
  virtual ~DX11Shaders() {
    safeRelease(pSBlob);
  }

  /**
   * @brief Compile the shader from a file.
   * @param _szFileName Name of the shader.
   * @param _szEntryPoint What section will we compile.
   * @param _szShaderModel What model of the shader is compiled.
   * @param _ppBlobOut Blob info.
   * @return If the compilation was successful or not.
   */
  uint32
  compileShaderFromFile(wstring _szFileName,
                        const char* _szEntryPoint,
                        const char* _szShaderModel) override;

 public:
  ID3DBlob* pSBlob = nullptr;
};
}
