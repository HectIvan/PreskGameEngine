#include "pkCamera.h"
#include "pkGameObject.h"
#include "pkGraphicsAPI.h"
#include "pkRendererManager.h"
#include "pkSamplerState.h"

namespace pkEngineSDK
{
void
RendererManager::createPasses()
{
  /**
   * Create the base pass.
   */
  SPtr<Pass> basePass;
  basePass->create();
  // set the data for the shaders to be compiled, and compile.
  basePass->setVSData(L"shaders/pkVShader.hlsl", "VS", "vs_5_0");
  basePass->setPSData(L"shaders/pkPShader.hlsl", "PS", "ps_5_0");
  basePass->compileShaders();
  // create the vertex shader input layout && sampler state.
  basePass->createInputLayout();
  basePass->createSamplerState(SAM_STATE_ADRESS::kWrap,
                               SAM_STATE_FILTERS::kFilterMigMagMipLinear);
  basePass->setInputLayout();
  // insert to the pass map.
  m_passes.insert({ 0, basePass });
}

void
RendererManager::updateCameraBuffers(Camera* _pCamera)
{
  // get the api instance to work with
  GraphicsAPI& api = GraphicsAPI::instance();

  // Update view
  CBView viewBuffer = CBView();
  viewBuffer.view = _pCamera->view.getTransposed();
  api.updateConstantBuffer(m_cBView, &viewBuffer, 0);

  // Update projection
  CBProjection projectionBuffer = CBProjection();
  projectionBuffer.projection = _pCamera->projection.getTransposed();
  api.updateConstantBuffer(m_cBProjection, &projectionBuffer, 0);
}

template<typename T>
SPtr<T>
RendererManager::actorToClass(SPtr<Actor>& _subject)
{
  // cast to a game actor
  SPtr<T> aTC = reinterpret_pointer_cast<T>(_subject);
  // casting was successful
  if (aTC) { return aTC; }
  // casting failed
  return nullptr;
}
  
void
RendererManager::VSSetConstantBuffers()
{
  // set the constant buffers
  g_GraphicAPI().VSSetConstantBuffer(m_cBView, 0, 1);
  g_GraphicAPI().VSSetConstantBuffer(m_cBProjection, 1, 1);
  g_GraphicAPI().VSSetConstantBuffer(m_cBWorld, 2, 1);
  g_GraphicAPI().VSSetConstantBuffer(m_cbLight, 3, 1);
}

void
RendererManager::PSSetConstantBuffers()
{
  // set the constant buffers
  g_GraphicAPI().PSSetConstantBuffer(m_cBView, 0, 1);
  g_GraphicAPI().PSSetConstantBuffer(m_cBProjection, 1, 1);
  g_GraphicAPI().PSSetConstantBuffer(m_cBWorld, 2, 1);
  g_GraphicAPI().PSSetConstantBuffer(m_cbLight, 3, 1);
}

void
RendererManager::setActorsBuffers(Scene& _scene)
{
  // for each actor in the world
  for (uint32 i = 0; i < _scene.m_actors.size(); ++i) {
    // Cast to a gameObject, if it fails, do none of the following process
    SPtr<GameObject> gameObject = actorToClass<GameObject>(_scene.m_actors[i]);
    if (gameObject) {
      // for each model in the game object
      for (uint32 j = 0; j < gameObject->models.size(); ++j) {
        // set its vertex and index buffers
        g_GraphicAPI().setVertexBuffer(gameObject->models[j]->vertexB);
        g_GraphicAPI().setIndexBuffer(gameObject->models[j]->indexB); ;
      }
    }
  }
}

void
RendererManager::renderActors(Vector<SPtr<Actor>> _gameActors)
{
  // for each actor
  for (uint32 i = 0; i < _gameActors.size(); ++i) {
    if (!_gameActors[i]->m_active) {
      continue;
    }
    // Get the final matrix by taking into account the parent actors
    SPtr<Actor> parent = _gameActors[i]->m_parent;
    Matrix4 transform = _gameActors[i]->m_transform;
    // while there's a parent
    while (parent) {
      // add the parent transform to the current transform matrix
      transform *= parent->m_transform;
      // the next parent will be the parent of this parent
      parent = parent->m_parent;
    }
    // set the current actor transform as the world in which the shader will work in
    g_GraphicAPI().updateConstantBuffer(m_cBWorld, &transform, static_cast<uint32>(sizeof(CBWorld)));

    /**
     * Recast to a gameobject. If it fails, do none of this
     */
    SPtr<GameObject> gameObject = actorToClass<GameObject>(_gameActors[i]);
    if (gameObject) {
      // set the diffuse texture to the resource view if the model has a material
      if (gameObject->getComponent<Material>()) {
        // get the material
        SPtr<Material> material = gameObject->getComponent<Material>();
        // set the material textures to the shader
        g_GraphicAPI().setShaderResourceView(material->diffuse, 0);
        g_GraphicAPI().setShaderResourceView(material->normal, 1);
        g_GraphicAPI().setShaderResourceView(material->height, 2);
        g_GraphicAPI().setShaderResourceView(material->metallic, 3);
        g_GraphicAPI().setShaderResourceView(material->occlusion, 4);
        g_GraphicAPI().setSampler();
      }
      // render the model component
      renderModel(*gameObject->getComponent<Model>());
    }
    // if the actor has children, do the same for them
    if (!_gameActors[i]->m_children.empty()) {
      renderActors(_gameActors[i]->m_children);
    }
  }
}


void
RendererManager::renderModel(Model& _model)
{
  // get a reference from the api
  // GraphicsAPI& api = GraphicsAPI::instance();
  // g_GraphicAPI().createInputLayoutFromVShader();
  g_GraphicAPI().setVertexBuffer(_model.vertexB);
  g_GraphicAPI().setIndexBuffer(_model.indexB);
  // offsets
  uint32 currentVertexOrigin = 0;
  uint32 currentIndexOrigin = 0;
  // for each mesh in the model
  for (uint32 i = 0; i < _model.meshes.size(); ++i) {
    // draw the mesh
    g_GraphicAPI().drawIndexed(static_cast<uint32>(_model.meshes[i].numIndex),
                               currentIndexOrigin,
                               currentVertexOrigin);
    // update the offsets
    currentIndexOrigin += static_cast<uint32>(_model.meshes[i].numIndex);
    currentVertexOrigin += static_cast<uint32>(_model.meshes[i].vertexCount);
  }
}
PK_CORE_EXPORT RendererManager&
g_RenderManager()
{
  return RendererManager::instance();
}
}