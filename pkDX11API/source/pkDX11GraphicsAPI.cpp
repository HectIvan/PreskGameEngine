/*****************************************************************************/
/**
 * @file    pkDX11GraphicsAPI.cpp
 * @author  Héctor  Iván Muñoz Ceballos
 * @date    19/11/2024
 * @brief   API file using DirectX 11 for the Presk Game Engine.
 *
 * @bug    No known bugs.
 */
 /*****************************************************************************/

/*********************************************/
/**
* Includes
**/
/*********************************************/

#include "pkAssetResourceManager.h"
#include "pkBlob.h"
#include "pkDX11BlendState.h"
#include "pkDX11ComputeShader.h"
#include "pkDX11GraphicsAPI.h"
#include "pkDX11IndexBuffer.h"
#include "pkDX11InputLayout.h"
#include "pkDX11PixelShader.h"
#include "pkDX11Prerequisites.h"
#include "pkDX11RasterizerState.h"
#include "pkDX11RenderTargetView.h"
#include "pkDX11SamplerState.h"
#include "pkDX11SwapChain.h"
#include "pkDX11VertexBuffer.h"
#include "pkDX11VertexShader.h"
#include "pkFileSystem.h"
#include "pkLogger.h"
#include "pkPlatformMath.h"
#include "pkTextureCodec.h"
#include "pkTextureResource.h"
#include "pkVertexBuffer.h"
#include "pkDX11ShaderCodec.h"


#if PK_PLATFORM == PK_PLATFORM_WIN32
#include <D3Dcompiler.h>

namespace pkEngineSDK
{
class ShaderInclude : public ID3DInclude
{
 public:
  HRESULT __stdcall
  Open(D3D_INCLUDE_TYPE, LPCSTR pFileName,
       LPCVOID, LPCVOID* ppData, UINT* pBytes) noexcept override {
    String workingDirectory;
    workingDirectory.resize(MAX_PATH);
    GetCurrentDirectoryA(MAX_PATH, &workingDirectory[0]);
    workingDirectory.resize(strlen(workingDirectory.c_str()));
    if (workingDirectory.back() != '\\' && workingDirectory.back() != '/') {
      workingDirectory.append("\\");
    }
    workingDirectory.append("shaders/");
    workingDirectory.append(pFileName);

    ifstream file(workingDirectory.c_str(), ios::binary | ios::ate);
    if (!file.is_open()) {
      return E_FAIL;
    }

    size_t size = file.tellg();
    file.seekg(0, ios::beg);

    if (size == 0) {
      return E_FAIL;
    }
    ANSICHAR* buffer = new ANSICHAR[size];
    file.read(buffer, size);

    *ppData = buffer;
    *pBytes = static_cast<UINT>(size);
    return S_OK;
  }

