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

class BaseResource;
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
  SPtr<BaseResource>
  createResourceFromModel(const SPtr<Model>& _pModel, const Path& _path);

  /**
   * @brief Create a model resource from a given path.
   * @param _path Path of the model.
   * @return Pointer to the model resource.
   */
  virtual SPtr<BaseResource>
  createResourceFromFile(const Path _path) = 0;

  /**
   * @brief Get the possible extensions for model search in folders.
   * @return possible extensions for model search in folders.
   */
  const String
  getPossibleExtensions() const
  {
    return "*.fbx;*.obj;*.gltf";
  }
};

PK_CORE_EXPORT ModelCodec&
g_ModelCodec();
}
