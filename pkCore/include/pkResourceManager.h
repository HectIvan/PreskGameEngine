/************************************************************************/
/**
* @pkResourceManager pkResourceManager.h
* @Hector Ivan Muñoz Ceballos
* @date 04/02/2025
* @Resource Manager file for the Presk Game Engine.
*
* This file will contain the Resource Manager used for the engine
*
* @bug No current function.
*
* @HectIvan 06/02/20245
* @brief Added a newMaterial function that creates a material pointer.
* @brief Added a newTexture function that creates a texture pointer.
*/
/************************************************************************/
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
namespace pkEngineSDK
{

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
   * @brief Create a new Texture.
   * @param _name Texture Name.
   * @param _directory Directory to look for.
   * @return The texture pointer.
   */
  SPtr<Texture>
  newTexture(String _name, String _directory);

  /**
   * @brief Loads a model from a file.
   * @param _path File path.
   */
  SPtr<Model>
  loadModel(String& _fileName);
};

PK_CORE_EXPORT ResourceManager&
g_ResourceManager();
}