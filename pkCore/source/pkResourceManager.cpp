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

SPtr<Texture>
ResourceManager::newTexture(String _name, String _directory)
{
  // create the texture adress.
  String textureName = _directory + _name;
  return g_GraphicAPI().createTextureFromFile(textureName, 8, false, 28);
}

SPtr<Model>
ResourceManager::loadModel(String& _fileName)
{
  // create the model path
  String modelPath = "models/" + _fileName;
  // create the model pointer
  SPtr<Model> model = make_shared<Model>();
  // load the model from the path
  model->load(modelPath);
  if (!model->index.empty()) {
    // create the index and vertex buffers
    model->vertexB = g_GraphicAPI().createVertexBuffer(model->vertex);
    model->indexB = g_GraphicAPI().createIndexBuffer(model->index);
    g_GraphicAPI().setIndexBuffer(model->indexB);
    g_GraphicAPI().setVertexBuffer(model->vertexB);
  }
  // return the final model
  return model;
}

PK_CORE_EXPORT ResourceManager&
g_ResourceManager()
{
  return ResourceManager::instance();
}
}