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

class AssimpModelCodec : public ModelCodec
{
 public:
  AssimpModelCodec() = default;
  virtual ~AssimpModelCodec() = default;

  /**
   * @brief Create a model resource from a given path using Assimp.
   * @param _path Path of the model.
   * @return Pointer to the model resource.
   */
  SPtr<BaseResource>
  createResourceFromFile(const Path _path) override;

  /**
   * @brief Convert an aiBone to a Bone.
   * @param _name Name of the bone.
   * @param _ID ID of the bone.
   * @param _channel aiNodeAnim channel of the bone.
   * @return The converted Bone.
   */
  Bone
  aiboneToBone(const String& _name, int32 _ID, const aiNodeAnim* _channel);
};
}