  HRESULT __stdcall Close(LPCVOID pData) noexcept override {
    delete[] static_cast<const ANSICHAR*>(pData);
    return S_OK;
  }
};

PKFORCEINLINE void
throwIfFailed(HRESULT hr) {
  if (FAILED(hr)) {
    PK_ASSERT(false && "Error in creation");
  }
}

extern "C" __declspec(dllexport) void
loadPlugin()
{
  GraphicsAPI::startUp<DX11GraphicsAPI>();
  ShaderCodec::startUp<DX11ShaderCodec>();
}

void
DX11GraphicsAPI::init(const Window& _window)
{
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

  WindowHandle winHandle = _window.getWindowHandle();
  uint32 width = static_cast<uint32>(_window.getClientWidthHeight().x);
  uint32 height = static_cast<uint32>(_window.getClientWidthHeight().y);

  createDeviceAndSwapChain(width,
                           height,
                           winHandle,
                           numDriverTypes,
                           driverTypes,
                           createDeviceFlags,
                           featureLevels,
                           numFeatureLevels);

  setViewport(width, height);

  // create the swap chain
  auto dxSCh = reinterpret_pointer_cast<DX11SwapChain>(m_pSwapChain);
  dxSCh->createRenderTargetView(m_pDevice);

  m_pDevice->setPrimitiveTopology();
}

SPtr<ConstantBuffer>
DX11GraphicsAPI::createConstantBuffer(const uint32 _size,
                                      const void* _pData,
                                      const uint32 _usage)
{
  Logger& log = g_Logger();
  // buffer description.
  D3D11_BUFFER_DESC bDesc;
  bDesc.Usage = static_cast<D3D11_USAGE>(_usage);
  bDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  bDesc.MiscFlags = 0;
  bDesc.ByteWidth = _size;
  bDesc.CPUAccessFlags = _usage == D3D10_USAGE_DYNAMIC ? D3D11_CPU_ACCESS_WRITE : 0;

  D3D11_SUBRESOURCE_DATA subData;
  if (_pData) {
    subData.pSysMem = _pData;
    subData.SysMemPitch = _size;
    subData.SysMemSlicePitch = 0;
  }

  // create the constant buffer.
  auto dxCB = make_shared<DX11ConstantBuffer>();
  const int32 hr = m_pDevice->m_pd3dDevice->CreateBuffer(&bDesc,
                                                         _pData ? &subData : nullptr,
                                                         &dxCB->pCBuffer);
  // check for errors.
  if (FAILED(hr)) {
    const String errMsg = log.getMessageError(hr);
    const String msg = "Failed to create the DX constant buffer. Error message: " + errMsg;
    log.registerMessage(msg, __FILE__, __LINE__, LOG_MSG_TYPE::kError);
    log.throwError(msg);
    return nullptr;
  }

  const String msg = "Created a DirectX constant buffer of size " + to_string(_size) + ".";
  log.registerMessage(msg, __FILE__, __LINE__);
  return dxCB;
}

void
DX11GraphicsAPI::updateConstantBuffer(const SPtr<ConstantBuffer>& _pCBuffer,
                                      const void* _pNewData,
                                      const SIZE_T _size)
{
  Logger& log = g_Logger();
  // cast to DX11ConstantBuffer
  auto dxCB = reinterpret_pointer_cast<DX11ConstantBuffer>(_pCBuffer);
  // casting failed
  if (!dxCB) {
    const String msg = ("Reinterpret constant buffer of size " +
                        to_string(_size) +
                        " to DX11ConstantBuffer failed.");
    log.registerMessage(msg, __FILE__, __LINE__, LOG_MSG_TYPE::kError);
    log.throwError(msg);
    return;
  }

  const uint32 size = static_cast<uint32>(_size);
  m_pDevice->m_pImmediateContext->UpdateSubresource(dxCB->pCBuffer,
                                                    0,
                                                    nullptr,
                                                    _pNewData,
                                                    size,
                                                    0);

  const String msg = "Updated a DirectX constant buffer of size " + to_string(_size) + ".";
  log.registerMessage(msg, __FILE__, __LINE__);
}

void
DX11GraphicsAPI::drawIndexed(uint32 _indexCount,
                             uint32 _startIndexLocation,
                             uint32 _baseVertexLocation)
{
  m_pDevice->m_pImmediateContext->DrawIndexed(_indexCount,
                                              _startIndexLocation,
                                              _baseVertexLocation);
}

void
DX11GraphicsAPI::draw(uint32 _indexCount, uint32 _startIndexLocation)
{
  m_pDevice->m_pImmediateContext->Draw(_indexCount, _startIndexLocation);
}

void
DX11GraphicsAPI::dispatch(uint32 _countX, uint32 _countY, uint32 _countZ)
{
  m_pDevice->m_pImmediateContext->Dispatch(_countX, _countY, _countZ);
}

void
DX11GraphicsAPI::clearRenderTargetViews(const Color& _color,
                                        const Vector<SPtr<Texture>>& _rtvs,
                                        const uint32 _mipSlice)
{
  for (uint32 i = 0; i < _rtvs.size(); ++i) {
    clearRenderTargetView(_color, _rtvs[i], _mipSlice);
  }
}

void
DX11GraphicsAPI::clearRenderTargetView(const Color& _color,
                                       const SPtr<Texture>& _rtv,
                                       const uint32 _mipSlice)
{
  PK_ASSERT(_rtv);
  Logger& log = g_Logger();
  // Texture to a DirectX texture
  SPtr<DX11Texture> dxRTV = reinterpret_pointer_cast<DX11Texture>(_rtv);
  // If the casting failed.
  if (!dxRTV) {
    const String msg = "Failed to clear the render target view.";
    log.registerMessage(msg, __FILE__, __LINE__, LOG_MSG_TYPE::kWarning);
    return;
  }

  float color[4] = { static_cast<float>(_color.getR()),
                     static_cast<float>(_color.getG()),
                     static_cast<float>(_color.getB()),
                     static_cast<float>(_color.getA()) };

  // if there is a mip slice specified to be cleared, clear said slice only and return.
  if (_mipSlice > -1) {
    ID3D11RenderTargetView* rtv = dxRTV->m_rTVs[_mipSlice];
    if (rtv) {
      m_pDevice->m_pImmediateContext->ClearRenderTargetView(rtv, color);
    }
    return;
  }
  // if no mipSlice is specified, clear all the slices.
  for (uint32 i = 0; i < dxRTV->m_rTVs.size(); ++i) {
    ID3D11RenderTargetView* rtv = dxRTV->m_rTVs[i];
    if (rtv) {
      m_pDevice->m_pImmediateContext->ClearRenderTargetView(rtv, color);
    }
  }
}

void
DX11GraphicsAPI::clearUnorderedAccessViews(const Vector<SPtr<Texture>>& _uavs,
                                           const Color& _color)
{
  for (uint32 i = 0; i < _uavs.size(); ++i) {
    clearUnorderedAccessView(_uavs[i], _color);
  }
}

void
DX11GraphicsAPI::clearUnorderedAccessView(const SPtr<Texture>& _uav, const Color& _color)
{
  PK_ASSERT(_uav);
  Logger& log = g_Logger();
  // Texture to a DirectX texture
  SPtr<DX11Texture> dxUAV = reinterpret_pointer_cast<DX11Texture>(_uav);
  // If the casting failed.
  if (!dxUAV) {
    const String msg = "Failed to clear the unordered access view.";
    log.registerMessage(msg, __FILE__, __LINE__, LOG_MSG_TYPE::kWarning);
    return;
  }

  float color[4] = { static_cast<float>(_color.getR()),
                     static_cast<float>(_color.getG()),
                     static_cast<float>(_color.getB()),
                     static_cast<float>(_color.getA()) };
  for (uint32 i = 0; i < dxUAV->m_uAVs.size(); ++i) {
    ID3D11UnorderedAccessView* uav = dxUAV->m_uAVs[i];
    if (uav) {
      m_pDevice->m_pImmediateContext->ClearUnorderedAccessViewFloat(uav, color);
    }
  }
}

void
DX11GraphicsAPI::clearDepthBuffer(float _depth, SPtr<Texture> _pDepthSV)
{
  // check if a depth stencil is being sent
  if (_pDepthSV) {
    PK_ASSERT(_pDepthSV);
    Logger& log = g_Logger();
    // Cast to a DirectX depth stencil.
    auto dxDSV = reinterpret_pointer_cast<DX11Texture>(_pDepthSV);
    // If the casting failed.
    if (!dxDSV) {
      const String msg = "Failed to reinterpret depth stencil view to DX11Texture.";
      log.registerMessage(msg, __FILE__, __LINE__, LOG_MSG_TYPE::kError);
      log.throwError(msg);
      return;
    }

    m_pDevice->m_pImmediateContext->ClearDepthStencilView(dxDSV->m_dSV,
                                                          D3D11_CLEAR_DEPTH,
                                                          _depth,
                                                          0);
  }
}

SPtr<Shader>
DX11GraphicsAPI::createVShader(SPtr<Shader>& _pShader)
{
  PK_ASSERT(_pShader);

  Logger& log = g_Logger();
  // convert from shader to dx vertex shader
  SPtr<DX11VertexShader> dxVShader = reinterpret_pointer_cast<DX11VertexShader>(_pShader);
  if (!dxVShader) {
    const String msg = "Shader is not Vertex Shader.";
    log.registerMessage(msg, __FILE__, __LINE__, LOG_MSG_TYPE::kError);
    log.throwError(msg);
    return nullptr;
  }
  // create the vertex shader
  uint32 hr;
  hr = m_pDevice->m_pd3dDevice->CreateVertexShader(dxVShader->m_pSBlob->getBufferPointer(),
                                                   dxVShader->m_pSBlob->getBufferSize(),
                                                   nullptr,
                                                   &dxVShader->m_pShader);
  // check if the creation was successful
  if (hr != 0x00000000) {
    const String errMsg = log.getMessageError(hr);
    const String msg = "Failed to create a DX Vertex Shader. Error message: " + errMsg;
    log.registerMessage(msg, __FILE__, __LINE__, LOG_MSG_TYPE::kError);
    safeRelease(dxVShader->m_pSBlob);
    return nullptr;
  }
  log.registerMessage("Created a DirectX Vertex Shader.", __FILE__, __LINE__);
  return dxVShader;
}

SPtr<Shader>
DX11GraphicsAPI::createPShader(SPtr<Shader>& _pShader)
{
  PK_ASSERT(_pShader);

  Logger& log = g_Logger();
  // convert from shader to dx pixel shader
  SPtr<DX11PixelShader> dxPShader = reinterpret_pointer_cast<DX11PixelShader>(_pShader);
  if (!dxPShader) {
    const String msg = "Shader is not Pixel Shader.";
    log.registerMessage(msg, __FILE__, __LINE__, LOG_MSG_TYPE::kError);
    log.throwError(msg);
    return nullptr;
  }

  uint32 hr;
  hr = m_pDevice->m_pd3dDevice->CreatePixelShader(dxPShader->m_pSBlob->getBufferPointer(),
                                                  dxPShader->m_pSBlob->getBufferSize(),
                                                  nullptr,
                                                  &dxPShader->m_pShader);
  // check if the creation was successful
  if (FAILED(hr)) {
    safeRelease(dxPShader->m_pSBlob);
    const String errMsg = log.getMessageError(hr);
    const String msg = "Failed to create a DX Pixel Shader. Error message: " + errMsg;
    log.registerMessage(msg, __FILE__, __LINE__, LOG_MSG_TYPE::kError);
    log.throwError(msg);
    return nullptr;
  }
  log.registerMessage("Created a DirectX Pixel Shader.", __FILE__, __LINE__);
  return dxPShader;
}

SPtr<Shader>
DX11GraphicsAPI::createCShader(SPtr<Shader>& _pShader)
{
  PK_ASSERT(_pShader);

  Logger& log = g_Logger();
  // convert from shader to dx compute shader
  SPtr<DX11ComputeShader> dxCShader = reinterpret_pointer_cast<DX11ComputeShader>(_pShader);

  if (!dxCShader) {
    const String msg = "Shader is not Compute Shader.";
    log.registerMessage(msg, __FILE__, __LINE__, LOG_MSG_TYPE::kError);
    log.throwError(msg);
    return nullptr;
  }

  uint32 hr;
  hr = m_pDevice->m_pd3dDevice->CreateComputeShader(dxCShader->m_pSBlob->getBufferPointer(),
                                                    dxCShader->m_pSBlob->getBufferSize(),
                                                    nullptr,
                                                    &dxCShader->m_pShader);
  // check if the creation was successful
  if (FAILED(hr)) {
    safeRelease(dxCShader->m_pSBlob);
    const String errMsg = log.getMessageError(hr);
    const String msg = "Failed to create a DX Compute Shader. Error message: " + errMsg;
    log.registerMessage(msg, __FILE__, __LINE__, LOG_MSG_TYPE::kError);
    log.throwError(msg);
    return nullptr;
  }
  log.registerMessage("Created a DirectX Compute Shader.", __FILE__, __LINE__);
  return dxCShader;
}


void
DX11GraphicsAPI::setVShader(const SPtr<Shader>& _pShader)
{
  SPtr<DX11VertexShader> dxVShader = reinterpret_pointer_cast<DX11VertexShader>(_pShader);

  // set the shader to the device context.
  m_pDevice->m_pImmediateContext->VSSetShader(dxVShader ? dxVShader->m_pShader : nullptr,
                                              nullptr,
                                              0);
}

void
DX11GraphicsAPI::setPShader(const SPtr<Shader>& _pShader)
{
  // reinterpret as a DirectX pixel shader
  SPtr<DX11PixelShader> dxPShader = reinterpret_pointer_cast<DX11PixelShader>(_pShader);

  m_pDevice->m_pImmediateContext->PSSetShader(dxPShader ? dxPShader->m_pShader : nullptr,
                                              nullptr,
                                              0);
}

void
DX11GraphicsAPI::setCShader(const SPtr<Shader>& _pShader)
{
  // reinterpret as a DirectX compute shader
  SPtr<DX11ComputeShader> dxCShader = reinterpret_pointer_cast<DX11ComputeShader>(_pShader);

  m_pDevice->m_pImmediateContext->CSSetShader(dxCShader ? dxCShader->m_pShader : nullptr,
                                              nullptr,
                                              0);
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
  Logger& log = g_Logger();
  // initialize device and swap chain
  m_pDevice = make_shared<DX11Device>();
  /**
  * Create the device and swap chains
  **/
  DXGI_SWAP_CHAIN_DESC sd;
  ZeroMemory(&sd, sizeof(sd));
  sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
  sd.BufferCount = 2;
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

  SPtr<DX11SwapChain> pSwapChain = make_shared<DX11SwapChain>();
  pSwapChain->setHeight(_height);
  pSwapChain->setWidth(_width);
  for (uint32 driverTypeIndex = 0; driverTypeIndex < _numDriverTypes; driverTypeIndex++) {
    // try and create the device and swap chain with the current driver type
    m_pDevice->m_pDriverType = new D3D_DRIVER_TYPE(_driverTypes[driverTypeIndex]);
    const int32 hr = D3D11CreateDeviceAndSwapChain(nullptr,
                                                   *m_pDevice->m_pDriverType,
                                                   nullptr,
                                                   _createDeviceFlags,
                                                   _featureLevels,
                                                   _numFeatureLevels,
                                                   D3D11_SDK_VERSION,
                                                   &sd,
                                                   &pSwapChain->m_pSch,
                                                   &m_pDevice->m_pd3dDevice,
                                                   &m_pDevice->m_featureLevel,
                                                   &m_pDevice->m_pImmediateContext);

    // if creation was successful
    if (hr == 0x00000000) {
      // end the entire process, no need to continue
      log.registerMessage("Created a DirectX device and Swap Chain.", __FILE__, __LINE__);
      m_pSwapChain = pSwapChain;
      break;
    }
  }
}

void
DX11GraphicsAPI::resizeSwapChain(const Vector2 _size)
{
  Logger& log = g_Logger();
  auto swapChain = reinterpret_pointer_cast<DX11SwapChain>(m_pSwapChain);

  if (!swapChain) {
    const String msg = "Swap chain not a directX 11 Swap chain.";
    log.registerMessage(msg, __FILE__, __LINE__, LOG_MSG_TYPE::kError);
    log.throwError(msg);
    return;
  }
  swapChain->resizebuffers(_size);
}

void
DX11GraphicsAPI::setRenderTargets(const Vector<SPtr<Texture>> _rTargets,
                                  const SPtr<Texture>& _pDepthSV,
                                  const uint32 _mipLevel)
{
  // reinterpret the depth stencil view to a DirectX texture
  auto pDSV = reinterpret_pointer_cast<DX11Texture>(_pDepthSV);
  // render target vector
  Vector<ID3D11RenderTargetView*> rTVector;
  // get the vector size
  const uint32 RTCount = static_cast<uint32>(_rTargets.size());
  // reinterpret each of the targets as a DX11 texture and store in the texture vector
  for (uint32 i = 0; i < RTCount; ++i) {
    SPtr<DX11Texture> dxTx = reinterpret_pointer_cast<DX11Texture>(_rTargets[i]);
    // if target is valid, store it
    ID3D11RenderTargetView* rTView = dxTx->m_rTVs[_mipLevel];
    if (_rTargets[i] && rTView) {
      rTVector.push_back(rTView);
    }
    // if its not, save a null pointer
    else { rTVector.push_back(nullptr); }
  }

  m_pDevice->m_pImmediateContext->OMSetRenderTargets(static_cast<uint32>(rTVector.size()),
                                                     rTVector.data(),
                                                     (pDSV) ? pDSV->m_dSV : nullptr);
}

void
DX11GraphicsAPI::unbindRenderTargets(const SIZE_T _count)
{
  static Vector<ID3D11RenderTargetView*> unbindRT;
  unbindRT.resize(_count, nullptr);

  // const uint32 size = static_cast<uint32>(unbindRT.size());
  m_pDevice->m_pImmediateContext->OMSetRenderTargets(static_cast<uint32>(_count),
                                                     unbindRT.data(),
                                                     nullptr);
}

void
DX11GraphicsAPI::setRenderTarget(const SPtr<Texture>& _pRTarget,
                                 const SPtr<Texture>& _pDepthSV,
                                 const uint32 _mipLevel)
{
  // reinterpet render target
  auto rTarget = reinterpret_pointer_cast<DX11Texture>(_pRTarget);
  // reinterpret the depth stencil view
  auto pDSV = reinterpret_pointer_cast<DX11Texture>(_pDepthSV);
  m_pDevice->m_pImmediateContext->OMSetRenderTargets(rTarget ? 1 : 0,
                                                     rTarget ? &rTarget->m_rTVs[_mipLevel] : nullptr,
                                                     pDSV ? pDSV->m_dSV : nullptr);
}

SPtr<SamplerState>
DX11GraphicsAPI::createSamplerState(const uint32 _mode, const uint32 _filter)
{
  Logger& log = g_Logger();
  // sampler state description
  D3D11_SAMPLER_DESC sampDesc;
  ZeroMemory(&sampDesc, sizeof(sampDesc));
  sampDesc.Filter = static_cast<D3D11_FILTER>(_filter);
  sampDesc.AddressU = static_cast<D3D11_TEXTURE_ADDRESS_MODE>(_mode);
  sampDesc.AddressV = static_cast<D3D11_TEXTURE_ADDRESS_MODE>(_mode);
  sampDesc.AddressW = static_cast<D3D11_TEXTURE_ADDRESS_MODE>(_mode);
  sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
  sampDesc.MinLOD = 0;
  sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

  SPtr<DX11SamplerState> pSamState = make_shared<DX11SamplerState>();
  const uint32 hr = m_pDevice->m_pd3dDevice->CreateSamplerState(&sampDesc,
                                                                &pSamState->m_pSampler);
  if (FAILED(hr)) {
    const String errMsg = g_Logger().getMessageError(hr);
    const String msg = "Failed to create a sampler state. Error message: " + errMsg;
    log.registerMessage(msg, __FILE__, __LINE__, LOG_MSG_TYPE::kError);
    log.throwError(msg);
    return nullptr;
  }
  return pSamState;
}

void
DX11GraphicsAPI::waitDevice()
{
  ID3D11Query* query;
  D3D11_QUERY_DESC desc = {};
  desc.Query = D3D11_QUERY_EVENT;

  m_pDevice->m_pd3dDevice->CreateQuery(&desc, &query);
  m_pDevice->m_pImmediateContext->End(query);

  while (m_pDevice->m_pImmediateContext->GetData(query, nullptr, 0, 0) == S_FALSE) {
    Sleep(0);
  }
}

void
DX11GraphicsAPI::setViewport(Vector2 _size)
{
  setViewport(_size.x, _size.y);
}

void
DX11GraphicsAPI::setViewport(float _width,
                             float _height)
{
  PK_ASSERT(m_pDevice);
  D3D11_VIEWPORT vp;
  vp.Width = _width;
  vp.Height = _height;
  vp.MinDepth = 0.0f;
  vp.MaxDepth = 1.0f;
  vp.TopLeftX = 0;
  vp.TopLeftY = 0;
  
  m_pDevice->m_pImmediateContext->RSSetViewports(1, &vp);
}

SPtr<BlendState>
DX11GraphicsAPI::createBlendState()
{
  Logger& log = g_Logger();
  // create the blend state
  PK_ASSERT(m_pDevice);
  // Create the description
  D3D11_BLEND_DESC blendDesc = {};
  blendDesc.AlphaToCoverageEnable = FALSE;

  for (int i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i) {
    blendDesc.RenderTarget[i].BlendEnable = true;
    blendDesc.RenderTarget[i].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    blendDesc.RenderTarget[i].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    blendDesc.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ZERO;
    blendDesc.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
  }

  SPtr<DX11BlendState> pBlendState = make_shared<DX11BlendState>();
  const uint32 hr = m_pDevice->m_pd3dDevice->CreateBlendState(&blendDesc,
                                                              &pBlendState->m_pBlendState);

  if (FAILED(hr)) {
    const String errMsg = log.getMessageError(hr);
    const String msg = "Failed to create Blend state. Error message: " + errMsg;
    log.registerMessage(msg, __FILE__, __LINE__, LOG_MSG_TYPE::kError);
    log.throwError(msg);
    return nullptr;
  }
  log.registerMessage("Created a Blend State.", __FILE__, __LINE__);
  return pBlendState;
}

SPtr<RasterizerState>
DX11GraphicsAPI::createRasterizerState(const RASTERIZER_DESC& _desc)
{
  Logger& log = g_Logger();
  // rasterizer description for directx.
  D3D11_RASTERIZER_DESC rDesc = {};
  rDesc.FillMode = static_cast<D3D11_FILL_MODE>(_desc.fillMode);
  rDesc.CullMode = static_cast<D3D11_CULL_MODE>(_desc.cullMode);
  rDesc.FrontCounterClockwise = _desc.frontCounterClockwise;
  rDesc.DepthClipEnable = _desc.depthClipEnable;

  // create the rasterizer state.
  SPtr<DX11RasterizerState> dxRS = make_shared<DX11RasterizerState>();
  const uint32 hr = m_pDevice->m_pd3dDevice->CreateRasterizerState(&rDesc,
                                                                   &dxRS->m_pRasterizer);

  // check for errors.
  if (FAILED(hr)) {
    const String errMsg = log.getMessageError(hr);
    const String msg = "Failed to create a Rasterizer State. Error message: " + errMsg;
    log.registerMessage(msg, __FILE__, __LINE__, LOG_MSG_TYPE::kError);
    log.throwError(msg);
    return nullptr;
  }

  log.registerMessage("Created a Rasterizer State.", __FILE__, __LINE__);
  return dxRS;
}

void
DX11GraphicsAPI::setBlendState(const SPtr<BlendState>& _pBlendState)
{
  PK_ASSERT(_pBlendState);

  Logger& log = g_Logger();
  // Reinterpret to a DirectX Blend State
  SPtr<DX11BlendState> dxBS = reinterpret_pointer_cast<DX11BlendState>(_pBlendState);
  if (!dxBS) {
    const String msg = "Failed to reinterpret a DX Blend State.";
    log.registerMessage(msg, __FILE__, __LINE__, LOG_MSG_TYPE::kError);
    return;
  }
  m_pDevice->m_pImmediateContext->OMSetBlendState(dxBS->m_pBlendState, nullptr, 0xFFFFFFFF);
}

void
DX11GraphicsAPI::setRasterizerState(const SPtr<RasterizerState>& _pRasterizerState)
{
  PK_ASSERT(m_pDevice);

  // Reinterpret to a DirectX Rasterizer State
  SPtr<DX11RasterizerState> dxRS = reinterpret_pointer_cast<DX11RasterizerState>(_pRasterizerState);
  // Set the rasterizer state
  m_pDevice->m_pImmediateContext->RSSetState(dxRS ? dxRS->m_pRasterizer : nullptr);
}

PKBlob*
DX11GraphicsAPI::compileShaderFromFile(Path _szFileName,
                                       const ANSICHAR* _szEntryPoint,
                                       const ANSICHAR* _szShaderModel)
{
  Logger& log = g_Logger();
  uint32 hr = S_OK;

  DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
  // Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
  // Setting this flag improves the shader debugging experience, but still allows 
  // the shaders to be optimized and to run exactly the way they will run in 
  // the release configuration of this program.
  dwShaderFlags |= D3DCOMPILE_DEBUG;
#else
  dwShaderFlags |= D3DCOMPILE_ENABLE_STRICTNESS; // temporarily for Release (D3DCOMPILE_SKIP_OPTIMIZATION)
#endif
  
  if (!FileSystem::fileExists(_szFileName)) {
    const String err = "Shader file not found: " + _szFileName.toString();
    log.registerMessage(err, __FILE__, __LINE__, LOG_MSG_TYPE::kError);
    return nullptr;
  }

  static ShaderInclude shaderInclude;
  ID3DBlob* pErrorBlob = nullptr;
  ID3DBlob* dxBlob = nullptr;
  WString widePath = FileSystem::getAbsolutePathWStr(_szFileName);// _szFileName.getPathWStr(); // release issue

  hr = D3DCompileFromFile(widePath.c_str(),
                          nullptr,
                          &shaderInclude,
                          _szEntryPoint,
                          _szShaderModel,
                          dwShaderFlags,
                          0,
                          &dxBlob,
                          &pErrorBlob);
  // to do: change this function to another as this one crashes easily.

  // if there's an error.
  if (pErrorBlob) {
    const ANSICHAR* msg = reinterpret_cast<ANSICHAR*>(pErrorBlob->GetBufferPointer());
    const SIZE_T len = pErrorBlob->GetBufferSize();
    const String error(msg, len);
    String fullMSG = "Shader failed to compile. Error message: " + error;
    // if the compilation outright failed
    if (FAILED(hr)) {
      log.registerMessage(fullMSG, __FILE__, __LINE__, LOG_MSG_TYPE::kError);
      return nullptr;
    }
    // if it didnt fail but there's a message, it must be a warning
    fullMSG = _szFileName.toString() + " - Warning: " + error;
    log.registerMessage(fullMSG, __FILE__, __LINE__, LOG_MSG_TYPE::kWarning);
    pErrorBlob->Release();
  }
  safeRelease(pErrorBlob);

  PKBlob* blob = new PKBlob(dxBlob->GetBufferPointer(), dxBlob->GetBufferSize());

  return blob;
}

SPtr<InputLayout>
DX11GraphicsAPI::createInputLayoutFromVShader(const SPtr<Shader>& _pShader)
{
  PK_ASSERT(_pShader);

  Logger& log = g_Logger();
  // create the input layout pointer
  SPtr<DX11InputLayout> pLayout = make_shared<DX11InputLayout>();
  // reinterpret to a DirectX vertex shader
  SPtr<DX11VertexShader> dxVShader = reinterpret_pointer_cast<DX11VertexShader>(_pShader);

  ID3D11ShaderReflection* pVShaderReflection = nullptr;
  throwIfFailed(D3DReflect(dxVShader->m_pSBlob->getBufferPointer(),
                           dxVShader->m_pSBlob->getBufferSize(),
                           __uuidof(ID3D11ShaderReflection),
                           reinterpret_cast<void**>(&pVShaderReflection)));

  // shader data
  D3D11_SHADER_DESC vShaderDesc;
  pVShaderReflection->GetDesc(&vShaderDesc);

  Vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;
  for (uint32 i = 0; i < vShaderDesc.InputParameters; ++i) {
    D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
    pVShaderReflection->GetInputParameterDesc(i, &paramDesc);

    // fill out input element
    D3D11_INPUT_ELEMENT_DESC elemDesc;
    elemDesc.SemanticIndex = paramDesc.SemanticIndex;
    elemDesc.SemanticName = paramDesc.SemanticName;
    elemDesc.InputSlot = 0;
    elemDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    elemDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    elemDesc.InstanceDataStepRate = 0;

    // Red channel
    if (paramDesc.Mask == 1) {
      if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) {
        elemDesc.Format = DXGI_FORMAT_R32_UINT;
      }
      else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) {
        elemDesc.Format = DXGI_FORMAT_R32_SINT;
      }
      else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) {
        elemDesc.Format = DXGI_FORMAT_R32_FLOAT;
      }
    }
    // Red Green channels
    else if (paramDesc.Mask <= 3) {
      if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) {
        elemDesc.Format = DXGI_FORMAT_R32G32_UINT;
      }
      else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) {
        elemDesc.Format = DXGI_FORMAT_R32G32_SINT;
      }
      else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) {
        elemDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
      }
    }
    // Red Green Blue Channels
    else if (paramDesc.Mask <= 7) {
      if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) {
        elemDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
      }
      else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) {
        elemDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
      }
      else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) {
        elemDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
      }
    }
    // Red Green Blue Alpha Channels
    else if (paramDesc.Mask <= 15) {
      if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) {
        elemDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
      }
      else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) {
        elemDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
      }
      else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) {
        elemDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
      }
    }

    inputLayoutDesc.push_back(elemDesc);
  }

  m_pDevice->m_pd3dDevice->CreateInputLayout(&inputLayoutDesc[0],
                                             static_cast<uint32>(inputLayoutDesc.size()),
                                             dxVShader->m_pSBlob->getBufferPointer(),
                                             dxVShader->m_pSBlob->getBufferSize(),
                                             &pLayout->m_pVertexLayout);
  if (!pLayout) {
    const String msg = "Failed to create the input layout.";
    log.registerMessage(msg, __FILE__, __LINE__, LOG_MSG_TYPE::kError);
    log.throwError(msg);
    return nullptr;
  }
  return pLayout;
}

