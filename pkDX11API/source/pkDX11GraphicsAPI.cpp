#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "pkDX11GraphicsAPI.h"
#include "pkDX11IndexBuffer.h"
#include "pkDX11Prerequisites.h"
#include "pkDX11VertexBuffer.h"
#include "pkDX11VertexShader.h"
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
  D3D_DRIVER_TYPE driverTypes[] = {
    D3D_DRIVER_TYPE_HARDWARE,
    D3D_DRIVER_TYPE_WARP,
    D3D_DRIVER_TYPE_REFERENCE,
  };

  D3D_FEATURE_LEVEL featureLevels[] = {
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

  m_pDevice->setPrimitiveTopology();
}

SPtr<ConstantBuffer>
DX11GraphicsAPI::createConstantBuffer(uint32 _size, const void* _pData, uint32 _usage)
{
  auto dxCB = make_shared<DX11ConstantBuffer>();
  HRESULT hr;
  D3D11_BUFFER_DESC bDesc;
  bDesc.Usage = static_cast<D3D11_USAGE>(_usage);
  bDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  bDesc.MiscFlags = 0;
  bDesc.ByteWidth = _size;
  bDesc.CPUAccessFlags = _usage == D3D10_USAGE_DYNAMIC ?
    D3D11_CPU_ACCESS_WRITE : 0;

  D3D11_SUBRESOURCE_DATA subData;
  if (_pData) {
    subData.pSysMem = _pData;
    subData.SysMemPitch = _size;
    subData.SysMemSlicePitch = 0;
  }

  hr = m_pDevice->pd3dDevice->CreateBuffer(&bDesc, _pData ? &subData : nullptr, &dxCB->pCBuffer);
  if (FAILED(hr)) {
    return nullptr;
  }

  return dxCB;
}

void
DX11GraphicsAPI::updateConstantBuffer(SPtr<ConstantBuffer> _pCBuffer,
                                      const void* _pNewData,
                                      uint32 _size)
{
  // cast to DX11ConstantBuffer
  auto dxCB = reinterpret_pointer_cast<DX11ConstantBuffer>(_pCBuffer);
  // casting failed
  if (!dxCB) {
    return;
  }
  // update with the new data
  m_pDevice->pImmediateContext->UpdateSubresource(dxCB->pCBuffer, 0, nullptr, _pNewData, _size, 0);
}

void
DX11GraphicsAPI::drawIndexed(uint32 _indexCount,
                             uint32 _startIndexLocation,
                             uint32 _baseVertexLocation)
{
  // draw the data
  m_pDevice->pImmediateContext->DrawIndexed(_indexCount,
                                            _startIndexLocation,
                                            _baseVertexLocation);
}

void
DX11GraphicsAPI::clearRenderTargetView(float _color[])
{
  m_pDevice->pImmediateContext->ClearRenderTargetView(pRTargetView->pRtv, _color);
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
DX11GraphicsAPI::makeShaders()
{
  pixelShader = make_shared<DX11PixelShader>();
  vertexShader = make_shared<DX11VertexShader>();
}

void
DX11GraphicsAPI::compileShaders()
{
  pixelShader->compile();
  vertexShader->compile();
}

void
DX11GraphicsAPI::createPShader()
{
  uint32 hr;
  hr = m_pDevice->pd3dDevice->CreatePixelShader(pixelShader->pSBlob->GetBufferPointer(),
                                                pixelShader->pSBlob->GetBufferSize(),
                                                nullptr, &pixelShader->pShader);
  if (hr != 0x00000000) {
    pixelShader->pSBlob->Release();
    return;
  }
  return;
}

void
DX11GraphicsAPI::createVShader()
{
  // create the vertex shader
  uint32 hr;
  hr = m_pDevice->pd3dDevice->CreateVertexShader(vertexShader->pSBlob->GetBufferPointer(),
                                                 vertexShader->pSBlob->GetBufferSize(),
                                                 nullptr,
                                                 &vertexShader->pShader);
  if (hr != 0x00000000) {
    vertexShader->pSBlob->Release();
    return;
  }
  return;
}

void
DX11GraphicsAPI::createShaders()
{
  createPShader();
  createVShader();
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
  m_pDevice = make_shared<DX11Device>();
  pSwapChain = make_shared<DX11SwapChain>();

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
  for (uint32 driverTypeIndex = 0; driverTypeIndex < _numDriverTypes; driverTypeIndex++) {
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
                                              &pSwapChain->pSch,
                                              &m_pDevice->pd3dDevice,
                                              &m_pDevice->featureLevel,
                                              &m_pDevice->pImmediateContext);

    // if creation was successful
    if (hr == 0x00000000) {
      // end the entire process, no need to continue
      break;
    }
  }
}

