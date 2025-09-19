#include "pkGPUResourceManager.h"
#include "pkGraphicsAPI.h"
#include "pkLogger.h"

namespace pkEngineSDK
{

SPtr<Material>
GPUResourceManager::newMaterial()
{
  // create the material component.
  SPtr<Material> pMatComp = make_shared<Material>();
  // return the material.
  return pMatComp;
}

SPtr<Model>
GPUResourceManager::loadModel(Path _directory)
{
  GraphicsAPI& api = g_GraphicAPI().instance();

  // Get model directory
  String dir = _directory.toString();
  // search if the model has been stored before
  for (uint32 i = 0; i < m_models.size(); ++i) {
    if (m_models[i]->directory.toString() == dir) {
      return m_models[i]->model;
      g_Logger().registerMessage("Found pre-loaded model of directory " + dir + ".");
    }
  }

  // create the model pointer
  SPtr<Model> model = make_shared<Model>();
  // load the model from the path
  if (model->load(_directory)) {
    // create the index and vertex buffers
    model->m_vertexB = api.createVertexBuffer(model->vertex);
    model->m_indexB = api.createIndexBuffer(model->index);
    api.setIndexBuffer(model->m_indexB);
    api.setVertexBuffer(model->m_vertexB);
  }
  else { // if the model could not be loaded, destroy the pointer and return null.
    model = nullptr;
    return nullptr;
  }

  // store the model in memory for later use if needed
  SPtr<ModelMemory> newModelMem = make_shared<ModelMemory>();
  newModelMem->directory = _directory;
  newModelMem->model = model;
  m_models.push_back(newModelMem);
  // return the final model
  return model;
}

SPtr<Mesh>
GPUResourceManager::searchMesh(String _name)
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