SPtr<Texture>
DX11GraphicsAPI::createTexture(const TextureDesc& _desc)
{
  return createTexture(_desc.width, _desc.height, _desc.format, _desc.usage,
                       _desc.bindFlags, _desc.shaderResourceFormat, _desc.mipLevels, _desc.isCube);
}

SPtr<InputLayout>
DX11GraphicsAPI::createInputLayout(const Vector<InputDesc>& _vDesc,
                                   const SPtr<Shader> _pVShader)
{
  PK_ASSERT(_pVShader);

  Logger& log = g_Logger();
  // make a shared DX11InputLayout pointer
  SPtr<DX11InputLayout> pInputL = make_shared<DX11InputLayout>();
  SPtr<DX11VertexShader> dxVShader = reinterpret_pointer_cast<DX11VertexShader>(_pVShader);
  
  uint32 hr;
  // define the input layout
  Vector<D3D11_INPUT_ELEMENT_DESC> dxLayout;
  const SIZE_T layoutSize = _vDesc.size();
  dxLayout.resize(layoutSize);

  uint32 offset = 0;

  for (uint32 i = 0; i < layoutSize; ++i) {
    auto& element = dxLayout[i];
    memset(&element, 0, sizeof(D3D11_INPUT_ELEMENT_DESC));
    element.Format = static_cast<DXGI_FORMAT>(_vDesc[i].format);
    element.SemanticIndex = 0;
    element.InputSlot = 0;
    element.AlignedByteOffset = offset;
    element.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    element.InstanceDataStepRate = 0;

    offset += _vDesc[i].size;

    if (_vDesc[i].type == INPUT_LAYOUT::kPosition) {
      element.SemanticName = "POSITION";
    }
    if (_vDesc[i].type == INPUT_LAYOUT::kNormal) {
      element.SemanticName = "NORMAL";
    }
    if (_vDesc[i].type == INPUT_LAYOUT::kTexCoord) {
      element.SemanticName = "TEXCOORD";
    }
    if (_vDesc[i].type == INPUT_LAYOUT::kTangent) {
      element.SemanticName = "TANGENT";
    }
    if (_vDesc[i].type == INPUT_LAYOUT::kBinormal) {
      element.SemanticName = "BINORMAL";
    }
    if (_vDesc[i].type == INPUT_LAYOUT::kColor) {
      element.SemanticName = "COLOR";
    }
  }

  hr = m_pDevice->m_pd3dDevice->CreateInputLayout(dxLayout.data(),
                                                  static_cast<uint32>(dxLayout.size()),
                                                  dxVShader->m_pSBlob->getBufferPointer(),
                                                  dxVShader->m_pSBlob->getBufferSize(),
                                                  &pInputL->m_pVertexLayout);
  // failed to create the input layout
  if (FAILED(hr)) {
    const String errMsg = log.getMessageError(hr);
    const String msg = "Failed to create the input layout. Error message: " + errMsg;
    log.registerMessage(msg, __FILE__, __LINE__, LOG_MSG_TYPE::kError);
    log.throwError(msg);
    return nullptr;
  }

  return pInputL;
}

