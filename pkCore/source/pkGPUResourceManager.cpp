#include "pkGPUResourceManager.h"
#include "pkGraphicsAPI.h"
#include "pkLogger.h"

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
GPUResourceManager::loadModel(Path _directory)
{
  GraphicsAPI& api = g_GraphicAPI();

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

  // load the model from the path depending on extension.
  if (_directory.getExtension() == "pkm") {
    if (!model->loadPK(_directory)) {
      model = nullptr;
      return nullptr;
    }
  }
  else if (!model->loadAssimp(_directory)) {
    model = nullptr;
    return nullptr;
  }

  // create the index and vertex buffers
  model->m_vertexB = api.createVertexBuffer(model->vertex);
  model->m_indexB = api.createIndexBuffer(model->index);
  api.setIndexBuffer(model->m_indexB);
  api.setVertexBuffer(model->m_vertexB);

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
