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
#include "pkMesh.h"
#include "pkModule.h"
#include "pkPrerequisitesCore.h"
#include "pkTexture.h"
#include "pkPath.h"

namespace pkEngineSDK
{

class Model;

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
   * @brief Initialize the GPU resource manager.
   */
  void
  init();

  /**
   * @brief Create a new Material component.
   * @param _name Name of the new material.
   * @return Pointer to the new material object.
   */
  SPtr<Material>
  newMaterial(String _name = "newMaterial");

  /**
   * @brief Loads a model from a file.
   * @param _directory File path.
   */
  SPtr<Model>
  loadModel(Path _directory);

  /**
   * @brief Loads a model from a .pkm file.
   * @param _path File path.
   * @return Pointer to the model if loaded; nullptr if failed.
   */
  SPtr<Model>
  loadPKModel(Path _path);

  /**
   * @brief Loads a model from a file using Assimp.
   * @param _path File path.
   * @return Pointer to the model if loaded; nullptr if failed.
   */
  SPtr<Model>
  loadAssimpModel(Path _path);

  /**
   * @brief search for a specific mesh
   * @param _name Name of the mesh.
   * @return Pointer to the mesh if found;
   */
  SPtr<Mesh>
  searchMesh(const String _name);

 public:
  Vector<SPtr<ModelMemory>> m_models;
  Vector<SPtr<Mesh>> m_meshes;

  // unique string for the default material
  String m_defaultMatName = "defaultMaterial";
  SPtr<Material> m_defaultMaterial;

  // assets
  // UMap<UUID, Asset> materials
};

PK_CORE_EXPORT GPUResourceManager&
g_GPUResourceManager();
}