void
DX11GraphicsAPI::vSSetConstantBuffers(const Vector<SPtr<ConstantBuffer>>& _pCBuffers,
                                      const uint32 _startSlot)
{
  // local array of directx buffers
  const uint32 count = static_cast<uint32>(_pCBuffers.size());
  Vector<ID3D11Buffer*> buffers(count);

  // set all buffers
  for (uint32 i = 0; i < count; ++i) {
    // Recast to a DirectX Constant buffer
    auto dxCB = reinterpret_pointer_cast<DX11ConstantBuffer>(_pCBuffers[i]);
    // set the buffer
    buffers[i] = dxCB ? dxCB->pCBuffer : nullptr;
  }
  // set the buffers
  m_pDevice->m_pImmediateContext->VSSetConstantBuffers(_startSlot, count, buffers.data());
}

void
DX11GraphicsAPI::vSUnbindConstantBuffers(const uint32 _count)
{
  Vector<ID3D11Buffer*> buffers(_count, nullptr);
  // set the buffers
  m_pDevice->m_pImmediateContext->VSSetConstantBuffers(0, _count, buffers.data());
}

void
DX11GraphicsAPI::pSSetConstantBuffers(const Vector<SPtr<ConstantBuffer>>& _pCBuffers,
                                      const uint32 _startSlot)
{
  // local array of directx buffers
  const uint32 count = static_cast<uint32>(_pCBuffers.size());
  Vector<ID3D11Buffer*> buffers(count);

  // set all the buffers in the array
  for (uint32 i = 0; i < _pCBuffers.size(); ++i) {
    // Recast to a DirectX Constant buffer.
    auto dxCB = reinterpret_pointer_cast<DX11ConstantBuffer>(_pCBuffers[i]);
    // set the buffer
    buffers[i] = dxCB ? dxCB->pCBuffer : nullptr;
  }
  // set the buffers
  m_pDevice->m_pImmediateContext->PSSetConstantBuffers(_startSlot, count, buffers.data());
}

