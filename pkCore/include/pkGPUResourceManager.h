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
 */
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
   * @brief Loads a model from a .pkm file.
   * @param _ID Resource ID.
   * @return Pointer to the model if loaded; nullptr if failed.
   */
  SPtr<Model>
  loadPKModel(const String& _ID);

  /**
   * @brief search for a specific mesh
   * @param _name Name of the mesh.
   * @return Pointer to the mesh if found;
   */
  SPtr<Mesh>
  searchMesh(const String _name);

  /**
   * @brief Insert a model into the memory.
   * @param _ID ID of the model to store.
   * @param _pModel Model to store.
   */
  void
  insertModelMemory(const String& _ID, const SPtr<Model>& _pModel);

  /**
   * @brief Get a model from memory.
   * @param _ID ID of the model to look for.
   * @return Pointer to the model. null if its not stored.
   */
  SPtr<Model>
  getModelMemory(const String& _ID);

 public:
  UMap<String, SPtr<Model>> m_models;
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