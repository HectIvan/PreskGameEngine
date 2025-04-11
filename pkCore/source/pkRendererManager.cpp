#include "pkCamera.h"
#include "pkGameObject.h"
#include "pkGraphicsAPI.h"
#include "pkLogger.h"
#include "pkRendererManager.h"
#include "pkSamplerState.h"
#include "pkScene.h"
#include "pkTextureManager.h"

using TEXTURE_FORMAT::kPK_FORMAT_R32G32B32A32_FLOAT;
using PK_USAGE::kPK_USAGE_DEFAULT;
using PK_BIND_FLAG::kPK_BIND_DEPTH_STENCIL;
using PK_BIND_FLAG::kPK_BIND_RENDER_TARGET;
using PK_BIND_FLAG::kPK_BIND_SHADER_RESOURCE;

namespace pkEngineSDK
{
void RendererManager::init(Window& _window)
{
  GraphicsAPI& api = g_GraphicAPI().instance();
  // create the render targets and depth stencil view
  m_pRTargetView = api.createRenderTargetView();

  m_pDepthRT = api.createTexture(nullptr,
                                 4,
                                 _window.getWidth(),
                                 _window.getHeight(),
                                 TEXTURE_FORMAT::kPK_FORMAT_R32_TYPELESS,
                                 kPK_USAGE_DEFAULT,
                                 kPK_BIND_SHADER_RESOURCE | kPK_BIND_DEPTH_STENCIL,
                                 false,
                                 TEXTURE_FORMAT::kPK_FORMAT_R32_FLOAT);

  m_pNormalRT = api.createTexture(nullptr,
                                  4,
                                  _window.getWidth(),
                                  _window.getHeight(),
                                  kPK_FORMAT_R32G32B32A32_FLOAT,
                                  kPK_USAGE_DEFAULT,
                                  kPK_BIND_SHADER_RESOURCE | kPK_BIND_RENDER_TARGET,
                                  false,
                                  kPK_FORMAT_R32G32B32A32_FLOAT);

  m_pDepthSView = api.createDepthStencilView(m_pDepthRT);

  // create shadow mapping
  m_pShadowDepth = api.createTexture(nullptr,
                                     4,
                                     _window.getWidth(),
                                     _window.getHeight(),
                                     TEXTURE_FORMAT::kPK_FORMAT_R32_TYPELESS,
                                     kPK_USAGE_DEFAULT,
                                     kPK_BIND_SHADER_RESOURCE | kPK_BIND_DEPTH_STENCIL,
                                     false,
                                     TEXTURE_FORMAT::kPK_FORMAT_R32_FLOAT);

  m_pShadowDepthSV = api.createDepthStencilView(m_pShadowDepth);

  Vector<SPtr<Texture>> rTVector;
  rTVector.push_back(m_pRTargetView);
  rTVector.push_back(m_pNormalRT);
  rTVector.push_back(m_pDepthRT);

  // set the render targets
  api.setRenderTargets(rTVector, m_pDepthSView);// m_pDepthSView);
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
  g_GraphicAPI().setInputLayout(basePass->getInputLayout());
  // insert to the pass map.
  m_passes.insert({ 0, basePass });

  SPtr<Pass> shadowPass = make_shared<Pass>();
  // create all pointers
  shadowPass->create();
  shadowPass->setVSData(L"shaders/pkShadowMapping.hlsl", "VS", "vs_5_0");
  shadowPass->setPSData(L"shaders/pkShadowMapping.hlsl", "PS", "ps_5_0");
  shadowPass->compileShaders();
  shadowPass->createShaders();
  shadowPass->createInputLayout();
  shadowPass->createSamplerState(SAM_STATE_ADRESS::kWrap,
                                 SAM_STATE_FILTERS::kFilterMigMagMipLinear);
  m_passes.insert({ 1, shadowPass });

  //SPtr<Pass> AOPass = make_shared<Pass>();
  //// create all pointers
  //AOPass->create();
  //// set and compile shaders
  //AOPass->setVSData(L"shaders/pkDeferredShader.hlsl", "VS", "vs_5_0");
  //AOPass->setPSData(L"shaders/pkPSAOshader.hlsl", "PS", "ps_5_0");
  //AOPass->compileShaders();
  //AOPass->createShaders();
  //AOPass->createInputLayout();
  //AOPass->createSamplerState(SAM_STATE_ADRESS::kClamp,
  //                           SAM_STATE_FILTERS::kFilterMigMagMipLinear);
  //m_passes.insert({ 1, AOPass });
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
  m_CView = _pCamera->view.getTransposed();
  updateBuffer(viewBuffer, m_cBView);

  // Update projection
  CBProjection projectionBuffer = CBProjection();
  projectionBuffer.projection = _pCamera->projection.getTransposed();
  m_CProj = _pCamera->projection.getTransposed();
  updateBuffer(projectionBuffer, m_cBProjection);

  // update camera
  CBCamera cameraBuffer = CBCamera();
  cameraBuffer.eye = _pCamera->eye;
  cameraBuffer.forward = _pCamera->getForward();
  cameraBuffer.view = _pCamera->view;
  cameraBuffer.projection = _pCamera->projection;
  updateBuffer(cameraBuffer, m_cbCamera);
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

template<class T> void
RendererManager::updateBuffer(T& _data, SPtr<ConstantBuffer> _pCBuffer)
{
  g_GraphicAPI().updateConstantBuffer(_pCBuffer, &_data, 0);
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

  // set light
  light.Type = pkEngineSDK::LIGHT_TYPE::kDirectional;
  light.SpotCutoff = 1.0f;
  light.SpotExponent = 32.0f;
  light.LightDir = Vector3::FORWARD;
  light.LightPos = Vector3(0.0f, 50.0f, 0.0f);
  light.LightColor = Vector3(1.0f, 1.0f, 1.0f);

  /*
   * Shadow Map render
  api.clearRenderTargetView(clearColor, m_pRTargetView);
  api.clearDepthBuffer(1.0f, m_pShadowDepthSV);
  
  // set the base pass for the shadow rendering stage
  api.setPSShader(m_passes.find(1)->second->getPShader());
  api.setVSShader(m_passes.find(1)->second->getVShader());
  api.setSampler(m_passes.find(1)->second->getSamplerState());
  
  Camera lightCam;
  lightCam.init(30,
                17,
                3.1416f / 4.0f,
                0.01f,
                1000.0f,
                light.LightPos, // position
                light.LightDir, // target
                Vector3::UP,
                pkEngineSDK::CAMERA_PROJ::kOrthographic); // up vector);
  
  Matrix4 lightProj = lightCam.projection.getTransposed();
  Matrix4 lightView = lightCam.view.getTransposed();
  
  api.updateConstantBuffer(m_cBView, &lightView, static_cast<uint32>(sizeof(CBView)));
  api.updateConstantBuffer(m_cBProjection, &lightProj, static_cast<uint32>(sizeof(CBProjection)));
  
  // set constant buffers for the pixel and vertex shaders
  VSSetConstantBuffers();
  PSSetConstantBuffers();
  // render the objects
  renderActors(g_sceneManager().getAllActors());*/

  api.updateConstantBuffer(m_cbLight,
    &light,
    static_cast<uint32>(sizeof(Light)));
  api.clearRenderTargetView(clearColor, m_pRTargetView);
  api.clearDepthBuffer(1.0f, m_pDepthSView);

  // set the base pass for the first rendering stage
  api.setPSShader(m_passes.find(0)->second->getPShader());
  api.setVSShader(m_passes.find(0)->second->getVShader());
  api.setSampler(m_passes.find(0)->second->getSamplerState());

  api.updateConstantBuffer(m_cBView, &m_CView, static_cast<uint32>(sizeof(CBView)));
  api.updateConstantBuffer(m_cBProjection, &m_CProj, static_cast<uint32>(sizeof(CBProjection)));

  // set constant buffers for the pixel and vertex shaders
  VSSetConstantBuffers();
  PSSetConstantBuffers();
  // render the objects
  renderActors(g_sceneManager().getAllActors());
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
    // if there's a parent
    if (parent) { transform = parent->m_globalTransform * transform; }

    _gameActors[i]->m_globalTransform = transform;
    g_GraphicAPI().updateConstantBuffer(m_cBWorld,
                                        &transform,
                                        static_cast<uint32>(sizeof(CBWorld)));

    /**
     * Recast to a gameobject. If it fails, do none of this
     */
    SPtr<GameObject> gameObject = actorToClass<GameObject>(_gameActors[i]);
    renderModel(*gameObject->getComponent<Model>());
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
    // get the material
    SPtr<Material> material = _model.meshes[i]->material;
    // set the material textures to the shader
    g_GraphicAPI().PSSetShaderResourceView(material->diffuse, 0);
    g_GraphicAPI().PSSetShaderResourceView(material->normal, 1);
    g_GraphicAPI().PSSetShaderResourceView(material->height, 2);
    g_GraphicAPI().PSSetShaderResourceView(material->metallic, 3);
    g_GraphicAPI().PSSetShaderResourceView(material->occlusion, 4);
    // draw the mesh
    g_GraphicAPI().drawIndexed(static_cast<uint32>(_model.meshes[i]->numIndex),
                               currentIndexOrigin,
                               currentVertexOrigin);
    // update the offsets
    currentIndexOrigin += static_cast<uint32>(_model.meshes[i]->numIndex);
    currentVertexOrigin += static_cast<uint32>(_model.meshes[i]->vertexCount);
  }
}
PK_CORE_EXPORT RendererManager&
g_RenderManager()
{
  return RendererManager::instance();
}
}