void
DX11GraphicsAPI::pSUnbindConstantBuffers(const uint32 _count)
{
  Vector<ID3D11Buffer*> buffers(_count, nullptr);
  // set the buffers
  m_pDevice->m_pImmediateContext->PSSetConstantBuffers(0, _count, buffers.data());
}

void
DX11GraphicsAPI::cSSetConstantBuffers(const Vector<SPtr<ConstantBuffer>>& _pCBuffers,
                                      const uint32 _startSlot)
{
  // local array of directx buffers
  const uint32 count = static_cast<uint32>(_pCBuffers.size());
  Vector<ID3D11Buffer*> buffers(count);

  for (uint32 i = 0; i < _pCBuffers.size(); ++i) {
    // Recast to a DirectX Constant buffer.
    auto dxCB = reinterpret_pointer_cast<DX11ConstantBuffer>(_pCBuffers[i]);
    // set the buffer
    buffers[i] = dxCB ? dxCB->pCBuffer : nullptr;
  }
  // set the buffers
  m_pDevice->m_pImmediateContext->CSSetConstantBuffers(_startSlot, count, buffers.data());
}

// to do: change this to dynamic size
void
DX11GraphicsAPI::cSUnbindConstantBuffers(const uint32 _count)
{
  Vector<ID3D11Buffer*> buffers(_count, nullptr);
  // set the buffers
  m_pDevice->m_pImmediateContext->CSSetConstantBuffers(0, _count, buffers.data());
}

void
DX11GraphicsAPI::present(uint32 _syncInterval, uint32 _flags)
{
  Logger& log = g_Logger();
  // reinterpret the swap chain to a DirectX swap chain
  auto dxSwapChain = reinterpret_pointer_cast<DX11SwapChain>(m_pSwapChain);
  if (!dxSwapChain) {
    const String msg = "Failed to present a result on the screen.";
    log.registerMessage(msg, __FILE__, __LINE__, LOG_MSG_TYPE::kError);
    return;
  }
  // use the swap chain to present the result
  dxSwapChain->m_pSch->Present(_syncInterval, _flags);
}

Vector2
DX11GraphicsAPI::getViewportSize(uint32 _vpPos)
{
  D3D11_VIEWPORT viewport;
  m_pDevice->m_pImmediateContext->RSGetViewports(&_vpPos, &viewport);
  
  return Vector2(viewport.Width, viewport.Height);
}

void
DX11GraphicsAPI::setSampler(const SPtr<SamplerState> _pSamLinear,
                            uint32 _startSlot,
                            uint32 _numSamplers)
{
  // reinterpret to DirectX sampler state
  auto dxSS = reinterpret_pointer_cast<DX11SamplerState>(_pSamLinear);
  if (_pSamLinear && !dxSS) {
    const String msg = "Failed to set a sampler.";
    g_Logger().registerMessage(msg, __FILE__, __LINE__, LOG_MSG_TYPE::kError);
    return;
  }
  m_pDevice->m_pImmediateContext->PSSetSamplers(_startSlot,
                                                _pSamLinear ? _numSamplers : 0,
                                                _pSamLinear ? &dxSS->m_pSampler : nullptr);
}

void
DX11GraphicsAPI::pSSetShaderResourceViews(const Vector<SPtr<Texture>>& _pTextures,
                                          const uint32 _start)
{
  const uint32 count = static_cast<uint32>(_pTextures.size());
  Vector<ID3D11ShaderResourceView*> vResourceVector(count);

  for (uint32 i = 0; i < _pTextures.size(); ++i) {
    // Recast to a DirectX Texture
    auto dxSRV = reinterpret_pointer_cast<DX11Texture>(_pTextures[i]);
    // set the resource
    vResourceVector[i] = dxSRV ? dxSRV->m_sRV : nullptr;
  }
  m_pDevice->m_pImmediateContext->PSSetShaderResources(_start, count, vResourceVector.data());
}

