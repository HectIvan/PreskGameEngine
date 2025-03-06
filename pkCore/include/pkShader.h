/************************************************************************/
/**
* @pkShader pkShader.h
* @Hector Ivan Muñoz Ceballos
* @date 17/12/2024
* @Shader file for the Presk Game Engine.
*
* This file contains the Shader class of the engine
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
#include "pkPrerequisitesCore.h"

namespace pkEngineSDK
{

class Shader
{
 public:
  Shader() = default;
  virtual ~Shader() = default;

  /**
   * @brief Compile the shader from a file.
   * @param _szFileName Name of the shader.
   * @param _szEntryPoint What section will we compile.
   * @param _szShaderModel What model of the shader is compiled.
   * @return If the compilation was successful or not.
   */
  virtual uint32
  compileShaderFromFile(WString _szFileName,
                        const char* _szEntryPoint,
                        const char* _szShaderModel) = 0;
};
}