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
GPUResourceManager::loadPKModel(const String& _ID)
{
  // check if the model has already been stored.
  SPtr<Model> model = getModelMemory(_ID);
  if (model) {
    return model;
  }

  GraphicsAPI& api = g_GraphicAPI();
  AssetResourceManager& assetResMgr = g_AssetResourceManager();
  Logger& log = g_Logger();

  // get the model resource.
  auto resource = assetResMgr.getResource(_ID);
  auto modelRes = reinterpret_pointer_cast<ModelResource>(resource);
  modelRes->load();
  assetResMgr.insertLoadedResource(modelRes);

  // check if the resource is a valid model resource.
  if (!modelRes) {
    const String msg = "Failed to load model resource of ID: " + resource->m_id + ".";
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kWarning);
    return nullptr;
  }

  // save the model data.
  model = make_shared<Model>();
  model->setName(modelRes->m_name.c_str());
  model->index = modelRes->m_index;
  model->vertex = modelRes->m_vertex;
  model->meshes = modelRes->m_meshes;

  model->setVerticesIndices();

  model->m_vertexB = api.createVertexBuffer(model->vertex);
  model->m_indexB = api.createIndexBuffer(model->index);
  api.setIndexBuffer(model->m_indexB);
  api.setVertexBuffer(model->m_vertexB);

  insertModelMemory(modelRes->m_id, model);

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

void
GPUResourceManager::insertModelMemory(const String& _ID, const SPtr<Model>& _pModel)
{
  m_models.insert({ _ID, _pModel });
}

SPtr<Model>
GPUResourceManager::getModelMemory(const String& _ID)
{
  for (auto& model : m_models) {
    if (_ID == model.first) {
      return model.second;
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
