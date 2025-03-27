#include "pkCamera.h"
#include "pkGameObject.h"
#include "pkGraphicsAPI.h"
#include "pkLogger.h"
#include "pkRendererManager.h"
#include "pkSamplerState.h"
#include "pkScene.h"

using TEXTURE_FORMAT::kPK_FORMAT_R32G32B32A32_FLOAT;
using PK_USAGE::kPK_USAGE_DEFAULT;
using PK_BIND_FLAG::kPK_BIND_DEPTH_STENCIL;
using PK_BIND_FLAG::kPK_BIND_RENDER_TARGET;
using PK_BIND_FLAG::kPK_BIND_SHADER_RESOURCE;

namespace pkEngineSDK
{
void RendererManager::init(Window& _window)
{
  // create the render targets and depth stencil view
  m_pRTargetView = g_GraphicAPI().createRenderTargetView();

  m_pDepthRT = g_GraphicAPI().createTexture(nullptr,
                                            4,
                                            _window.getWidth(),
                                            _window.getHeight(),
                                            kPK_FORMAT_R32G32B32A32_FLOAT,
                                            kPK_USAGE_DEFAULT,
                                            kPK_BIND_DEPTH_STENCIL,
                                            false);

  m_pNormalRT = g_GraphicAPI().createTexture(nullptr,
                                             4,
                                             _window.getWidth(),
                                             _window.getHeight(),
                                             kPK_FORMAT_R32G32B32A32_FLOAT,
                                             kPK_USAGE_DEFAULT,
                                             kPK_BIND_SHADER_RESOURCE | kPK_BIND_RENDER_TARGET,
                                             false);

  m_pDepthSView = g_GraphicAPI().createDepthStencilView(m_pDepthRT);
  Vector<SPtr<Texture>> rTVector;
  rTVector.push_back(m_pRTargetView);
  rTVector.push_back(m_pNormalRT);
  rTVector.push_back(m_pDepthRT);
  // set the render targets
  g_GraphicAPI().setRenderTargets(rTVector, m_pDepthSView);
  // create the passes needed
  createPasses();
}

void
RendererManager::createPasses()
{
  /**
   * Create the base pass.
   */
  SPtr<Pass> basePass = make_shared<Pass>();
  // create all pointers
  basePass->create();
  // set the data for the shaders to be compiled, and compile.
  basePass->setVSData(L"shaders/pkVShader.hlsl", "VS", "vs_5_0");
  basePass->setPSData(L"shaders/pkVShader.hlsl", "PS", "ps_5_0");
  basePass->compileShaders();
  basePass->createShaders();
  // create the vertex shader input layout && sampler state.
  basePass->createInputLayout();
  basePass->createSamplerState(SAM_STATE_ADRESS::kWrap,
                               SAM_STATE_FILTERS::kFilterMigMagMipLinear);
  basePass->setInputLayout();
  // insert to the pass map.
  m_passes.insert({ 0, basePass });
}

void
RendererManager::compileShaders()
{
  Map<uint32, SPtr<Pass>>::iterator it;
  for (it = m_passes.begin(); it != m_passes.end(); ++it) {
    // Compile shaders
    it->second->compileShaders();
    it->second->createShaders();
    g_Logger().print("recompiled shaders.");
  }
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

  // update camera
  CBCamera cameraBuffer = CBCamera();
  cameraBuffer.eye = _pCamera->eye;
  cameraBuffer.forward = _pCamera->getForward();
  api.updateConstantBuffer(m_cbCamera, &cameraBuffer, 0);
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
  GraphicsAPI& api = g_GraphicAPI().instance();
  // set the constant buffers
  api.VSSetConstantBuffer(m_cBView, 0, 1);
  api.VSSetConstantBuffer(m_cBProjection, 1, 1);
  api.VSSetConstantBuffer(m_cBWorld, 2, 1);
  api.VSSetConstantBuffer(m_cbLight, 3, 1);
  api.VSSetConstantBuffer(m_cbCamera, 4, 1);
}

void
RendererManager::PSSetConstantBuffers()
{
  GraphicsAPI& api = g_GraphicAPI().instance();
  // set the constant buffers
  api.PSSetConstantBuffer(m_cBView, 0, 1);
  api.PSSetConstantBuffer(m_cBProjection, 1, 1);
  api.PSSetConstantBuffer(m_cBWorld, 2, 1);
  api.PSSetConstantBuffer(m_cbLight, 3, 1);
  api.PSSetConstantBuffer(m_cbCamera, 4, 1);
}

void
RendererManager::setActorsBuffers()
{
  // for each actor in the world
  for (uint32 i = 0; i < g_sceneManager().getAllActors().size(); ++i) {
    // Cast to a gameObject, if it fails, do none of the following process
    SPtr<Actor> actor = g_sceneManager().getActor(i);
    SPtr<GameObject> gameObject = actorToClass<GameObject>(actor);
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
RendererManager::render()
{
  // screen clear color
  float clearColor[4] = { 0.0f, 0.123f, 0.3f, 1.0f };
  GraphicsAPI& api = g_GraphicAPI().instance();
  RendererManager& renderer = g_RenderManager().instance();

  api.clearRenderTargetView(clearColor, m_pRTargetView);
  api.clearDepthBuffer(1.0f, m_pDepthSView);


  Map<uint32, SPtr<Pass>>::iterator it;
  for (it = m_passes.begin(); it != m_passes.end(); ++it) {
    // Set shaders
    api.setPSShader(it->second->getPShader());
    api.setVSShader(it->second->getVShader());
  }
  // set light
  renderer.light.Type = pkEngineSDK::LIGHT_TYPE::kDirectional;
  renderer.light.SpotCutoff = 1.0f;
  renderer.light.SpotExponent = 32.0f;
  renderer.light.LightDir = Vector3::FORWARD;
  renderer.light.LightPos = Vector3(0.0f, 50.0f, 0.0f);
  renderer.light.LightColor = Vector3(1.0f, 1.0f, 1.0f);
  // update the light buffer
  api.updateConstantBuffer(m_cbLight,
                           &light,
                           static_cast<uint32>(sizeof(Light)));
  // set constant buffers for the pixel and vertex shaders
  renderer.VSSetConstantBuffers();
  renderer.PSSetConstantBuffers();
  // render the objects
  renderer.renderActors(g_sceneManager().getAllActors());
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
        g_GraphicAPI().PSSetShaderResourceView(material->diffuse, 0);
        g_GraphicAPI().PSSetShaderResourceView(material->normal, 1);
        g_GraphicAPI().PSSetShaderResourceView(material->height, 2);
        g_GraphicAPI().PSSetShaderResourceView(material->metallic, 3);
        g_GraphicAPI().PSSetShaderResourceView(material->occlusion, 4);

        // get the basic bass sampler and set it to the pixel shader
        g_GraphicAPI().setSampler(m_passes.begin()->second->getSamplerState());
        // Map<uint32, SPtr<Pass>>::iterator it;
        // for (it = m_passes.begin(); it != m_passes.end(); ++it) {
        //   // Set sampler
        //   g_GraphicAPI().setSampler(it->second->getSamplerState());
        // }
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