void
DX11GraphicsAPI::createRenderTargetView()
{
  // initiaize render target view
  pRTargetView = make_shared<DX11RenderTargetView>();

  // get buffer data
  ID3D11Texture2D* pBackBuffer = nullptr;
  uint32 hr = pSwapChain->pSch->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
  if (hr != 0x00000000) {
    return;
  }

  // create the render target view
  hr = m_pDevice->pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &pRTargetView->pRtv);
  pBackBuffer->Release();
  if (hr != 0x00000000) {
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
  pSamplerLinear = make_shared<DX11SamplerState>();
  uint32 hr = m_pDevice->pd3dDevice->CreateSamplerState(&sampDesc, &pSamplerLinear->pSampler);
  if (hr != 0x00000000) {
    return;
  }
}

void
DX11GraphicsAPI::createDepthStencilTexture(uint32 _width,
                                           uint32 _height)
{
  /**
  * Create depth stencil
  **/
  pDepthStencil = make_shared<DX11Texture>();
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
  uint32 hr = m_pDevice->pd3dDevice->CreateTexture2D(&descDepth, nullptr, &pDepthStencil->t2d);
  // if creating the texture failed
  if (hr != 0x00000000) {
    return;
  }

  // create depth stencil with the generated 2D texture
  pDepthSView = make_shared<DX11DepthStencilView>();
  D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
  ZeroMemory(&descDSV, sizeof(descDSV));
  descDSV.Format = descDepth.Format;
  descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
  descDSV.Texture2D.MipSlice = 0;
  hr = m_pDevice->pd3dDevice->CreateDepthStencilView(pDepthStencil->t2d, &descDSV, &pDepthSView->pDepthSV);
  if (hr != 0x00000000) {
    return;
  }
  m_pDevice->pImmediateContext->OMSetRenderTargets(1, &pRTargetView->pRtv, pDepthSView->pDepthSV);
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
  // world = Matrix4::IDENTITY;
}

void
DX11GraphicsAPI::setShaders()
{
  m_pDevice->pImmediateContext->VSSetShader(vertexShader->pShader, nullptr, 0);
  m_pDevice->pImmediateContext->PSSetShader(pixelShader->pShader, nullptr, 0);
}

