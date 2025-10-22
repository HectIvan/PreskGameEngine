/*****************************************************************************/
/**
 * @file    pkAssimpModelCodec.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    08/10/2025
 * @brief   Codec for loading models using Assimp.
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
#include "pkModelCodec.h"

namespace pkEngineSDK
{

class Window;

class AssimpModelCodec : public ModelCodec
{
 public:
  AssimpModelCodec() = default;
  AssimpModelCodec(const Window& _window);
  virtual ~AssimpModelCodec() = default;

  /**
   * @brief Load a model from a given path.
   * @param _path Path to load the model from.
   * @return Pointer to the loaded model.
   */
  SPtr<Model>
  loadModel(const Path _path) override;
};
}
