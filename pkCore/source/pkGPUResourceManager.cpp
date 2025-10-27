#include "pkAssetResourceManager.h"
#include "pkLogger.h"
#include "pkGPUResourceManager.h"
#include "pkGraphicsAPI.h"
#include "pkModel.h"
#include "pkModelResource.h"
#include "pkTexture.h"
#include "pkTextureManager.h"

namespace pkEngineSDK
{

void
GPUResourceManager::init()
{
  m_defaultMaterial = newMaterial(m_defaultMatName);
}

SPtr<Material>
GPUResourceManager::newMaterial(String _name)
{
  // create the material.
  if (_name == m_defaultMatName) {
    _name = "<Invalid Name>";
  }
  SPtr<Material> pMatComp = make_shared<Material>(_name);
  // return the material.
  return pMatComp;
}

SPtr<Model>
GPUResourceManager::loadPKModel(const Path _path)
{
  SPtr<Model> model = searchModel(_path);
  if (model) {
    return model;
  }

  GraphicsAPI& api = g_GraphicAPI();
  AssetResourceManager& assetResMgr = g_AssetResourceManager();

  model = make_shared<Model>();
  ModelResource* modelRes = assetResMgr.loadModelResource(_path);

  model->index = modelRes->m_index;
  model->vertex = modelRes->m_vertex;
  model->meshes = modelRes->m_meshes;

  model->setVerticesIndices();

  model->m_vertexB = api.createVertexBuffer(model->vertex);
  model->m_indexB = api.createIndexBuffer(model->index);
  api.setIndexBuffer(model->m_indexB);
  api.setVertexBuffer(model->m_vertexB);

  insertModelMemory(model, _path);

  return model;
}

SPtr<Mesh>
GPUResourceManager::searchMesh(const String _name)
{
  for (uint32 i = 0; i < m_meshes.size(); ++i) {
    SPtr<Mesh> mesh = m_meshes[i];
    if (_name == mesh->getName()) {
      return mesh;
    }
  }
  return nullptr;
}

SPtr<Model>
GPUResourceManager::searchModel(const Path _directory)
{
  uint32 modelCount = static_cast<uint32>(m_models.size());
  // search through the loaded models.
  for (uint32 i = 0; i < modelCount; ++i) {
    SPtr<ModelMemory> modelMem = m_models[i];
    // if a model with the same directory is found, return it.
    if (modelMem->directory.toString() == _directory.toString()) {
      return modelMem->model;
    }
  }
  return nullptr;
}

void
GPUResourceManager::insertModelMemory(const SPtr<Model> _pModel, const Path _directory)
{
  SPtr<ModelMemory> modelMem = make_shared<ModelMemory>();
  modelMem->directory = _directory;
  modelMem->model = _pModel;
  m_models.push_back(modelMem);
}

PK_CORE_EXPORT GPUResourceManager&
g_GPUResourceManager()
{
  return GPUResourceManager::instance();
}
}