void
DX11GraphicsAPI::createInputLayout()
{
  // make a shared DX11InputLayout pointer
  pInputL = make_shared<DX11InputLayout>();

  uint32 hr;
  // define the input layout
  D3D11_INPUT_ELEMENT_DESC layout[]
  {
    {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
  };
  uint32 numElem = ARRAYSIZE(layout);

  // create input layout
  hr = m_pDevice->pd3dDevice->CreateInputLayout(layout,
                                                numElem,
                                                vertexShader->pSBlob->GetBufferPointer(),
                                                vertexShader->pSBlob->GetBufferSize(),
                                                &pInputL->pVertexLayout);
  // failed to create the input layout
  if (hr != 0x00000000) {
    return;
  }
  // already used
  vertexShader->pSBlob->Release();
  // set the input layout
  pInputL->set(m_pDevice);
}

void
DX11GraphicsAPI::VSSetConstantBuffer(SPtr<ConstantBuffer> _pCBuffer,
                                      uint32 _startSlot,
                                      uint32 _numBuffers)
{
  auto dxCB = reinterpret_pointer_cast<DX11ConstantBuffer>(_pCBuffer);
  if (!dxCB) { return; } // casting failed
  m_pDevice->pImmediateContext->VSSetConstantBuffers(_startSlot, _numBuffers, &dxCB->pCBuffer);
}

void
DX11GraphicsAPI::PSSetConstantBuffer(SPtr<ConstantBuffer> _pCBuffer,
                                      uint32 _startSlot,
                                      uint32 _numBuffers)
{
  auto dxCB = reinterpret_pointer_cast<DX11ConstantBuffer>(_pCBuffer);
  if (!dxCB) { return; } // casting failed
  m_pDevice->pImmediateContext->PSSetConstantBuffers(_startSlot, _numBuffers, &dxCB->pCBuffer);
}

void
DX11GraphicsAPI::present(uint32 _syncInterval, uint32 _flags)
{
  pSwapChain->pSch->Present(_syncInterval, _flags);
}

void
DX11GraphicsAPI::setSampler(uint32 _startSlot,
                            uint32 _numSamplers)
{
  m_pDevice->pImmediateContext->PSSetSamplers(_startSlot,
                                              _numSamplers,
                                              &pSamplerLinear->pSampler);
}

void
DX11GraphicsAPI::setShaderResourceView(SPtr<Texture> _pTexture,
                                       uint32 _start,
                                       uint32 _numViews)
{
  // cast to a directX texture
  auto dxTX = reinterpret_pointer_cast<DX11Texture>(_pTexture);
  // if failed to cast to the texture
  if (!dxTX) { return; }
  // set the shader resource view 
  m_pDevice->pImmediateContext->PSSetShaderResources(_start, _numViews, &dxTX->srv);
}

SPtr<Texture>
DX11GraphicsAPI::createTextureFromFile(String& _fileName,
                                       uint32 _bindFlags,
                                       bool _mipLevels,
                                       uint32 _format)
{
  // values
  int32 width, height, bpp, pitch;

  // load the image data into a storage variable
  unsigned char* data = stbi_load(_fileName.c_str(), &width, &height, &bpp, 4);

  // check if the texture was found
  if (!data) { return make_shared<DX11Texture>(); }

  // how wide each line of the texture will be
  pitch = width * bpp;

  // create a default texture using the received parameters
  SPtr<Texture> temptTexture = createTextureDX(data, bpp, width, height, _format, 0, _bindFlags, _mipLevels);

  // if creating the texture failed
  if (!temptTexture) { return make_shared<DX11Texture>(); }

  // cast to DX11Texture
  SPtr<DX11Texture> dxTX = reinterpret_pointer_cast<DX11Texture>(temptTexture);

  // if casting the texture failed
  if (!dxTX) { return make_shared<DX11Texture>(); }

  // update the texture
  m_pDevice->pImmediateContext->UpdateSubresource(dxTX->t2d, 0, nullptr, data, pitch, 0);

  // free the texture data
  stbi_image_free(data);

  // set the path
  dxTX->path = _fileName;

  // return the texture
  return dxTX;
}

SPtr<Texture>
DX11GraphicsAPI::createTextureDX(unsigned char* _data,
                                 uint32 _bpp,
                                 uint32 _width,
                                 uint32 _height,
                                 uint32 _format,
                                 uint32 _usage,
                                 uint32 _bindFlags,
                                 bool _mipLevels)
{
  // texture description
  D3D11_TEXTURE2D_DESC desc;
  memset(&desc, 0, sizeof(desc));
  desc.Width = _width;
  desc.Height = _height;
  desc.MipLevels = _mipLevels ? 0 : 1;
  desc.ArraySize = 1;
  desc.Format = static_cast<DXGI_FORMAT>(_format);
  desc.SampleDesc.Count = 1;
  desc.SampleDesc.Quality = 0;
  desc.Usage = static_cast<D3D11_USAGE>(_usage);
  desc.BindFlags = _bindFlags;
  desc.CPUAccessFlags = 0;
  desc.MiscFlags = 0;

  // data of the texture
  D3D11_SUBRESOURCE_DATA initData;
  initData.pSysMem = _data;
  initData.SysMemPitch = static_cast<uint32>(_width * _bpp);
  initData.SysMemSlicePitch = 0;

  // create the texture
  SPtr<DX11Texture> tex = make_shared<DX11Texture>();
  m_pDevice->pd3dDevice->CreateTexture2D(&desc, &initData, &tex->t2d);

  if ((_bindFlags & D3D11_BIND_SHADER_RESOURCE) == D3D11_BIND_SHADER_RESOURCE) {
    // Create the shader resource descriptor for the texture
    D3D11_SHADER_RESOURCE_VIEW_DESC sDesc;
    memset(&sDesc, 0, sizeof(sDesc));
    sDesc.Format = desc.Format;
    sDesc.Texture2D.MipLevels = desc.MipLevels;
    sDesc.Texture2D.MostDetailedMip = 0;
    sDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

    // create the shader resource view
    m_pDevice->pd3dDevice->CreateShaderResourceView(tex->t2d, &sDesc, &tex->srv);
    if (!tex->srv) { return nullptr; } // failed to create shader resource view
  }

  if ((_bindFlags & D3D11_BIND_DEPTH_STENCIL) == D3D11_BIND_DEPTH_STENCIL) {
    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
    memset(&dsvDesc, 0, sizeof(dsvDesc));
    dsvDesc.Format = desc.Format;
    dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    dsvDesc.Texture2D.MipSlice = 0;
    m_pDevice->pd3dDevice->CreateDepthStencilView(tex->t2d, &dsvDesc, &pDepthSView->pDepthSV);
    if (!pDepthSView->pDepthSV) { return nullptr; }
  }

  return tex;
}

void
DX11GraphicsAPI::setInputLayout()
{
  pInputL->set(m_pDevice);
}

SPtr<VertexBuffer>
DX11GraphicsAPI::createVertexBuffer(const Vector<SimpleVertex>& _vertex,
                                    uint32 _usage)
{
  auto dxVB = make_shared<DX11VertexBuffer>();

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

  // if the device creation failed
  if (!m_pDevice->pd3dDevice) {
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
  auto dxVB = reinterpret_pointer_cast<DX11VertexBuffer>(_pVertexB);
  // if failed to cast to DX11VertexBuffer
  if (!dxVB) {
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
  auto dxIB = make_shared<DX11IndexBuffer>();

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
  auto dxIB = reinterpret_pointer_cast<DX11IndexBuffer>(_pIndexB);
  // if failed to cast to DX11IndexBuffer
  if (!dxIB) {
    return;
  }
  m_pDevice->pImmediateContext->IASetIndexBuffer(dxIB->pBuffer,
                                                 static_cast<DXGI_FORMAT>(_format),
                                                 _offset);
}
}

#endif