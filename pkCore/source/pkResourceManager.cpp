#include "pkResourceManager.h"
#include "pkGraphicsAPI.h"

namespace pkEngineSDK
{

SPtr<Material>
ResourceManager::newMaterial()
{
  // create the material component.
  SPtr<Material> pMatComp = make_shared<Material>();
  // return the material.
  return pMatComp;
}

SPtr<Model>
ResourceManager::loadModel(Path _directory)
{
  // search if the model has been stored before
  for (uint32 i = 0; i < m_models.size(); ++i) {
    if (m_models[i]->directory.toString() == _directory.toString()) {
      return m_models[i]->model;
    }
  }

  // create the model pointer
  SPtr<Model> model = make_shared<Model>();
  // load the model from the path
  model->load(_directory);
  if (!model->index.empty()) {
    // create the index and vertex buffers
    model->m_vertexB = g_GraphicAPI().createVertexBuffer(model->vertex);
    model->m_indexB = g_GraphicAPI().createIndexBuffer(model->index);
    g_GraphicAPI().setIndexBuffer(model->m_indexB);
    g_GraphicAPI().setVertexBuffer(model->m_vertexB);
  }
  SPtr<ModelMemory> newModelMem = make_shared<ModelMemory>();
  newModelMem->directory = _directory;
  newModelMem->model = model;
  m_models.push_back(newModelMem);
  // return the final model
  return model;
}

PK_CORE_EXPORT ResourceManager&
g_ResourceManager()
{
  return ResourceManager::instance();
}
}