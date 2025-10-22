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
   * @brief Save a model as a pkm to a given path.
   * @param _pModel Pointer to the model to save.
   * @param _path Path to save the model to.
   * @return True if the model was saved successfully, false otherwise.
   */
  bool
  savePKModel(const SPtr<Model>& _pModel, const Path _path);

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