void
DX11GraphicsAPI::pSUnbindShaderResourceViews(const SIZE_T _count)
{
  static Vector<ID3D11ShaderResourceView*> unbindSRV;
  unbindSRV.resize(_count, nullptr);

  const uint32 size = static_cast<uint32>(unbindSRV.size());
  m_pDevice->m_pImmediateContext->PSSetShaderResources(0, size, unbindSRV.data());
}

void
DX11GraphicsAPI::vSSetShaderResourceViews(const Vector<SPtr<Texture>>& _pTextures,
                                          const uint32 _start)
{
  const uint32 count = static_cast<uint32>(_pTextures.size());
  Vector<ID3D11ShaderResourceView*> vResourceVector(count);

  for (uint32 i = 0; i < _pTextures.size(); ++i) {
    // Recast to a DirectX Texture
    auto dxSRV = reinterpret_pointer_cast<DX11Texture>(_pTextures[i]);
    // set the resource
    vResourceVector[i] = dxSRV ? dxSRV->m_sRV : nullptr;
  }
  m_pDevice->m_pImmediateContext->VSSetShaderResources(_start, count, vResourceVector.data());
}

void
DX11GraphicsAPI::vSUnbindShaderResourceViews(const SIZE_T _count)
{
  static Vector<ID3D11ShaderResourceView*> unbindSRV;
  unbindSRV.resize(_count, nullptr);

  const uint32 size = static_cast<uint32>(unbindSRV.size());
  m_pDevice->m_pImmediateContext->VSSetShaderResources(0, size, unbindSRV.data());
}

void
DX11GraphicsAPI::cSSetShaderResourceViews(const Vector<SPtr<Texture>>& _pTextures,
                                          const uint32 _start)
{
  const uint32 count = static_cast<uint32>(_pTextures.size());
  Vector<ID3D11ShaderResourceView*> uavVector(count);

  for (uint32 i = 0; i < _pTextures.size(); ++i) {
    // Recast to a DirectX Texture
    auto dxUAV = reinterpret_pointer_cast<DX11Texture>(_pTextures[i]);
    // set the resource
    uavVector[i] = dxUAV ? dxUAV->m_sRV : nullptr;
  }
  m_pDevice->m_pImmediateContext->CSSetShaderResources(_start, count, uavVector.data());
}

void
DX11GraphicsAPI::cSUnbindShaderResourceViews(const SIZE_T _count)
{
  static Vector<ID3D11ShaderResourceView*> unbindSRV;
  unbindSRV.resize(_count, nullptr);

  const uint32 size = static_cast<uint32>(unbindSRV.size());
  m_pDevice->m_pImmediateContext->CSSetShaderResources(0, size, unbindSRV.data());
}

void
DX11GraphicsAPI::cSSetUnorderedAccessViews(const Vector<SPtr<Texture>>& _pTextures,
                                           const uint32 _start,
                                           const uint32* _initialCounts,
                                           const uint32 _mipLevel)
{
  const uint32 count = static_cast<uint32>(_pTextures.size());
  Vector<ID3D11UnorderedAccessView*> uavVector(count);

  for (uint32 i = 0; i < _pTextures.size(); ++i) {
    // Recast to a DirectX Texture
    auto dxUAV = reinterpret_pointer_cast<DX11Texture>(_pTextures[i]);
    // set the resource
    uavVector[i] = dxUAV ? dxUAV->m_uAVs[_mipLevel] : nullptr;
  }

  m_pDevice->m_pImmediateContext->CSSetUnorderedAccessViews(_start,
                                                            count,
                                                            uavVector.data(),
                                                            _initialCounts);
}

void
DX11GraphicsAPI::cSUnbindUnorderedAccessViews(const SIZE_T _count)
{
  static Vector<ID3D11UnorderedAccessView*> unbindUAV;
  unbindUAV.resize(_count, nullptr);

  const uint32 numViews = static_cast<uint32>(unbindUAV.size());
  m_pDevice->m_pImmediateContext->CSSetUnorderedAccessViews(0,
                                                            numViews,
                                                            unbindUAV.data(),
                                                            nullptr);
}

SPtr<Texture>
DX11GraphicsAPI::createTextureFromResource(const SPtr<BaseResource>& _pResource,
                                           uint32 _bindFlags)
{
  Logger& log = g_Logger();

  if (_pResource->getType() != RESOURCE_TYPE::kTexture) {
    const String msg = "Failed to create texture from resource: " +
                       _pResource->m_name +
                       ". Resource is not a texture resource.";
    log.registerMessage(msg, __FILE__, __LINE__, LOG_MSG_TYPE::kWarning);
    return nullptr;
  }

  auto resource = reinterpret_pointer_cast<TextureResource>(_pResource);

  const uint32 mipLevels = resource->m_mipMapCount;

  resource->load();

  // create a default texture using the received parameters
  SPtr<Texture> tempTexture = createTexture(resource->m_width,
                                            resource->m_height,
                                            resource->m_format,
                                            PK_USAGE::kPK_USAGE_DEFAULT,
                                            _bindFlags,
                                            resource->m_format,
                                            mipLevels);

  // if creating the texture failed
  if (!tempTexture) {
    const String msg = "Failed to create a texture.";
    log.registerMessage(msg, __FILE__, __LINE__, LOG_MSG_TYPE::kError);
    return nullptr;
  }

  // add data to the texture
  resource->m_bpp = getBytesFromFormat(static_cast<PK_TEXTURE_FORMAT::E>(resource->m_format));
  auto dxTex = reinterpret_pointer_cast<DX11Texture>(tempTexture);
  m_pDevice->m_pImmediateContext->UpdateSubresource(dxTex->getTexture2D(),
                                                    0,
                                                    nullptr,
                                                    resource->m_data.data(),
                                                    resource->m_width * resource->m_bpp,
                                                    0);

  const bool genMips = (mipLevels == 0 || mipLevels > 1);
  if (genMips) {
    generateMips(tempTexture);
  }

  // set the path
  tempTexture->setName(resource->m_name);
  tempTexture->setID(resource->m_id);

  // return the texture
  return tempTexture;
}

SPtr<Texture>
DX11GraphicsAPI::createDDSTextureFromFile(const Path& _directory)
{
  Logger& log = g_Logger();
  SPtr<DX11Texture> texture = make_shared<DX11Texture>();

  const uint32 hr = DirectX::CreateDDSTextureFromFile(m_pDevice->m_pd3dDevice,
                                                      _directory.getDirectoryWStr().c_str(),
                                                      nullptr,
                                                      &texture->m_sRV);
  if (FAILED(hr)) {
    const String errMsg = log.getMessageError(hr);
    const String msg = "Failed to create a DSS texture. Error message: " + errMsg;
    log.registerMessage(msg, __FILE__, __LINE__, LOG_MSG_TYPE::kError);
    return nullptr;
  }
  return texture;
}

void
DX11GraphicsAPI::generateMips(const SPtr<Texture>& _pTexture)
{
  auto texture = reinterpret_pointer_cast<DX11Texture>(_pTexture);
  if (!texture) {
    const String msg = "Failed to generate mips for texture: " +
                       _pTexture->getName().getPath();
    g_Logger().registerMessage(msg, __FILE__, __LINE__, LOG_MSG_TYPE::kError);
    return;
  }

  ID3D11ShaderResourceView* srv = texture->getSRV();
  if (srv) {
    m_pDevice->m_pImmediateContext->GenerateMips(srv);
  }
}

