/*****************************************************************************/
/**
 * @file    pkMaterialManager.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    04/11/2025
 * @brief   Manager used for creating and storing materials.
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
#include "pkPrerequisitesCore.h"
#include "pkMaterial.h"
#include "pkModule.h"

namespace pkEngineSDK
{

class PK_CORE_EXPORT MaterialManager : public Module<MaterialManager>
{
 public:
   MaterialManager() = default;
  virtual ~MaterialManager() = default;

  /**
   * @brief initialize the material manager by loading a default material.
   */
  void
  init();

  // /**
  //  * @brief on shut down of the module.
  //  */
  // virtual void
  // onShutDown();

  /**
   * @brief Load texture from a texture id.
   * @param _ID ID of the material to load.
   * @return Pointer to the newly created material.
   */
  SPtr<Material>
  loadMaterial(const String& _ID);

  /**
   * @brief Get a saved material via its ID.
   * @param _ID ID to look for.
   * @return Pointer to the saved material.
   */
  SPtr<Material>
  getMaterial(const String& _ID);
  
  /**
   * @brief Store a loaded material into the memory for later use.
   * @param _ID ID of the material.
   * @param _pMaterial Material to store.
   */
  void
  insertLoadedMaterial(const String& _ID, const SPtr<Material>& _pMaterial);

 public:
  // save materials with the material id
  UMap<String, SPtr<Material>> m_materials;
};
PK_CORE_EXPORT MaterialManager&
g_MaterialManager();
}
