#include "pkDX11GraphicsAPI.h"
#include "pkDX11Prerequisites.h"
#include "pkDX11VertexBuffer.h"
#include "pkWindow.h"

#if PK_PLATFORM == PK_PLATFORM_WIN32

namespace pkEngineSDK
{

void
DX11GraphicsAPI::init(const Window& _window)
{
  m_vMeshColor = Vector4(0.7f, 0.7f, 0.7f, 1.0f);

  uint32 createDeviceFlags = 0;
#ifdef _DEBUG
  createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // _DEBUG


  // graphics api (do not store)
  D3D_DRIVER_TYPE driverTypes[] =
  {
    D3D_DRIVER_TYPE_HARDWARE,
    D3D_DRIVER_TYPE_WARP,
    D3D_DRIVER_TYPE_REFERENCE,
  };

  D3D_FEATURE_LEVEL featureLevels[] =
  {
    D3D_FEATURE_LEVEL_11_0,
    D3D_FEATURE_LEVEL_10_1,
    D3D_FEATURE_LEVEL_10_0,
  };
  uint32 numDriverTypes = ARRAYSIZE(driverTypes);
  uint32 numFeatureLevels = ARRAYSIZE(featureLevels);

  WindowHandle winHandle = m_window.getWindowHandle();
  uint32 width = static_cast<uint32>(_window.getSize().x);
  uint32 height = static_cast<uint32>(_window.getSize().y);
  createDeviceAndSwapChain(width,
                           height,
                           winHandle,
                           numDriverTypes,
                           driverTypes,
                           createDeviceFlags,
                           featureLevels,
                           numFeatureLevels);

  createRenderTargetView();
  createDepthStencilTexture(width, height);
  setViewport(width, height);
}

void
DX11GraphicsAPI::render()
{
  static float t = 0.0f;
  if (*m_pDevice->m_pDriverType == D3D_DRIVER_TYPE_REFERENCE)
  {
    t += 3.1416f * 0.0125f;
  }
  else
  {
    static DWORD dwTimeStart = 0;
    DWORD dwTimeCur = static_cast<DWORD>(GetTickCount64());
    if (dwTimeStart == 0)
      dwTimeStart = dwTimeCur;
    t = (dwTimeCur - dwTimeStart) / 1000.0f;
  }

  // Modify the color
  m_vMeshColor.x = (sinf(t * 1.0f) + 1.0f) * 0.5f;
  m_vMeshColor.y = (cosf(t * 3.0f) + 1.0f) * 0.5f;
  m_vMeshColor.z = (sinf(t * 5.0f) + 1.0f) * 0.5f;

  // screen clear color
  float clearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red, green, blue, alpha
  // clear the back buffer and the depth buffer
  clearDepthBackBuffers(clearColor, 1.0f);
  
  // update world and light constant buffers
  CBWorld ef;
  ef.mWorld = m_world;
  ef.vMeshColor = m_vMeshColor;
  m_cBWorld.updateSubResource(m_pDevice, &ef, (uint32)sizeof(CBWorld));
  m_LightCB.updateSubResource(m_pDevice, m_light, (uint32)sizeof(*m_light));

  // Render the Game Objects
  setShaders();
  // for each Game Oobject
  for (uint32 i = 0; i < gameObjects.size(); ++i)
  {
    // check all their models
    for (uint32 j = 0; j < gameObjects[i].m_models.size(); ++j)
    {
      // set the vertex and index buffers of the models
      setVertexBuffers(*gameObjects[i].m_models[j]);
      setIndexBuffers(*gameObjects[i].m_models[j]);
    }
  }
  VSSetConstantBuffers();
  PSSetConstantBuffers();
  m_pDevice->m_pImmediateContext->PSSetSamplers(0, 1, &m_pSamplerLinear->m_pSampler);
  // for each Game Object
  for (uint32 i = 0; i < gameObjects.size(); ++i)
  {
    // check all their models
    for (uint32 j = 0; j < gameObjects[i].m_models.size(); ++j)
    {
      // draw the model
      drawIndexed(*gameObjects[i].m_models[j]);
    }
  }
  // Present our back buffer to our front buffer
  m_pSwapChain->Present(1, 0);
}

void
DX11GraphicsAPI::createDeviceAndSwapChain(uint32 _width,
                                          uint32 _height,
                                          WindowHandle& _wHnd,
                                          uint32 _numDriverTypes,
                                          D3D_DRIVER_TYPE _driverTypes[],
                                          uint32 _createDeviceFlags,
                                          D3D_FEATURE_LEVEL _featureLevels[],
                                          uint32 _numFeatureLevels)
{
  // initialize device and swap chain
  m_pDevice = new DX11Device();
  m_pSwapChain = nullptr;

  /**
  * Create the device and swap chains
  **/
  DXGI_SWAP_CHAIN_DESC sd;
  ZeroMemory(&sd, sizeof(sd));
  sd.BufferCount = 1;
  sd.BufferDesc.Width = _width;
  sd.BufferDesc.Height = _height;
  sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  sd.BufferDesc.RefreshRate.Numerator = 60;
  sd.BufferDesc.RefreshRate.Denominator = 1;
  sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  sd.OutputWindow = _wHnd;
  sd.SampleDesc.Count = 1;
  sd.SampleDesc.Quality = 0;
  sd.Windowed = true;
  for (uint32 driverTypeIndex = 0; driverTypeIndex < _numDriverTypes; driverTypeIndex++)
  {
    // try and create the device and swap chain with the current driver type
    m_pDevice->m_pDriverType = new D3D_DRIVER_TYPE(_driverTypes[driverTypeIndex]);
    uint32 hr = D3D11CreateDeviceAndSwapChain(nullptr,
      *m_pDevice->m_pDriverType,
      nullptr,
      _createDeviceFlags,
      _featureLevels,
      _numFeatureLevels,
      D3D11_SDK_VERSION,
      &sd,
      &m_pSwapChain,
      &m_pDevice->m_pd3dDevice,
      &m_pDevice->m_featureLevel,
      &m_pDevice->m_pImmediateContext);

    // if creation was successful
    if (hr == 0x00000000)
    {
      // end the entire process, no need to continue
      break;
    }
  }
}

void
DX11GraphicsAPI::createRenderTargetView()
{
  // initiaize render target view
  m_pRTargetView = nullptr;

  // get buffer data
  ID3D11Texture2D* pBackBuffer = nullptr;
  uint32 hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
  if (hr != 0x00000000)
  {
    return;
  }

  // create the render target view
  hr = m_pDevice->m_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pRTargetView);
  pBackBuffer->Release();
  if (hr != 0x00000000)
  {
    return;
  }
}