uint32
DX11GraphicsAPI::getBytesFromFormat(const uint32 _format)
{
  // RGBA of value 32
  if (_format == PK_TEXTURE_FORMAT::kPK_FORMAT_R32G32B32A32_FLOAT ||
      _format == PK_TEXTURE_FORMAT::kPK_FORMAT_R32G32B32A32_TYPELESS ||
      _format == PK_TEXTURE_FORMAT::kPK_FORMAT_R32G32B32A32_UINT ||
      _format == PK_TEXTURE_FORMAT::kPK_FORMAT_R32G32B32A32_SINT) {
    return (32 * 4) / 8;
  }
  // RGB of value 32
  if (_format == PK_TEXTURE_FORMAT::kPK_FORMAT_R32G32B32_FLOAT ||
      _format == PK_TEXTURE_FORMAT::kPK_FORMAT_R32G32B32_TYPELESS ||
      _format == PK_TEXTURE_FORMAT::kPK_FORMAT_R32G32B32_UINT ||
      _format == PK_TEXTURE_FORMAT::kPK_FORMAT_R32G32B32_SINT) {
    return (32 * 3) / 8;
  }
  // RGBA of value 16
  if (_format == PK_TEXTURE_FORMAT::kPK_FORMAT_R16G16B16A16_FLOAT ||
      _format == PK_TEXTURE_FORMAT::kPK_FORMAT_R16G16B16A16_TYPELESS ||
      _format == PK_TEXTURE_FORMAT::kPK_FORMAT_R16G16B16A16_UINT ||
      _format == PK_TEXTURE_FORMAT::kPK_FORMAT_R16G16B16A16_SINT) {
    return (16 * 4) / 8;
  }
  // RG of value 16
  if (_format == PK_TEXTURE_FORMAT::kPK_FORMAT_R16G16_FLOAT ||
      _format == PK_TEXTURE_FORMAT::kPK_FORMAT_R16G16_TYPELESS ||
      _format == PK_TEXTURE_FORMAT::kPK_FORMAT_R16G16_UINT ||
      _format == PK_TEXTURE_FORMAT::kPK_FORMAT_R16G16_SINT) {
    return (16 * 2) / 8;
  }
  // RGBA of value 8
  if (_format == PK_TEXTURE_FORMAT::kPK_FORMAT_R8G8B8A8_UNORM ||
      _format == PK_TEXTURE_FORMAT::kPK_FORMAT_R8G8B8A8_TYPELESS ||
      _format == PK_TEXTURE_FORMAT::kPK_FORMAT_R8G8B8A8_UINT ||
      _format == PK_TEXTURE_FORMAT::kPK_FORMAT_R8G8B8A8_SINT || 
      _format == PK_TEXTURE_FORMAT::kPK_FORMAT_B8G8R8A8_UNORM ||
      _format == PK_TEXTURE_FORMAT::kPK_FORMAT_B8G8R8A8_TYPELESS) {
    return (8 * 4) / 8;
  }
  // R of value 8
  if (_format == PK_TEXTURE_FORMAT::kPK_FORMAT_R8_TYPELESS ||
      _format == PK_TEXTURE_FORMAT::kPK_FORMAT_R8_UNORM || 
      _format == PK_TEXTURE_FORMAT::kPK_FORMAT_R8_SINT || 
      _format == PK_TEXTURE_FORMAT::kPK_FORMAT_R8_UINT) {
    return 1;
  }
  return 0;
}

uint32
DX11GraphicsAPI::getFormatFromBytes(const uint32 _bpp)
{
  if (_bpp == 4) {
    return PK_TEXTURE_FORMAT::kPK_FORMAT_R32G32B32A32_FLOAT;
  }
  if (_bpp == 3) {
    return PK_TEXTURE_FORMAT::kPK_FORMAT_R32G32B32_FLOAT;
  }
  if (_bpp == 2) {
    return PK_TEXTURE_FORMAT::kPK_FORMAT_R16G16_FLOAT;
  }
  if (_bpp == 1) {
    return PK_TEXTURE_FORMAT::kPK_FORMAT_R8_UNORM;
  }
  return -1;
}

SPtr<Texture>
DX11GraphicsAPI::createEmptyTexture()
{
  return make_shared<DX11Texture>();
}

SPtr<Texture>
DX11GraphicsAPI::createTexture(const uint32 _width,
                               const uint32 _height,
                               const int32 _format,
                               const int32 _usage,
                               int32 _bindFlags,
                               const int32 _shaderResourceFormat,
                               int32 _mipLevels,
                               const bool _isCube)
{
  PK_ASSERT(m_pDevice);
  Logger& log = g_Logger();

  bool generateMips = (_mipLevels == 0 || _mipLevels > 1);
  if (generateMips) {
    _bindFlags |= D3D11_BIND_RENDER_TARGET;
    _bindFlags |= D3D11_BIND_SHADER_RESOURCE;

    if (_mipLevels == 0) {
      _mipLevels = static_cast<uint32>(Math::log2(Math::max(_width, _height)) + 1);
    }
  }

  // create the texture
  SPtr<DX11Texture> dxTex = make_shared<DX11Texture>();
  dxTex->setSize(Vector2(_width, _height));

  // texture description
  D3D11_TEXTURE2D_DESC desc;
  memset(&desc, 0, sizeof(desc));
  desc.Width = _width;
  desc.Height = _height;
  desc.MipLevels = _mipLevels;
  desc.ArraySize = _isCube? 6 : 1;
  desc.Format = static_cast<DXGI_FORMAT>(_format);
  desc.SampleDesc.Count = 1;
  desc.SampleDesc.Quality = 0;
  desc.Usage = static_cast<D3D11_USAGE>(_usage);
  desc.BindFlags = _bindFlags;
  desc.CPUAccessFlags = _usage == D3D11_USAGE_DYNAMIC ? D3D11_CPU_ACCESS_WRITE : 0;
  desc.MiscFlags = 0;

  if (generateMips) {
    desc.MiscFlags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;
  }

  if (_isCube) {
    desc.MiscFlags |= D3D11_RESOURCE_MISC_TEXTURECUBE;
  }

  // create the texture
  int32 hr = m_pDevice->m_pd3dDevice->CreateTexture2D(&desc, nullptr, &dxTex->m_t2d);

  // if texture creation failed
  if (FAILED(hr)) {
    const String errMsg = log.getMessageError(hr);
    const String msg = "Failed to create a texture. Error message: " + errMsg;
    log.registerMessage(msg, __FILE__, __LINE__, LOG_MSG_TYPE::kError);
    return nullptr;
  }
  /**
   * Create shader resource.
   */
  if ((_bindFlags & D3D11_BIND_SHADER_RESOURCE) == D3D11_BIND_SHADER_RESOURCE) {
    // Create the shader resource descriptor for the texture
    D3D11_SHADER_RESOURCE_VIEW_DESC sDesc;
    memset(&sDesc, 0, sizeof(sDesc));
    sDesc.Format = static_cast<DXGI_FORMAT>(_shaderResourceFormat);
    sDesc.Texture2D.MipLevels = (_mipLevels == 0) ? -1 : desc.MipLevels;
    sDesc.Texture2D.MostDetailedMip = 0;
    sDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

    if (_isCube) {
      sDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
    }

    // create the shader resource view
    hr = m_pDevice->m_pd3dDevice->CreateShaderResourceView(dxTex->m_t2d, &sDesc, &dxTex->m_sRV);
    // if failed to create shader resource view
    if (FAILED(hr)) {
      const String errMsg = log.getMessageError(hr);
      const String msg = "Failed to create a shader resource view. Error message: " + errMsg;
      log.registerMessage(msg, __FILE__, __LINE__, LOG_MSG_TYPE::kError);
      return nullptr;
    }
  }
  /**
   * Create Depth stencil
   */
  if ((_bindFlags & D3D11_BIND_DEPTH_STENCIL) == D3D11_BIND_DEPTH_STENCIL) {
    // texture description
    D3D11_TEXTURE2D_DESC descDepth;
    ZeroMemory(&descDepth, sizeof(descDepth));
    descDepth.Width = _width;
    descDepth.Height = _height;
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_D32_FLOAT; // DXGI_FORMAT_D24_UNORM_S8_UINT;
    descDepth.SampleDesc.Count = 1;
    descDepth.SampleDesc.Quality = 0;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    descDepth.CPUAccessFlags = 0;
    descDepth.MiscFlags = 0;

    // create the depth stencil side of the texture
    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
    dsvDesc.Format = DXGI_FORMAT_D32_FLOAT; // DXGI_FORMAT_D24_UNORM_S8_UINT;
    dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    dsvDesc.Texture2D.MipSlice = 0;

    hr = m_pDevice->m_pd3dDevice->CreateDepthStencilView(dxTex->m_t2d, &dsvDesc, &dxTex->m_dSV);
    if (!dxTex->m_dSV) {
      const String errMsg = log.getMessageError(hr);
      const String msg = "Failed to create the depth stencil. Error message: " + errMsg;
      log.registerMessage(msg, __FILE__, __LINE__, LOG_MSG_TYPE::kError);
      return nullptr;
    }
  }
  /**
   * Create render target
   */
  if ((_bindFlags & D3D11_BIND_RENDER_TARGET) == D3D11_BIND_RENDER_TARGET) {
    // render target description
    D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
    memset(&rtvDesc, 0, sizeof(rtvDesc));
    rtvDesc.Format = desc.Format;
    rtvDesc.Texture2D.MipSlice = 0;

    // if it's a cube texture.
    if (_isCube) {
      rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
      dxTex->m_rTVs.resize(6 * _mipLevels);
      
      for (uint32 cubeFace = 0; cubeFace < 6; ++cubeFace) {
        for (int32 mip = 0; mip < _mipLevels; ++mip) {
          rtvDesc.Texture2DArray.FirstArraySlice = cubeFace;
          rtvDesc.Texture2DArray.MipSlice = mip;
          rtvDesc.Texture2DArray.ArraySize = 1;

          const uint32 index = cubeFace * _mipLevels + mip;
          hr = m_pDevice->m_pd3dDevice->CreateRenderTargetView(dxTex->m_t2d,
                                                               &rtvDesc,
                                                               &dxTex->m_rTVs[index]);

          if (FAILED(hr)) {
            const String errMsg = log.getMessageError(hr);
            const String msg = "Failed to create a render target view. Error message: " +
                               errMsg;
            log.registerMessage(msg, __FILE__, __LINE__, LOG_MSG_TYPE::kError);
            return nullptr;
          }
        }
      }
    }
    // if it's not a cube texture.
    else {
      rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
      dxTex->m_rTVs.resize(_mipLevels);
      for (int32 i = 0; i < _mipLevels; ++i) {
        rtvDesc.Texture2D.MipSlice = i;
        hr = m_pDevice->m_pd3dDevice->CreateRenderTargetView(dxTex->m_t2d,
                                                             &rtvDesc,
                                                             &dxTex->m_rTVs[i]);
        if (FAILED(hr)) {
          const String errMsg = log.getMessageError(hr);
          const String msg = "Failed to create a render target view. Error message: " + errMsg;
          log.registerMessage(msg, __FILE__, __LINE__, LOG_MSG_TYPE::kError);
          return nullptr;
        }
      }
    }
  }
  /**
   * Create unordered access
   */
  if ((_bindFlags & D3D11_BIND_UNORDERED_ACCESS) == D3D11_BIND_UNORDERED_ACCESS) {
    D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
    memset(&uavDesc, 0, sizeof(uavDesc));
    uavDesc.Format = desc.Format;
    uavDesc.Texture2D.MipSlice = 0;
    uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
    dxTex->m_uAVs.resize(_mipLevels);

    for (int32 i = 0; i < _mipLevels; ++i) {
      uavDesc.Texture2D.MipSlice = i;
      hr = m_pDevice->m_pd3dDevice->CreateUnorderedAccessView(dxTex->m_t2d,
                                                              &uavDesc,
                                                              &dxTex->m_uAVs[i]);
      if (FAILED(hr)) {
        const String errMsg = log.getMessageError(hr);
        const String msg = "Failed to create an unordered access view. Error message: " +
                            errMsg;
        log.registerMessage(msg, __FILE__, __LINE__, LOG_MSG_TYPE::kError);
        return nullptr;
      }
    }
  }

  return dxTex;
}

