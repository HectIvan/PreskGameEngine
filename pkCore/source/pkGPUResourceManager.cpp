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
GPUResourceManager::loadPKModel(Path _path)
{
  GraphicsAPI& api = g_GraphicAPI();
  AssetResourceManager& assetResMgr = g_AssetResourceManager();

  SPtr<Model> model = make_shared<Model>();
  ModelResource* modelRes = assetResMgr.loadModelResource(_path);

  model->index = modelRes->m_index;
  model->vertex = modelRes->m_vertex;
  model->meshes = modelRes->m_meshes;

  model->setVerticesIndices();

  model->m_vertexB = api.createVertexBuffer(model->vertex);
  model->m_indexB = api.createIndexBuffer(model->index);
  api.setIndexBuffer(model->m_indexB);
  api.setVertexBuffer(model->m_vertexB);

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

PK_CORE_EXPORT GPUResourceManager&
g_GPUResourceManager()
{
  return GPUResourceManager::instance();
}
}