void
DX11GraphicsAPI::createSamplerState()
{
  // sampler state description
  D3D11_SAMPLER_DESC sampDesc;
  ZeroMemory(&sampDesc, sizeof(sampDesc));
  sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
  sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
  sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
  sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
  sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
  sampDesc.MinLOD = 0;
  sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

  // sampler state creation
  uint32 hr = m_pDevice->m_pd3dDevice->CreateSamplerState(&sampDesc, &m_pSamplerLinear->m_pSampler);
  if (hr != 0x00000000)
  {
    return;
  }
}

void
DX11GraphicsAPI::createDepthStencilTexture(uint32 _width,
                                           uint32 _height)
{
  /**
  * Create depth stencil view
  **/
  D3D11_TEXTURE2D_DESC descDepth;
  ZeroMemory(&descDepth, sizeof(descDepth));
  descDepth.Width = _width;
  descDepth.Height = _height;
  descDepth.MipLevels = 1;
  descDepth.ArraySize = 1;
  descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
  descDepth.SampleDesc.Count = 1;
  descDepth.SampleDesc.Quality = 0;
  descDepth.Usage = D3D11_USAGE_DEFAULT;
  descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
  descDepth.CPUAccessFlags = 0;
  descDepth.MiscFlags = 0;
  uint32 hr = m_pDevice->m_pd3dDevice->CreateTexture2D(&descDepth, nullptr, &m_pDepthStencil);
  // if creating the texture failed
  if (hr != 0x00000000)
  {
    return;
  }

  // create depth stencil with the generated 2D texture
  m_pDepthSView = std::make_shared<DX11DepthStencilView>();
  D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
  ZeroMemory(&descDSV, sizeof(descDSV));
  descDSV.Format = descDepth.Format;
  descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
  descDSV.Texture2D.MipSlice = 0;
  hr = m_pDevice->m_pd3dDevice->CreateDepthStencilView(m_pDepthStencil, &descDSV, &m_pDepthSView->m_pDepthSV);
  if (hr != 0x00000000)
  {
    return;
  }
  m_pDevice->m_pImmediateContext->OMSetRenderTargets(1, &m_pRTargetView, m_pDepthSView->m_pDepthSV);
}

