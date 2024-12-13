#include "pkDX11GraphicsAPI.h"
#include "pkDX11IndexBuffer.h"
#include "pkDX11Prerequisites.h"
#include "pkDX11VertexBuffer.h"
#include "pkWindow.h"
#include "pkVertexBuffer.h"

#include "pkDX11InputLayout.h"
#include "pkCamera.h"

#if PK_PLATFORM == PK_PLATFORM_WIN32
#include <d3dcompiler.h>

namespace pkEngineSDK
{

extern "C" __declspec(dllexport) void
loadPlugin(const Window& _window)
{
  GraphicsAPI::startUp<DX11GraphicsAPI>(_window);
}

DX11GraphicsAPI::DX11GraphicsAPI(const Window& _window)
{
  initApi(_window);
}

void
DX11GraphicsAPI::initApi(const Window& _window)
{
  window = _window;

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

  WindowHandle winHandle = window.getWindowHandle();
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

  /************************************************/
  /**
  * TEMPORARY, THIS WILL BE SWAPPED TO THE BASE APP
  **/
  /************************************************/

  pixelShader.compile();
  pixelShader.create(m_pDevice);
  vertexShader.compile();
  vertexShader.create(m_pDevice);

  DX11InputLayout input;
  input.create(m_pDevice, vertexShader);
  input.set(m_pDevice);

  m_pDevice->setPrimitiveTopology();

  light.Type = LIGHT_TYPE::kDirectional;
  light.LightDir = Vector3::FORWARD;
  cBView.create(m_pDevice, static_cast<uint32>(sizeof(CBView)));
  cBProjection.create(m_pDevice, static_cast<uint32>(sizeof(CBProjection)));
  cBWorld.create(m_pDevice, static_cast<uint32>(sizeof(CBWorld)));
  cbLight.create(m_pDevice, static_cast<uint32>(sizeof(Light)));

  createSamplerState();
}

void
DX11GraphicsAPI::drawIndexed(uint32 _indexCount,
                             uint32 _startIndexLocation,
                             uint32 _baseVertexLocation)
{
  m_pDevice->pImmediateContext->DrawIndexed(_indexCount,
                                            _startIndexLocation,
                                            _baseVertexLocation);
}

void
DX11GraphicsAPI::updateCamera(Camera* _pCamera)
{
  /*****************/
  /**
  * Update view
  **/
  /*****************/
  CBView viewBuffer = CBView();
  viewBuffer.view = _pCamera->view.getTransposed();
  m_pDevice->pImmediateContext->UpdateSubresource(cBView.pCBuffer,
                                                  0,
                                                  nullptr,
                                                  &viewBuffer,
                                                  0,
                                                  0);

  /*****************/
  /**
  * Update projection
  **/
  /*****************/
  CBProjection projectionBuffer = CBProjection();
  projectionBuffer.projection = _pCamera->projection.getTransposed();
  m_pDevice->pImmediateContext->UpdateSubresource(cBProjection.pCBuffer,
                                                0,
                                                nullptr,
                                                &projectionBuffer,
                                                0,
                                                0);
}

void
DX11GraphicsAPI::clearRenderTargetView(float _color[])
{
  m_pDevice->pImmediateContext->ClearRenderTargetView(pRTargetView, _color);
}

void
DX11GraphicsAPI::clearDepthBuffer(float _depth)
{
  // Clear the depth buffer to 1.0 (max depth)
  m_pDevice->pImmediateContext->ClearDepthStencilView(pDepthSView->pDepthSV,
                                                      D3D11_CLEAR_DEPTH,
                                                      _depth,
                                                      0);
}

void
DX11GraphicsAPI::updateWorldAndLightCB()
{
  CBWorld ef;
  ef.world = world;
  cBWorld.updateSubResource(m_pDevice, &ef, (uint32)sizeof(CBWorld));
  cbLight.updateSubResource(m_pDevice, &light, (uint32)sizeof(Light));
}

void
DX11GraphicsAPI::createDeviceAndSwapChain(uint32& _width,
                                          uint32& _height,
                                          WindowHandle& _wHnd,
                                          uint32& _numDriverTypes,
                                          D3D_DRIVER_TYPE _driverTypes[],
                                          uint32& _createDeviceFlags,
                                          D3D_FEATURE_LEVEL _featureLevels[],
                                          uint32& _numFeatureLevels)
{
  // initialize device and swap chain
  m_pDevice = new DX11Device();
  pSwapChain = nullptr;

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
    m_pDevice->pDriverType = new D3D_DRIVER_TYPE(_driverTypes[driverTypeIndex]);
    uint32 hr = D3D11CreateDeviceAndSwapChain(nullptr,
                                              *m_pDevice->pDriverType,
                                              nullptr,
                                              _createDeviceFlags,
                                              _featureLevels,
                                              _numFeatureLevels,
                                              D3D11_SDK_VERSION,
                                              &sd,
                                              &pSwapChain,
                                              &m_pDevice->pd3dDevice,
                                              &m_pDevice->featureLevel,
                                              &m_pDevice->pImmediateContext);

    // if creation was successful
    if (hr == 0x00000000)
    {
      // auto err(hr);
      // LPCTSTR errMsg = err.ErrorMessage();
      // end the entire process, no need to continue
      break;
    }
  }
}

