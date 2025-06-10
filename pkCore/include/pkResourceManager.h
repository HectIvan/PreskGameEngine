/*****************************************************************************/
/**
 * @file    pkResourceManager.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    04/02/2025
 * @brief   Resource Manager used for the engine
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
#include "pkMaterial.h"
#include "pkModel.h"
#include "pkModule.h"
#include "pkPrerequisitesCore.h"
#include "pkTexture.h"
#include "pkPath.h"

namespace pkEngineSDK
{

/**
 * Memory structures used to store already loaded data and call it when needed.
 */
struct ModelMemory
{
  Path directory;
  SPtr<Model> model;
};


class PK_CORE_EXPORT ResourceManager : public Module<ResourceManager>
{
 public:
  ResourceManager() = default;
  virtual ~ResourceManager() = default;

  /**
   * @brief Create a new Material component.
   * @param _textureName Name of the texture file to load.
   * @return Pointer to the new material object.
   */
  SPtr<Material>
  newMaterial();

  /**
   * @brief Loads a model from a file.
   * @param _directory File path.
   */
  SPtr<Model>
  loadModel(Path _directory);

 public:
  Vector<SPtr<ModelMemory>> m_models;
};

PK_CORE_EXPORT ResourceManager&
g_ResourceManager();
}