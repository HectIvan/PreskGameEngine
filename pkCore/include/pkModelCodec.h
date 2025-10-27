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

class ModelResource;

class PK_CORE_EXPORT ModelCodec : public Module<ModelCodec>
{
 public:
  ModelCodec() = default;
  virtual ~ModelCodec() = default;

  /**
   * @brief Save a model as a pkm to a given path.
   * @param _pModel Pointer to the model to save.
   * @param _path Path to save the model to.
   * @return Model resource.
   */
  ModelResource*
  savePKModel(const SPtr<Model>& _pModel, const Path _path);

  /**
   * @brief Create a model resource from a given path.
   * @param _path Path of the model.
   * @return Pointer to the model resource.
   */
  virtual ModelResource*
  createResourceFromFile(const Path _path) = 0;
};

PK_CORE_EXPORT ModelCodec&
g_ModelCodec();
}