void
DX11GraphicsAPI::setViewport(uint32 _width,
                             uint32 _height)
{
  D3D11_VIEWPORT vp;
  vp.Width = static_cast<float>(_width);
  vp.Height = static_cast<float>(_height);
  vp.MinDepth = 0.0f;
  vp.MaxDepth = 1.0f;
  vp.TopLeftX = 0;
  vp.TopLeftY = 0;
  m_pDevice->m_pImmediateContext->RSSetViewports(1, &vp);
  m_world = Matrix4::IDENTITY;
}

void
DX11GraphicsAPI::setVertexBuffers(Model& _model)
{
  _model.vertexB->set(m_pDevice);
}

void DX11GraphicsAPI::setIndexBuffers(Model& _model)
{
  _model.indexB->set(m_pDevice);
}

void
DX11GraphicsAPI::setShaders()
{
  m_pDevice->m_pImmediateContext->VSSetShader(m_vertexShader.m_pShader, nullptr, 0);
  m_pDevice->m_pImmediateContext->PSSetShader(m_pixelShader.m_pShader, nullptr, 0);
}

void
DX11GraphicsAPI::VSSetConstantBuffers()
{
  m_pDevice->m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_cBView.m_pCBuffer);
  m_pDevice->m_pImmediateContext->VSSetConstantBuffers(1, 1, &m_cBProjection.m_pCBuffer);
  m_pDevice->m_pImmediateContext->VSSetConstantBuffers(2, 1, &m_cBWorld.m_pCBuffer);
  m_pDevice->m_pImmediateContext->VSSetConstantBuffers(3, 1, &m_LightCB.m_pCBuffer);
}

void
DX11GraphicsAPI::PSSetConstantBuffers()
{
  m_pDevice->m_pImmediateContext->PSSetConstantBuffers(0, 1, &m_cBView.m_pCBuffer);
  m_pDevice->m_pImmediateContext->PSSetConstantBuffers(1, 1, &m_cBProjection.m_pCBuffer);
  m_pDevice->m_pImmediateContext->PSSetConstantBuffers(2, 1, &m_cBWorld.m_pCBuffer);
  m_pDevice->m_pImmediateContext->PSSetConstantBuffers(3, 1, &m_LightCB.m_pCBuffer);
}

void
DX11GraphicsAPI::clearDepthBackBuffers(float _color[], float _depth)
{
  m_pDevice->m_pImmediateContext->ClearRenderTargetView(m_pRTargetView, _color);
  // Clear the depth buffer to 1.0 (max depth)
  m_pDevice->m_pImmediateContext->ClearDepthStencilView(m_pDepthSView->m_pDepthSV, D3D11_CLEAR_DEPTH, _depth, 0);
}

void
DX11GraphicsAPI::drawIndexed(Model& model)
{
  // offsets
  uint32 currentVertexOrigin = 0;
  uint32 currentIndexOrigin = 0;
  // for each mesh in the model
  for (uint32 i = 0; i < model.meshes.size(); ++i)
  {
    // draw the mesh
    m_pDevice->m_pImmediateContext->DrawIndexed(static_cast<uint32>(model.meshes[i].numIndex), currentIndexOrigin, currentVertexOrigin);
    // update the offsets
    currentIndexOrigin += static_cast<uint32>(model.meshes[i].numIndex);
    currentVertexOrigin += static_cast<uint32>(model.meshes[i].vertexCount);
  }
}
}

#endif