void
DX11GraphicsAPI::createRenderTargetView()
{
  // initiaize render target view
  pRTargetView = nullptr;

  // get buffer data
  ID3D11Texture2D* pBackBuffer = nullptr;
  uint32 hr = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
  if (hr != 0x00000000)
  {
    return;
  }

  // create the render target view
  hr = m_pDevice->pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &pRTargetView);
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
  pSamplerLinear = new DX11SamplerState();
  uint32 hr = m_pDevice->pd3dDevice->CreateSamplerState(&sampDesc, &pSamplerLinear->pSampler);
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
  uint32 hr = m_pDevice->pd3dDevice->CreateTexture2D(&descDepth, nullptr, &pDepthStencil);
  // if creating the texture failed
  if (hr != 0x00000000)
  {
    return;
  }

  // create depth stencil with the generated 2D texture
  pDepthSView = std::make_shared<DX11DepthStencilView>();
  D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
  ZeroMemory(&descDSV, sizeof(descDSV));
  descDSV.Format = descDepth.Format;
  descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
  descDSV.Texture2D.MipSlice = 0;
  hr = m_pDevice->pd3dDevice->CreateDepthStencilView(pDepthStencil, &descDSV, &pDepthSView->pDepthSV);
  if (hr != 0x00000000)
  {
    return;
  }
  m_pDevice->pImmediateContext->OMSetRenderTargets(1, &pRTargetView, pDepthSView->pDepthSV);
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
  m_pDevice->pImmediateContext->RSSetViewports(1, &vp);
  world = Matrix4::IDENTITY;
}

void
DX11GraphicsAPI::setShaders()
{
  m_pDevice->pImmediateContext->VSSetShader(vertexShader.pShader, nullptr, 0);
  m_pDevice->pImmediateContext->PSSetShader(pixelShader.pShader, nullptr, 0);
}

void
DX11GraphicsAPI::VSSetConstantBuffers()
{
  m_pDevice->pImmediateContext->VSSetConstantBuffers(0, 1, &cBView.pCBuffer);
  m_pDevice->pImmediateContext->VSSetConstantBuffers(1, 1, &cBProjection.pCBuffer);
  m_pDevice->pImmediateContext->VSSetConstantBuffers(2, 1, &cBWorld.pCBuffer);
  m_pDevice->pImmediateContext->VSSetConstantBuffers(3, 1, &cbLight.pCBuffer);
}

void
DX11GraphicsAPI::PSSetConstantBuffers()
{
  m_pDevice->pImmediateContext->PSSetConstantBuffers(0, 1, &cBView.pCBuffer);
  m_pDevice->pImmediateContext->PSSetConstantBuffers(1, 1, &cBProjection.pCBuffer);
  m_pDevice->pImmediateContext->PSSetConstantBuffers(2, 1, &cBWorld.pCBuffer);
  m_pDevice->pImmediateContext->PSSetConstantBuffers(3, 1, &cbLight.pCBuffer);
}

void
DX11GraphicsAPI::present(uint32 _syncInterval, uint32 _flags)
{
  pSwapChain->Present(1, 0);
}

void
DX11GraphicsAPI::setSampler()
{
  m_pDevice->pImmediateContext->PSSetSamplers(0, 1, &pSamplerLinear->pSampler);
}

