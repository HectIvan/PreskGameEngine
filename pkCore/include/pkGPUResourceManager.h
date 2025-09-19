/*****************************************************************************/
/**
 * @file    pkGPUResourceManager.h
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
#include "pkMesh.h"
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

struct MeshMemory
{
  Path directory;
  SPtr<Mesh> mesh;
};


class PK_CORE_EXPORT GPUResourceManager : public Module<GPUResourceManager>
{
 public:
  GPUResourceManager() = default;
  virtual ~GPUResourceManager() = default;

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

  /**
   * @brief search for a specific mesh
   * @param _name Name of the mesh.
   * @return Pointer to the mesh if found;
   */
  SPtr<Mesh>
  searchMesh(String _name);

 public:
  Vector<SPtr<ModelMemory>> m_models;
  Vector<SPtr<Mesh>> m_meshes;

  // assets
  // UMap<UUID, Asset> materials
};

PK_CORE_EXPORT GPUResourceManager&
g_GPUResourceManager();
}