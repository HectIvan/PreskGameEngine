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
  
}

SPtr<Model>
GPUResourceManager::loadPKModel(const UUID& _ID)
{
  // check if the model has already been stored.
  SPtr<Model> model = getModelMemory(_ID);
  if (model) {
    return model;
  }

  GraphicsAPI& api = g_GraphicAPI();
  AssetResourceManager& assetResMgr = g_AssetResourceManager();

  // get the model resource.
  auto resource = assetResMgr.getResource(_ID);
  if (resource->getType() != RESOURCE_TYPE::kModel) {
    return nullptr;
  }
  auto modelRes = reinterpret_pointer_cast<ModelResource>(resource);
  modelRes->load();
  assetResMgr.insertLoadedResource(modelRes);

  // check if the resource is a valid model resource.
  if (!modelRes) {
    const String msg = "Failed to load model resource of ID: " +
                       resource->m_id.toString() +
                       ".";
    LOG_WARNING(msg, __FILE__, __LINE__);
    return nullptr;
  }

  // save the model data.
  model = make_shared<Model>();
  model->setName(modelRes->m_name);
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
GPUResourceManager::insertModelMemory(const UUID& _ID, const SPtr<Model>& _pModel)
{
  m_models.insert({ _ID, _pModel });
}

SPtr<Model>
GPUResourceManager::getModelMemory(const UUID& _ID)
{
  for (auto& model : m_models) {
    if (_ID.toString() == model.first.toString()) { // to do: make a == operator
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