SPtr<VertexBuffer>
DX11GraphicsAPI::createVertexBuffer(const Vector<SimpleVertex>& _vertex,
                                    uint32 _usage)
{
  auto dxVB = std::make_shared<DX11VertexBuffer>();

  /***************************************************************/
  /**
  * Define and create the buffer
  **/
  /***************************************************************/
  D3D11_BUFFER_DESC bd;
  memset(&bd, 0, sizeof(bd));
  bd.ByteWidth = static_cast<uint32>(sizeof(SimpleVertex) * _vertex.size()); // size of the buffer
  bd.Usage = static_cast<D3D11_USAGE>(_usage); // how it is expected to be read
  bd.BindFlags = D3D11_BIND_VERTEX_BUFFER; // how it will be binded to the pipeline
  bd.CPUAccessFlags = 0; // default -> CPU ha no accesss to this
  bd.MiscFlags = 0;

  // info descriptor
  D3D11_SUBRESOURCE_DATA InitData;
  memset(&InitData, 0, sizeof(InitData));
  InitData.pSysMem = _vertex.data(); // pointer to the initialization data
  InitData.SysMemPitch = static_cast<uint32>(_vertex.size() * sizeof(SimpleVertex)); // distance between values

  // create the buffer
  if (!m_pDevice->pd3dDevice)
  {
    // if device is null
    return nullptr;
  }
  m_pDevice->pd3dDevice->CreateBuffer(&bd, &InitData, &dxVB->pBuffer);
  return dxVB;
}

void
DX11GraphicsAPI::setVertexBuffer(SPtr<VertexBuffer>& _pVertexB,
                                 uint32 _start,
                                 uint32 _bufferCount,
                                 uint32 _offset)
{
  // reinterpret pointer
  auto dxVB = std::dynamic_pointer_cast<DX11VertexBuffer>(_pVertexB);
  if (!dxVB)
  {
    // failed to cast to DX11VertexBuffer
    return;
  }
  
  // get the offset
  uint32 stride = sizeof(SimpleVertex);
  //set the buffer
  m_pDevice->pImmediateContext->IASetVertexBuffers(_start,
                                                 _bufferCount,
                                                 &dxVB->pBuffer,
                                                 &stride,
                                                 &_offset);
}

SPtr<IndexBuffer>
DX11GraphicsAPI::createIndexBuffer(const Vector<uint32>& _index,
                                   uint32 _usage)
{
  auto dxIB = std::make_shared<DX11IndexBuffer>();

  /***************************************************************/
  /**
  * Define and create the buffer
  **/
  /***************************************************************/
  D3D11_BUFFER_DESC bd;
  memset(&bd, 0, sizeof(bd));
  bd.ByteWidth = sizeof(uint32) * (uint32)_index.size(); // size of the buffer
  bd.Usage = static_cast<D3D11_USAGE>(_usage); // how it is expected to be read
  bd.BindFlags = D3D11_BIND_INDEX_BUFFER; // how it will be binded to the pipeline
  bd.CPUAccessFlags = 0; // default -> CPU ha no accesss to this
  bd.MiscFlags = 0;

  D3D11_SUBRESOURCE_DATA InitData; // info descriptor
  memset(&InitData, 0, sizeof(InitData));
  InitData.pSysMem = _index.data(); // pointer to the initialization data
  // InitData.SysMemPitch = (uint32)index.size() * sizeof(uint32); // distance between values
  // create the buffer
  m_pDevice->pd3dDevice->CreateBuffer(&bd, &InitData, &dxIB->pBuffer);
  return dxIB;
}

void
DX11GraphicsAPI::setIndexBuffer(SPtr<IndexBuffer>& _pIndexB,
                                uint32 _format,
                                uint32 _offset)
{
  // reinterpret pointer
  auto dxIB = std::dynamic_pointer_cast<DX11IndexBuffer>(_pIndexB);
  if (!dxIB)
  {
    // failed to cast to DX11IndexBuffer
    return;
  }
  m_pDevice->pImmediateContext->IASetIndexBuffer(dxIB->pBuffer,
                                                 static_cast<DXGI_FORMAT>(_format),
                                                 _offset);
}
}

#endif