void
DX11GraphicsAPI::setInputLayout( const SPtr<InputLayout> _pInputLayout)
{
  // reinterpret to a DirectX input layout
  const SPtr<DX11InputLayout> dxIL = reinterpret_pointer_cast<DX11InputLayout>(_pInputLayout);
  // if the layout is not null but failed to reinterpret
  if (_pInputLayout && !dxIL) {
    const String msg = "Failed to set the input layout.";
    g_Logger().registerMessage(msg, __FILE__, __LINE__, LOG_MSG_TYPE::kError);
    return;
  }
  // get the dx11 input layout
  const auto inputLayout = reinterpret_pointer_cast<DX11InputLayout>(_pInputLayout);
  // set the input layout
  m_pDevice->m_pImmediateContext->IASetInputLayout(inputLayout ? inputLayout->m_pVertexLayout : 
                                                   nullptr);
}

SPtr<VertexBuffer>
DX11GraphicsAPI::createVertexBuffer(const Vector<SimpleVertex>& _vertex,
                                    const uint32 _usage)
{
  Logger& log = g_Logger();
  auto dxVB = make_shared<DX11VertexBuffer>();
  /***************************************************************/
  /**
  * Define and create the buffer
  **/
  /***************************************************************/
  const uint32 byteWidth = static_cast<uint32>(sizeof(SimpleVertex) * _vertex.size());
  D3D11_BUFFER_DESC bd;
  memset(&bd, 0, sizeof(bd));
  bd.ByteWidth = byteWidth; // size of the buffer
  bd.Usage = static_cast<D3D11_USAGE>(_usage); // how it is expected to be read
  bd.BindFlags = D3D11_BIND_VERTEX_BUFFER; // how it will be binded to the pipeline
  bd.CPUAccessFlags = 0; // default -> CPU ha no accesss to this
  bd.MiscFlags = 0;

  // info descriptor
  D3D11_SUBRESOURCE_DATA InitData;
  memset(&InitData, 0, sizeof(InitData));
  InitData.pSysMem = _vertex.data(); // pointer to the initialization data
  InitData.SysMemPitch = byteWidth; // distance between values

  const uint32 hr = m_pDevice->m_pd3dDevice->CreateBuffer(&bd, &InitData, &dxVB->pBuffer);
  if (FAILED(hr)) {
    const String errMsg = log.getMessageError(hr);
    const String msg = "Failed to create a vertex buffer. Error message: " + errMsg;
    log.registerMessage(msg, __FILE__, __LINE__, LOG_MSG_TYPE::kError);
    return nullptr;
  }
  return dxVB;
}

void
DX11GraphicsAPI::setVertexBuffer(const SPtr<VertexBuffer>& _pVertexB,
                                 const uint32 _start,
                                 const uint32 _bufferCount,
                                 const uint32 _offset)
{
  // reinterpret vertex buffer.
  const auto dxVB = reinterpret_pointer_cast<DX11VertexBuffer>(_pVertexB);
  if (!dxVB) {
    const String msg = "Failed to set the vertex buffer.";
    g_Logger().registerMessage(msg, __FILE__, __LINE__, LOG_MSG_TYPE::kError);
    return;
  }
  
  // get the offset
  const uint32 stride = sizeof(SimpleVertex);
  //set the buffer
  m_pDevice->m_pImmediateContext->IASetVertexBuffers(_start,
                                                     _bufferCount,
                                                     &dxVB->pBuffer,
                                                     &stride,
                                                     &_offset);
}

SPtr<IndexBuffer>
DX11GraphicsAPI::createIndexBuffer(const Vector<uint32>& _index,
                                   const uint32 _usage)
{
  Logger& log = g_Logger();
  auto dxIB = make_shared<DX11IndexBuffer>();
  /***************************************************************/
  /**
  * Define and create the buffer
  **/
  /***************************************************************/
  D3D11_BUFFER_DESC bd;
  memset(&bd, 0, sizeof(bd));
  bd.ByteWidth = sizeof(uint32) * static_cast<uint32>(_index.size()); // size of the buffer
  bd.Usage = static_cast<D3D11_USAGE>(_usage); // how it is expected to be read
  bd.BindFlags = D3D11_BIND_INDEX_BUFFER; // how it will be binded to the pipeline
  bd.CPUAccessFlags = 0; // default -> CPU ha no accesss to this
  bd.MiscFlags = 0;

  D3D11_SUBRESOURCE_DATA InitData; // info descriptor
  memset(&InitData, 0, sizeof(InitData));
  InitData.pSysMem = _index.data(); // pointer to the initialization data
  // create the buffer
  const int32 hr = m_pDevice->m_pd3dDevice->CreateBuffer(&bd, &InitData, &dxIB->pBuffer);
  if (FAILED(hr)) {
    const String errMsg = log.getMessageError(hr);
    const String msg = "failed to create an index buffer. Error message: " + errMsg;
    log.registerMessage(msg, __FILE__, __LINE__, LOG_MSG_TYPE::kError);
    return nullptr;
  }
  return dxIB;
}

void
DX11GraphicsAPI::setIndexBuffer(const SPtr<IndexBuffer>& _pIndexB,
                                const uint32 _format,
                                const uint32 _offset)
{
  // reinterpret pointer
  auto dxIB = reinterpret_pointer_cast<DX11IndexBuffer>(_pIndexB);
  // if failed to cast to DX11IndexBuffer
  if (!dxIB) {
    const String msg = "Failed to set the index buffer.";
    g_Logger().registerMessage(msg, __FILE__, __LINE__, LOG_MSG_TYPE::kError);
    return;
  }
  m_pDevice->m_pImmediateContext->IASetIndexBuffer(dxIB->pBuffer,
                                                   static_cast<DXGI_FORMAT>(_format),
                                                   _offset);
}
}
#endif