/*****************************************************************************/
/**
 * @file    pkModelCodec.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    08/10/2025
 * @brief   Codec for loading models.
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
#include "pkModel.h"
#include "pkModule.h"
#include "pkPath.h"

namespace pkEngineSDK
{

class PK_CORE_EXPORT ModelCodec : public Module<ModelCodec>
{
 public:
  ModelCodec() = default;
  virtual ~ModelCodec() = default;

  // /**
  //  * @brief Load .pkm file.
  //  */
  // void
  // loadPKM() { return; }
  
  /**
   * @brief Load a model from a given path.
   * @param _path Path to load the model from.
   * @return Pointer to the loaded model.
   */
  virtual SPtr<Model>
  loadModel(const Path _path) = 0;
};

PK_CORE_EXPORT ModelCodec&
g_ModelCodec();
}
