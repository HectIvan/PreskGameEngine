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
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ENABLE_OPENEXR
#include "stb_image.h"

#include "pkLogger.h"
#include "pkDX11BlendState.h"
#include "pkDX11ComputeShader.h"
#include "pkDX11GraphicsAPI.h"
#include "pkDX11IndexBuffer.h"
#include "pkDX11PixelShader.h"
#include "pkDX11Prerequisites.h"
#include "pkDX11RasterizerState.h"
#include "pkDX11RenderTargetView.h"
#include "pkDX11SamplerState.h"
#include "pkDX11SwapChain.h"
#include "pkDX11VertexBuffer.h"
#include "pkDX11VertexShader.h"
#include "pkVertexBuffer.h"

#include "pkDX11InputLayout.h"

#if PK_PLATFORM == PK_PLATFORM_WIN32
#include <d3dcompiler.h>

namespace pkEngineSDK
{
class ShaderInclude : public ID3DInclude
{
public:
  HRESULT __stdcall Open(D3D_INCLUDE_TYPE, LPCSTR pFileName,
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

    char* buffer = new char[size];
    file.read(buffer, size);

    *ppData = buffer;
    *pBytes = static_cast<UINT>(size);
    return S_OK;
  }

  HRESULT __stdcall Close(LPCVOID pData) noexcept override {
    delete[] static_cast<const char*>(pData);
    return S_OK;
  }
};

FORCEINLINE void
throwIfFailed(HRESULT hr) {
  if (FAILED(hr)) {
    PK_ASSERT(false && "Error in creation");
  }
}

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
  
  auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
  if (!device) {
    g_Logger().print("Failed to utilize the DX device in the API Initialization");
    return;
  }
  device->setPrimitiveTopology();
}

SPtr<ConstantBuffer>
DX11GraphicsAPI::createConstantBuffer(uint32 _size, const void* _pData, uint32 _usage)
{
  Logger& log = g_Logger().instance();
  auto dxCB = make_shared<DX11ConstantBuffer>();
  int32 hr;
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

  auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
  if (!device) {
    const String msg = "Failed to utilize the DX device in the creation of a CBuffer.";
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kError);
    return nullptr;
  }
  hr = device->m_pd3dDevice->CreateBuffer(&bDesc,
                                          _pData ? &subData : nullptr,
                                          &dxCB->pCBuffer);
  if (FAILED(hr)) {
    String errMsg = log.getMessageError(hr);
    const String msg = "Failed to create the DX constant buffer. Error message: " + errMsg;
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kError);
    return nullptr;
  }
  log.registerMessage("Created a DirectX constant buffer");
  return dxCB;
}

void
DX11GraphicsAPI::updateConstantBuffer(SPtr<ConstantBuffer> _pCBuffer,
                                      const void* _pNewData,
                                      SIZE_T _size)
{
  // cast to DX11ConstantBuffer
  auto dxCB = reinterpret_pointer_cast<DX11ConstantBuffer>(_pCBuffer);
  // casting failed
  if (!dxCB) {
    g_Logger().print("Failed to update constant buffers.");
    return;
  }
  // update with the new data
  auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
  if (!device) {
    g_Logger().print("Failed to utilize the DX device in the update of a CBuffer");
    return;
  }
  uint32 size = static_cast<uint32>(_size);
  device->m_pImmediateContext->UpdateSubresource(dxCB->pCBuffer,
                                                 0,
                                                 nullptr,
                                                 _pNewData,
                                                 size,
                                                 0);
}

void
DX11GraphicsAPI::drawIndexed(uint32 _indexCount,
                             uint32 _startIndexLocation,
                             uint32 _baseVertexLocation)
{
  // draw the data
  auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
  if (!device) {
    g_Logger().print("Failed to utilize the DX device in the Draw Indexed call");
    return;
  }
  device->m_pImmediateContext->DrawIndexed(_indexCount,
                                           _startIndexLocation,
                                           _baseVertexLocation);
}

void
DX11GraphicsAPI::draw(uint32 _indexCount, uint32 _startIndexLocation)
{
  // draw the data
  auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
  if (!device) {
    g_Logger().print("Failed to utilize the DX device in the Draw call");
    return;
  }
  device->m_pImmediateContext->Draw(_indexCount,
                                    _startIndexLocation);
}

void
DX11GraphicsAPI::dispatch(uint32 _countX, uint32 _countY, uint32 _countZ)
{
  // draw the data
  auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
  if (!device) {
    g_Logger().print("Failed to utilize the DX device in the Dispatch call");
    return;
  }
  device->m_pImmediateContext->Dispatch(_countX, _countY, _countZ);
}

void
DX11GraphicsAPI::clearRenderTargetViews(const Color& _color, Vector<SPtr<Texture>> _rtvs)
{
  for (uint32 i = 0; i < _rtvs.size(); ++i) {
    clearRenderTargetView(_color, _rtvs[i]);
  }
}

void
DX11GraphicsAPI::clearRenderTargetView(const Color& _color, SPtr<Texture> _rtv)
{
  PK_ASSERT(_rtv);
  // Texture to a DirectX texture
  SPtr<DX11Texture> dxRTV = reinterpret_pointer_cast<DX11Texture>(_rtv);
  // If the casting failed.
  if (!dxRTV) {
    g_Logger().print("Failed to clear the render target view");
    return;
  }
  // clear the render target
  auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
  if (!device) {
    g_Logger().print("Failed to utilize the DX device in the clearing of the Render TV.");
    return;
  }
  float color[4] = { static_cast<float>(_color.getR()),
                     static_cast<float>(_color.getG()),
                     static_cast<float>(_color.getB()),
                     static_cast<float>(_color.getA()) };
  device->m_pImmediateContext->ClearRenderTargetView(dxRTV->m_rTV, color);
}

void
DX11GraphicsAPI::clearUnorderedAccessViews(Vector<SPtr<Texture>> _uavs, const Color& _color)
{
  for (uint32 i = 0; i < _uavs.size(); ++i) {
    clearUnorderedAccessView(_uavs[i], _color);
  }
}

void
DX11GraphicsAPI::clearUnorderedAccessView(SPtr<Texture> _uav, const Color& _color)
{
  PK_ASSERT(_uav);
  // Texture to a DirectX texture
  SPtr<DX11Texture> dxUAV = reinterpret_pointer_cast<DX11Texture>(_uav);
  // If the casting failed.
  if (!dxUAV) {
    g_Logger().print("Failed to clear the unordered access view");
    return;
  }
  // clear the unordered access view
  auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
  if (!device) {
    g_Logger().print("Failed to utilize the DX device in the clearing of the unordered AV.");
    return;
  }
  float color[4] = { static_cast<float>(_color.getR()),
                     static_cast<float>(_color.getG()),
                     static_cast<float>(_color.getB()),
                     static_cast<float>(_color.getA()) };
  device->m_pImmediateContext->ClearUnorderedAccessViewFloat(dxUAV->m_uAV, color);
}

void
DX11GraphicsAPI::clearDepthBuffer(float _depth, SPtr<Texture> _pDepthSV)
{
  // check if a depth stencil is being sent
  if (_pDepthSV) {
    PK_ASSERT(_pDepthSV);
    // Cast to a DirectX depth stencil.
    auto dxDSV = reinterpret_pointer_cast<DX11Texture>(_pDepthSV);
    // If the casting failed.
    if (!dxDSV) {
      g_Logger().print("Failed to clear depth stencil view.");
      return;
    }
    // Clear the depth buffer to 1.0 (max depth)
    auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
    if (!device) {
      g_Logger().print("Failed to utilize the DX device in the clearing of the depth buffer.");
      return;
    }
    device->m_pImmediateContext->ClearDepthStencilView(dxDSV->m_dSV,
                                                       D3D11_CLEAR_DEPTH,
                                                       _depth,
                                                       0);
  }
}

SPtr<Shader>
DX11GraphicsAPI::createVShader(SPtr<Shader> _pShader)
{
  PK_ASSERT(_pShader);

  Logger& log = g_Logger().instance();
  // convert from shader to dx vertex shader
  SPtr<DX11VertexShader> dxVShader = reinterpret_pointer_cast<DX11VertexShader>(_pShader);
  // SPtr<DX11VertexShader> dxVShader = make_shared<DX11VertexShader>();
  // create the vertex shader
  uint32 hr;
  auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
  if (!device) {
    const String msg = "Failed to utilize the DX device in the VShader creation.";
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kError);
    return nullptr;
  }
  hr = device->m_pd3dDevice->CreateVertexShader(dxVShader->m_pSBlob->GetBufferPointer(),
                                                dxVShader->m_pSBlob->GetBufferSize(),
                                                nullptr,
                                                &dxVShader->m_pShader);
  // check if the creation was successful
  if (hr != 0x00000000) {
    const String errMsg = log.getMessageError(hr);
    const String msg = "Failed to create a DX Vertex Shader. Error message: " + errMsg;
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kError);
    dxVShader->m_pSBlob->Release();
    return nullptr;
  }
  log.registerMessage("Created a DirectX Vertex Shader.");
  return dxVShader;
}

SPtr<Shader>
DX11GraphicsAPI::createPShader(SPtr<Shader> _pShader)
{
  PK_ASSERT(_pShader);

  Logger& log = g_Logger().instance();
  // convert from shader to dx pixel shader
  SPtr<DX11PixelShader> dxPShader = reinterpret_pointer_cast<DX11PixelShader>(_pShader);
  uint32 hr;
  // create the pixel shader
  auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
  if (!device) {
    const String msg = "Failed to utilize the DX device in the PShader creation.";
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kError);
    return nullptr;
  }
  hr = device->m_pd3dDevice->CreatePixelShader(dxPShader->m_pSBlob->GetBufferPointer(),
                                               dxPShader->m_pSBlob->GetBufferSize(),
                                               nullptr,
                                               &dxPShader->m_pShader);
  // check if the creation was successful
  if (FAILED(hr)) {
    const String errMsg = log.getMessageError(hr);
    const String msg = "Failed to create a DX Pixel Shader. Error message: " + errMsg;
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kError);
    dxPShader->m_pSBlob->Release();
    return nullptr;
  }
  log.registerMessage("Created a DirectX Pixel Shader.");
  return dxPShader;
}

SPtr<Shader>
DX11GraphicsAPI::createCShader(SPtr<Shader> _pShader)
{
  PK_ASSERT(_pShader);

  Logger& log = g_Logger().instance();
  // convert from shader to dx compute shader
  SPtr<DX11ComputeShader> dxCShader = reinterpret_pointer_cast<DX11ComputeShader>(_pShader);
  uint32 hr;
  // create the comptue shader
  auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
  if (!device) {
    const String msg = "Failed to utilize the DX device in the CShader creation.";
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kError);
    return nullptr;
  }
  hr = device->m_pd3dDevice->CreateComputeShader(dxCShader->m_pSBlob->GetBufferPointer(),
                                                 dxCShader->m_pSBlob->GetBufferSize(),
                                                 nullptr,
                                                 &dxCShader->m_pShader);
  // check if the creation was successful
  if (FAILED(hr)) {
    const String errMsg = log.getMessageError(hr);
    const String msg = "Failed to create a DX Compute Shader. Error message: " + errMsg;
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kError);
    dxCShader->m_pSBlob->Release();
    return nullptr;
  }
  log.registerMessage("Created a DirectX Compute Shader.");
  return dxCShader;
}


void
DX11GraphicsAPI::setVShader(const SPtr<Shader> _pShader)
{
  PK_ASSERT(m_pDevice);
  // reinterpret as a DirectX vertex shader
  SPtr<DX11VertexShader> dxVShader = reinterpret_pointer_cast<DX11VertexShader>(_pShader);
  if (_pShader && !dxVShader) {
    g_Logger().print("Failed to set a vertex shader.");
    return;
  }
  // set the shader
  auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
  if (!device) {
    g_Logger().print("Failed to utilize the DX device in the setting of a vertex shader.");
    return;
  }
  device->m_pImmediateContext->VSSetShader(_pShader ? dxVShader->m_pShader : nullptr,
                                           nullptr,
                                           0);
}

void
DX11GraphicsAPI::setPShader(const SPtr<Shader> _pShader)
{
  PK_ASSERT(m_pDevice);
  // reinterpret as a DirectX pixel shader
  SPtr<DX11PixelShader> dxPShader = reinterpret_pointer_cast<DX11PixelShader>(_pShader);
  if (_pShader && !dxPShader) {
    g_Logger().print("Failed to set a pixel shader.");
    return;
  }
  // set the shader
  auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
  if (!device) {
    g_Logger().print("Failed to utilize the DX device in the setting of a pixel shader.");
    return;
  }
  device->m_pImmediateContext->PSSetShader(_pShader ? dxPShader->m_pShader : nullptr,
                                           nullptr,
                                           0);
}

void
DX11GraphicsAPI::setCShader(const SPtr<Shader> _pShader)
{
  PK_ASSERT(m_pDevice);
  // reinterpret as a DirectX compute shader
  SPtr<DX11ComputeShader> dxCShader = reinterpret_pointer_cast<DX11ComputeShader>(_pShader);
  if (_pShader && !dxCShader) {
    g_Logger().print("Failed to set a compute shader.");
    return;
  }
  // set the shader
  auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
  if (!device) {
    g_Logger().print("Failed to utilize the DX device in the setting of a compute shader.");
    return;
  }
  device->m_pImmediateContext->CSSetShader(_pShader ? dxCShader->m_pShader : nullptr,
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
  Logger& log = g_Logger().instance();
  // initialize device and swap chain
  m_pDevice = make_shared<DX11Device>();
  SPtr<DX11SwapChain> pSwapChain = make_shared<DX11SwapChain>();
  pSwapChain->setHeight(_height);
  pSwapChain->setWidth(_width);
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
  auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
  if (!device) {
    const String msg = "Failed to utilize the DX device in the device creation.";
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kError);
    return;
  }
  for (uint32 driverTypeIndex = 0; driverTypeIndex < _numDriverTypes; driverTypeIndex++) {
    // try and create the device and swap chain with the current driver type
    device->m_pDriverType = new D3D_DRIVER_TYPE(_driverTypes[driverTypeIndex]);
    int32 hr = D3D11CreateDeviceAndSwapChain(nullptr,
                                             *device->m_pDriverType,
                                             nullptr,
                                             _createDeviceFlags,
                                             _featureLevels,
                                             _numFeatureLevels,
                                             D3D11_SDK_VERSION,
                                             &sd,
                                             &pSwapChain->m_pSch,
                                             &device->m_pd3dDevice,
                                             &device->m_featureLevel,
                                             &device->m_pImmediateContext);

    // if creation was successful
    if (hr == 0x00000000) {
      // end the entire process, no need to continue
      log.registerMessage("Created a DirectX device and Swap Chain.");
      m_pSwapChain = pSwapChain;
      break;
    }
  }
}

void
DX11GraphicsAPI::setRenderTargets(Vector<SPtr<Texture>> _rTargets,
                                  SPtr<Texture> _pDepthSV)
{
  Logger& log = g_Logger().instance();
  // reinterpret the depth stencil view to a DirectX texture
  auto pDSV = reinterpret_pointer_cast<DX11Texture>(_pDepthSV);
  // render target vector
  Vector<ID3D11RenderTargetView*> rTVector;
  // get the vector size
  uint32 RTCount = static_cast<uint32>(_rTargets.size());
  // reinterpret each of the targets as a DX11 texture and store in the texture vector
  for (uint32 i = 0; i < RTCount; ++i) {
    SPtr<DX11Texture> dxTx = reinterpret_pointer_cast<DX11Texture>(_rTargets[i]);
    // if target is valid, store it
    if (_rTargets[i] && dxTx->m_rTV) { rTVector.push_back(dxTx->m_rTV); }
    // if its not, save a null pointer
    else { rTVector.push_back(nullptr); }
  }
  // set the render targets
  auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
  if (!device) {
    const String msg = "Failed to utilize the DX device in the setting of the render targets.";
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kError);
    return;
  }
  device->m_pImmediateContext->OMSetRenderTargets(static_cast<uint32>(rTVector.size()),
                                                  rTVector.data(),
                                                  (pDSV) ? pDSV->m_dSV : nullptr);
}

void
DX11GraphicsAPI::unbindRenderTargets()
{
  Logger& log = g_Logger().instance();
  static Vector<ID3D11RenderTargetView*> unbindRT = { nullptr, nullptr, nullptr, nullptr,
                                                      nullptr, nullptr, nullptr, nullptr };
  // set the render targets
  auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
  if (!device) {
    const String msg = "Failed to utilize the DX device in the setting of the render targets.";
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kError);
    return;
  }

  // const uint32 size = static_cast<uint32>(unbindRT.size());
  device->m_pImmediateContext->OMSetRenderTargets(0,
                                                  unbindRT.data(),
                                                  nullptr);
}

void
DX11GraphicsAPI::setRenderTarget(const SPtr<Texture> _pRTarget, SPtr<Texture> _pDepthSV)
{
  Logger& log = g_Logger().instance();
  // reinterpet render target
  auto rTarget = reinterpret_pointer_cast<DX11Texture>(_pRTarget);
  // reinterpret the depth stencil view
  auto pDSV = reinterpret_pointer_cast<DX11Texture>(_pDepthSV);
  // reinterpet device
  auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
  if (!device) {
    const String msg = "Failed to utilize the DX device in the setting of the render target.";
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kError);
    return;
  }
  device->m_pImmediateContext->OMSetRenderTargets(rTarget ? 1 : 0,
                                                  rTarget ?  &rTarget->m_rTV : nullptr,
                                                  pDSV ? pDSV->m_dSV : nullptr);
}

SPtr<SamplerState>
DX11GraphicsAPI::createSamplerState(const uint32 _mode, const uint32 _filter)
{
  Logger& log = g_Logger().instance();
  // sampler state creation
  SPtr<DX11SamplerState> pSamState = make_shared<DX11SamplerState>();
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

  auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
  if (!device) {
    const String msg = "Failed to utilize the DX device in the sampler state creation.";
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kError);
    return nullptr;
  }
  int32 hr = device->m_pd3dDevice->CreateSamplerState(&sampDesc, &pSamState->m_pSampler);
  if (hr != 0x00000000) {
    String errMsg = g_Logger().getMessageError(hr);
    const String msg = "Failed to create a sampler state. Error message: " + errMsg;
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kError);
    return nullptr;
  }
  return pSamState;
}

void
DX11GraphicsAPI::setViewport(uint32 _width,
                             uint32 _height)
{
  Logger& log = g_Logger().instance();
  PK_ASSERT(m_pDevice);
  D3D11_VIEWPORT vp;
  vp.Width = static_cast<float>(_width);
  vp.Height = static_cast<float>(_height);
  vp.MinDepth = 0.0f;
  vp.MaxDepth = 1.0f;
  vp.TopLeftX = 0;
  vp.TopLeftY = 0;
  auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
  if (!device) {
    const String msg = "Failed to utilize the DX device in the setting of the viewport.";
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kError);
    return;
  }
  device->m_pImmediateContext->RSSetViewports(1, &vp);
}

SPtr<BlendState>
DX11GraphicsAPI::createBlendState()
{
  Logger& log = g_Logger().instance();
  // create the blend state
  SPtr<DX11BlendState> pBlendState = make_shared<DX11BlendState>();
  PK_ASSERT(pBlendState);
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

  // Create the blend state.
  auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
  if (!device) {
    const String msg = "Failed to utilize the DX device in the creation of a blend state.";
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kError);
    return nullptr;
  }
  int32 hr = device->m_pd3dDevice->CreateBlendState(&blendDesc, &pBlendState->m_pBlendState);

  if (FAILED(hr)) {
    const String errMsg = log.getMessageError(hr);
    const String msg = "Failed to create Blend state. Error message: " + errMsg;
    log.registerMessage(msg, LOG_MSG_TYPE::kError);
    return nullptr;
  }
  log.registerMessage("Created a Blend State.");
  return pBlendState;
}

SPtr<RasterizerState>
DX11GraphicsAPI::createRasterizerState(const RASTERIZER_DESC& _desc)
{
  Logger& log = g_Logger().instance();
  // create the rasterizer state
  SPtr<DX11RasterizerState> dxRS = make_shared<DX11RasterizerState>();
  dxRS->m_pRasterizer = nullptr;
  // rasterizer description for directx
  D3D11_RASTERIZER_DESC rDesc = {};
  rDesc.FillMode = static_cast<D3D11_FILL_MODE>(_desc.fillMode);
  rDesc.CullMode = static_cast<D3D11_CULL_MODE>(_desc.cullMode);
  rDesc.FrontCounterClockwise = _desc.frontCounterClockwise;
  rDesc.DepthClipEnable = _desc.depthClipEnable;

  // Create the rasterizer state.
  auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
  if (!device) {
    const String msg = "Failed to utilize the DX device in the creation of a Rasterizer State.";
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kError);
    return nullptr;
  }

  HRESULT hr = device->m_pd3dDevice->CreateRasterizerState(&rDesc, &dxRS->m_pRasterizer);
  if (FAILED(hr)) {
    const String errMsg = log.getMessageError(hr);
    const String msg = "Failed to create a Rasterizer State. Error message: " + errMsg;
    log.registerMessage(msg, LOG_MSG_TYPE::kError);
    return nullptr;
  }
  log.registerMessage("Created a Rasterizer State.");
  return dxRS;
}

void
DX11GraphicsAPI::setBlendState(const SPtr<BlendState> _pBlendState)
{
  PK_ASSERT(_pBlendState);
  PK_ASSERT(m_pDevice);

  Logger& log = g_Logger().instance();
  // Reinterpret to a DirectX Blend State
  SPtr<DX11BlendState> dxBS = reinterpret_pointer_cast<DX11BlendState>(_pBlendState);
  if (!dxBS) {
    const String msg = "Failed to reinterpret a DX Blend State.";
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kError);
    return;
  }
  // Set the blend state
  auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
  if (!device) {
    const String msg = "Failed to utilize the DX device in the setting of a blend state.";
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kError);
    return;
  }
  device->m_pImmediateContext->OMSetBlendState(dxBS->m_pBlendState, nullptr, 0xFFFFFFFF);
}

void
DX11GraphicsAPI::setRasterizerState(const SPtr<RasterizerState> _pRasterizerState)
{
  PK_ASSERT(m_pDevice);

  Logger& log = g_Logger().instance();
  // Reinterpret to a DirectX Rasterizer State
  SPtr<DX11RasterizerState> dxRS =
       reinterpret_pointer_cast<DX11RasterizerState>(_pRasterizerState);
  // Set the rasterizer state
  auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
  if (!device) {
    const String msg = "Failed to utilize the DX device in the setting of a Rasterizer state.";
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kError);
    return;
  }
  device->m_pImmediateContext->RSSetState(dxRS ? dxRS->m_pRasterizer : nullptr);
}

void**
DX11GraphicsAPI::compileShaderFromFile(Path _szFileName,
                                       const char* _szEntryPoint,
                                       const char* _szShaderModel)
{
  Logger& log = g_Logger().instance();

  int32 hr = S_OK;

  DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
  // Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
  // Setting this flag improves the shader debugging experience, but still allows 
  // the shaders to be optimized and to run exactly the way they will run in 
  // the release configuration of this program.
  dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif
  static ShaderInclude shaderInclude;
  ID3DBlob* pErrorBlob = nullptr;
  ID3DBlob* dxBlob = nullptr;
  hr = D3DCompileFromFile(_szFileName.getPathWStr().c_str(),
                          nullptr,
                          &shaderInclude,
                          _szEntryPoint,
                          _szShaderModel,
                          dwShaderFlags,
                          0,
                          &dxBlob,
                          &pErrorBlob);
  if (FAILED(hr)) {
    if (pErrorBlob != nullptr) {
      const String error = static_cast<char*>(pErrorBlob->GetBufferPointer());
      const String msg = "Shader failed to compile. Error message:" + error;
      log.print(msg);
      log.registerMessage(msg, LOG_MSG_TYPE::kError);
      pErrorBlob->Release();
    }
    safeRelease(pErrorBlob);
    return nullptr;
  }
  return reinterpret_cast<void**>(dxBlob);
}

SPtr<InputLayout>
DX11GraphicsAPI::createInputLayoutFromVShader(const SPtr<Shader> _pShader)
{
  PK_ASSERT(_pShader);
  PK_ASSERT(m_pDevice);

  Logger& log = g_Logger().instance();
  // create the input layout pointer
  SPtr<DX11InputLayout> pLayout = make_shared<DX11InputLayout>();
  // reinterpret to a DirectX vertex shader
  SPtr<DX11VertexShader> dxVShader = reinterpret_pointer_cast<DX11VertexShader>(_pShader);

  ID3D11ShaderReflection* pVShaderReflection = nullptr;
  throwIfFailed(D3DReflect(dxVShader->m_pSBlob->GetBufferPointer(),
                           dxVShader->m_pSBlob->GetBufferSize(),
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
  auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
  if (!device) {
    const String msg = "Failed to utilize the DX device in the creation of an input layout.";
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kError);
    return nullptr;
  }
  device->m_pd3dDevice->CreateInputLayout(&inputLayoutDesc[0],
                                          static_cast<uint32>(inputLayoutDesc.size()),
                                          dxVShader->m_pSBlob->GetBufferPointer(),
                                          dxVShader->m_pSBlob->GetBufferSize(),
                                          &pLayout->m_pVertexLayout);
  if (!pLayout) {
    const String msg = "Failed to create the input layout.";
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kError);
    return nullptr;
  }
  return pLayout;
}

SPtr<Texture>
DX11GraphicsAPI::createTexture(const TextureDesc& _desc)
{
  return createTexture(_desc.bpp, _desc.width, _desc.height, _desc.format, _desc.usage,
                       _desc.bindFlags, _desc.shaderResourceFormat, _desc.mipLevels,
                       _desc.miscFlags, _desc.data);
}

SPtr<InputLayout>
DX11GraphicsAPI::createInputLayout(const Vector<InputDesc>& _vDesc,
                                   const SPtr<Shader> _pVShader)
{
  PK_ASSERT(m_pDevice);
  PK_ASSERT(_pVShader);

  Logger& log = g_Logger().instance();
  // make a shared DX11InputLayout pointer
  SPtr<DX11InputLayout> pInputL = make_shared<DX11InputLayout>();
  SPtr<DX11VertexShader> dxVShader = reinterpret_pointer_cast<DX11VertexShader>(_pVShader);
  
  int32 hr;
  // define the input layout
  Vector<D3D11_INPUT_ELEMENT_DESC> dxLayout;
  dxLayout.resize(_vDesc.size());

  uint32 offset = 0;

  for (uint32 i = 0; i < _vDesc.size(); ++i) {
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

  // create input layout
  auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
  if (!device) {
    const String msg = "Failed to utilize the DX device in the creation of an input layout.";
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kError);
    return nullptr;
  }
  hr = device->m_pd3dDevice->CreateInputLayout(dxLayout.data(),
                                               static_cast<uint32>(dxLayout.size()),
                                               dxVShader->m_pSBlob->GetBufferPointer(),
                                               dxVShader->m_pSBlob->GetBufferSize(),
                                               &pInputL->m_pVertexLayout);
  // failed to create the input layout
  if (hr != 0x00000000) {
    const String errMsg = log.getMessageError(hr);
    const String msg = "Failed to create the input layout. Error message: " + errMsg;
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kError);
    return nullptr;
  }

  return pInputL;
}

void
DX11GraphicsAPI::vSSetConstantBuffers(const Vector<SPtr<ConstantBuffer>>& _pCBuffers,
                                      const uint32 _startSlot)
{
  Logger& log = g_Logger().instance();
  // convert the device to a directx device.
  auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
  if (!device) {
    const String msg = "Failed to utilize the DX device in the setting of a vertex CBuffer.";
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kError);
    return;
  }

  // local array of directx buffers
  const uint32 count = static_cast<uint32>(_pCBuffers.size());
  Vector<ID3D11Buffer*> buffers(count);

  // set all buffers
  for (uint32 i = 0; i < _pCBuffers.size(); ++i) {
    // Recast to a DirectX Constant buffer
    auto dxCB = reinterpret_pointer_cast<DX11ConstantBuffer>(_pCBuffers[i]);
    // set the buffer
    buffers[i] = dxCB ? dxCB->pCBuffer : nullptr;
  }
  // set the buffers
  device->m_pImmediateContext->VSSetConstantBuffers(_startSlot, count, buffers.data());
}

void
DX11GraphicsAPI::vSUnbindConstantBuffers()
{
  Logger& log = g_Logger().instance();
  Vector<ID3D11Buffer*> buffers = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
                                    nullptr, nullptr };
  const uint32 count = static_cast<uint32>(buffers.size());
  // convert the device to a directx device.
  auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
  if (!device) {
    const String msg = "Failed to utilize the DX device in the unbinding of a vertex CBuffer.";
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kError);
    return;
  }
  // set the buffers
  device->m_pImmediateContext->VSSetConstantBuffers(0, count, buffers.data());
}

void
DX11GraphicsAPI::pSSetConstantBuffers(const Vector<SPtr<ConstantBuffer>>& _pCBuffers,
                                      const uint32 _startSlot)
{
  Logger& log = g_Logger().instance();
  // convert the device to a directx device.
  auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
  if (!device) {
    const String msg = "Failed to utilize the DX device in the setting of a pixel CBuffer.";
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kError);
    return;
  }

  // local array of directx buffers
  uint32 count = static_cast<uint32>(_pCBuffers.size());
  Vector<ID3D11Buffer*> buffers(count);

  // set all the buffers in the array
  for (uint32 i = 0; i < _pCBuffers.size(); ++i) {
    // Recast to a DirectX Constant buffer.
    auto dxCB = reinterpret_pointer_cast<DX11ConstantBuffer>(_pCBuffers[i]);
    // set the buffer
    buffers[i] = dxCB ? dxCB->pCBuffer : nullptr;
  }
  // set the buffers
  device->m_pImmediateContext->PSSetConstantBuffers(_startSlot, count, buffers.data());
}

void
DX11GraphicsAPI::pSUnbindConstantBuffers()
{
  Logger& log = g_Logger().instance();
  Vector<ID3D11Buffer*> buffers = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
                                    nullptr, nullptr };
  const uint32 count = static_cast<uint32>(buffers.size());
  // convert the device to a directx device.
  auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
  if (!device) {
    const String msg = "Failed to utilize the DX device in the unbinding of a pixel CBuffer.";
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kError);
    return;
  }
  // set the buffers
  device->m_pImmediateContext->PSSetConstantBuffers(0, count, buffers.data());
}

void
DX11GraphicsAPI::cSSetConstantBuffers(const Vector<SPtr<ConstantBuffer>>& _pCBuffers,
                                      const uint32 _startSlot)
{
  Logger& log = g_Logger().instance();
  // convert the device to a directx device.
  auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
  if (!device) {
    const String msg = "Failed to utilize the DX device in the setting of a compute CBuffer.";
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kError);
    return;
  }

  // local array of directx buffers
  uint32 count = static_cast<uint32>(_pCBuffers.size());
  Vector<ID3D11Buffer*> buffers(count);

  for (uint32 i = 0; i < _pCBuffers.size(); ++i)
  {
    // Recast to a DirectX Constant buffer.
    auto dxCB = reinterpret_pointer_cast<DX11ConstantBuffer>(_pCBuffers[i]);
    // set the buffer
    buffers[i] = dxCB ? dxCB->pCBuffer : nullptr;
  }
  // set the buffers
  device->m_pImmediateContext->CSSetConstantBuffers(_startSlot, count, buffers.data());
}

void
DX11GraphicsAPI::cSUnbindConstantBuffers()
{
  Logger& log = g_Logger().instance();
  Vector<ID3D11Buffer*> buffers = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
                                    nullptr, nullptr };
  const uint32 count = static_cast<uint32>(buffers.size());
  // convert the device to a directx device.
  auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
  if (!device) {
    const String msg = "Failed to utilize the DX device in the unbinding of a compute CBuffer.";
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kError);
    return;
  }
  // set the buffers
  device->m_pImmediateContext->CSSetConstantBuffers(0, count, buffers.data());
}

void
DX11GraphicsAPI::present(uint32 _syncInterval, uint32 _flags)
{
  Logger& log = g_Logger().instance();
  // reinterpret the swap chain to a DirectX swap chain
  auto dxSwapChain = reinterpret_pointer_cast<DX11SwapChain>(m_pSwapChain);
  if (!dxSwapChain) {
    const String msg = "Failed to present a result on the screen.";
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kError);
    return;
  }
  // use the swap chain to present the result
  dxSwapChain->m_pSch->Present(_syncInterval, _flags);
}

Vector2
DX11GraphicsAPI::getViewportSize(uint32 _vpPos)
{
  Logger& log = g_Logger().instance();
  auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
  if (!device) {
    const String msg = "Failed to get the device to get the viewport size.";
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kError);
    return Vector2(0.0f);
  }
  D3D11_VIEWPORT viewport;
  device->m_pImmediateContext->RSGetViewports(&_vpPos, &viewport);
  
  return Vector2(viewport.Width, viewport.Height);
}

void
DX11GraphicsAPI::setSampler(const SPtr<SamplerState> _pSamLinear,
                            uint32 _startSlot,
                            uint32 _numSamplers)
{
  Logger& log = g_Logger().instance();
  // reinterpret to DirectX sampler state
  auto dxSS = reinterpret_pointer_cast<DX11SamplerState>(_pSamLinear);
  if (_pSamLinear && !dxSS) {
    const String msg = "Failed to set a sampler.";
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kError);
    return;
  }
  // set to the device
  auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
  if (!device) {
    const String msg = "Failed to utilize the DX device in the setting of a sampler.";
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kError);
    return;
  }
  device->m_pImmediateContext->PSSetSamplers(_startSlot,
                                             _pSamLinear ? _numSamplers : 0,
                                             _pSamLinear ? &dxSS->m_pSampler : nullptr);
}

void
DX11GraphicsAPI::pSSetShaderResourceViews(const Vector<SPtr<Texture>> _pTextures,
                                          uint32 _start)
{
  Logger& log = g_Logger().instance();
  // cast to a directX device
  auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
  if (!device) {
    const String msg = "Failed to utilize the DX device in the setting "\
                        "of a pixel shader resource view.";
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kError);
    return;
  }

  uint32 count = static_cast<uint32>(_pTextures.size());
  Vector<ID3D11ShaderResourceView*> vResourceVector(count);

  for (uint32 i = 0; i < _pTextures.size(); ++i) {
    // Recast to a DirectX Texture
    auto dxSRV = reinterpret_pointer_cast<DX11Texture>(_pTextures[i]);
    // set the resource
    vResourceVector[i] = dxSRV ? dxSRV->m_sRV : nullptr;
  }
  device->m_pImmediateContext->PSSetShaderResources(_start, count, vResourceVector.data());
}

void
DX11GraphicsAPI::pSUnbindShaderResourceViews()
{
  Logger& log = g_Logger().instance();
  static Vector<ID3D11ShaderResourceView*> _unbindSRV = { nullptr, nullptr, nullptr, nullptr,
                                                          nullptr, nullptr, nullptr, nullptr };
  // cast to a directX device
  auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
  if (!device) {
    const String msg = "Failed to utilize the DX device in the unbinding" \
                       " of a pixel shader resource view.";
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kError);
    return;
  }

  uint32 size = static_cast<uint32>(_unbindSRV.size());
  device->m_pImmediateContext->PSSetShaderResources(0,
                                                    size,
                                                    _unbindSRV.data());
}

void
DX11GraphicsAPI::vSSetShaderResourceViews(const Vector<SPtr<Texture>> _pTextures,
                                          uint32 _start)
{
  Logger& log = g_Logger().instance();
  // cast to a directX device
  auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
  if (!device) {
    const String msg = "Failed to utilize the DX device in the setting " \
                       "of a vertex shader resource view.";
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kError);
    return;
  }

  uint32 count = static_cast<uint32>(_pTextures.size());
  Vector<ID3D11ShaderResourceView*> vResourceVector(count);

  for (uint32 i = 0; i < _pTextures.size(); ++i) {
    // Recast to a DirectX Texture
    auto dxSRV = reinterpret_pointer_cast<DX11Texture>(_pTextures[i]);
    // set the resource
    vResourceVector[i] = dxSRV ? dxSRV->m_sRV : nullptr;
  }
  device->m_pImmediateContext->VSSetShaderResources(_start, count, vResourceVector.data());
}

void
DX11GraphicsAPI::vSUnbindShaderResourceViews()
{
  Logger& log = g_Logger().instance();
  static Vector<ID3D11ShaderResourceView*> _unbindSRV = { nullptr, nullptr, nullptr, nullptr,
                                                          nullptr, nullptr, nullptr, nullptr };
  // cast to a directX device
  auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
  if (!device) {
    const String msg = "Failed to utilize the DX device in the unbinding " \
                       "of a vertex shader resource view.";
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kError);
    return;
  }

  const uint32 size = static_cast<uint32>(_unbindSRV.size());
  device->m_pImmediateContext->VSSetShaderResources(0,
                                                    size,
                                                    _unbindSRV.data());
}

void
DX11GraphicsAPI::cSSetShaderResourceViews(const Vector<SPtr<Texture>> _pTextures,
                                          uint32 _start)
{
  Logger& log = g_Logger().instance();
  // cast to a directX device
  auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
  if (!device) {
    const String msg = "Failed to utilize the DX device in the setting " \
                       "of a compute shader resource view.";
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kError);
    return;
  }

  const uint32 count = static_cast<uint32>(_pTextures.size());
  Vector<ID3D11ShaderResourceView*> uavVector(count);

  for (uint32 i = 0; i < _pTextures.size(); ++i) {
    // Recast to a DirectX Texture
    auto dxUAV = reinterpret_pointer_cast<DX11Texture>(_pTextures[i]);
    // set the resource
    uavVector[i] = dxUAV ? dxUAV->m_sRV : nullptr;
  }
  device->m_pImmediateContext->CSSetShaderResources(_start, count, uavVector.data());
}

void
DX11GraphicsAPI::cSUnbindShaderResourceViews()
{
  Logger& log = g_Logger().instance();
  static Vector<ID3D11ShaderResourceView*> _unbindSRV = { nullptr, nullptr, nullptr, nullptr,
                                                          nullptr, nullptr, nullptr, nullptr };
  // cast to a directX device
  auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
  if (!device) {
    const String msg = "Failed to utilize the DX device in the unbinding " \
                       "of a compute shader resource view.";
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kError);
    return;
  }

  const uint32 size = static_cast<uint32>(_unbindSRV.size());
  device->m_pImmediateContext->CSSetShaderResources(0,
                                                    size,
                                                    _unbindSRV.data());
}

void
DX11GraphicsAPI::cSSetUnorderedAccessViews(const Vector<SPtr<Texture>> _pTextures,
                                           uint32 _start,
                                           uint32* _initialCounts)
{
  Logger& log = g_Logger().instance();
  // cast to a directX device
  auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
  if (!device) {
    const String msg = "Failed to utilize the DX device in the setting " \
                       "of a compute shader resource view.";
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kError);
    return;
  }

  const uint32 count = static_cast<uint32>(_pTextures.size());
  Vector<ID3D11UnorderedAccessView*> uavVector(count);

  for (uint32 i = 0; i < _pTextures.size(); ++i) {
    // Recast to a DirectX Texture
    auto dxUAV = reinterpret_pointer_cast<DX11Texture>(_pTextures[i]);
    // set the resource
    uavVector[i] = dxUAV ? dxUAV->m_uAV : nullptr;
  }

  device->m_pImmediateContext->CSSetUnorderedAccessViews(_start,
                                                         count,
                                                         uavVector.data(),
                                                         _initialCounts);
}

void
DX11GraphicsAPI::cSUnbindUnorderedAccessViews()
{
  Logger& log = g_Logger().instance();
  static Vector<ID3D11UnorderedAccessView*> _unbindUAV = {nullptr, nullptr, nullptr, nullptr,
                                                          nullptr, nullptr, nullptr, nullptr };
  // cast to a directX device
  auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
  if (!device) {
    const String msg = "Failed to utilize the DX device in the unbinding " \
                       "of a compute shader unordered access view.";
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kError);
    return;
  }

  const uint32 numViews = static_cast<uint32>(_unbindUAV.size());
  device->m_pImmediateContext->CSSetUnorderedAccessViews(0,
                                                         numViews,
                                                         _unbindUAV.data(),
                                                         nullptr);
}

SPtr<Texture>
DX11GraphicsAPI::createTextureFromFile(const Path& _fileName,
                                       uint32 _bindFlags,
                                       int32 _mipLevels,
                                       uint32 _format,
                                       int32 _miscFlags)
{
  Logger& log = g_Logger().instance();
  // values
  int32 width, height, bpp;

  // load the image data into a storage variable
  unsigned char* data = stbi_load(_fileName.toString().c_str(), &width, &height, &bpp, 4);

  // check if the texture was found
  if (!data) {
    delete data;
    data = nullptr;
    const String msg = "Can't open " + _fileName.getDirectory() + ", unable to open file.";
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kError);
    return nullptr;
  }

  // how wide each line of the texture will be
  // if (bpp == 3) { ++bpp; }

  // create a default texture using the received parameters
  SPtr<Texture> temptTexture = createTexture(bpp,
                                             width,
                                             height,
                                             _format,
                                             PK_USAGE::kPK_USAGE_DEFAULT,
                                             _bindFlags,
                                             _format,
                                             _mipLevels,
                                             _miscFlags,
                                             data);

  // if creating the texture failed
  if (!temptTexture) {
    const String msg = "Failed to create a texture.";
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kError);
    return nullptr;
  }

  // free the texture data if there's data to release
  if (data) { stbi_image_free(data); }

  // set the path
  temptTexture->setName(_fileName);

  // return the texture
  return temptTexture;
}

SPtr<Texture>
DX11GraphicsAPI::createDDSTextureFromFile(const Path& _directory)
{
  Logger& log = g_Logger().instance();
  SPtr<DX11Texture> texture = make_shared<DX11Texture>();
  // set to the device
  auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
  if (!device) {
    const String msg = "Failed to utilize the DX device in the creation of a DSS texture.";
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kError);
    return nullptr;
  }
  uint32 hr = DirectX::CreateDDSTextureFromFile(device->m_pd3dDevice,
                                                _directory.getDirectoryWStr().c_str(),
                                                nullptr,
                                                &texture->m_sRV);
  if (FAILED(hr)) {
    const String errMsg = g_Logger().getMessageError(hr);
    const String msg = "Failed to create a DSS texture. Error message: " + errMsg;
    log.registerMessage(msg, LOG_MSG_TYPE::kError);
    return nullptr;
  }
  return texture;
}

SPtr<Texture>
DX11GraphicsAPI::createTextureFromFileF(const Path& _fileName,
                                        uint32 _bindFlags,
                                        int32 _mipLevels,
                                        int32 _miscFlags,
                                        PK_USAGE::E _usage)
{
  Logger& log = g_Logger().instance();
  // values
  int32 width, height, channels, bpp;

  // load the image data into a storage variable
  float* data = stbi_loadf(_fileName.toString().c_str(), &width, &height, &channels, 4);

  // check if the texture was found
  if (!data) {
    delete data;
    data = nullptr;
    const String msg = "Can't open " + _fileName.getFileName() + ", unable to open file."
                        + " Reason: " + stbi_failure_reason();
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kError);
    return nullptr;
  }

  // how wide each line of the texture will be
  bpp = 4 * sizeof(float);
  uint32 format = PK_TEXTURE_FORMAT::kPK_FORMAT_R32G32B32A32_FLOAT;
  // if (channels == 4) { format = TEXTURE_FORMAT::kPK_FORMAT_R32G32B32A32_FLOAT; }

  // create a default texture using the received parameters
  SPtr<Texture> temptTexture = createTexture(bpp,
                                             width,
                                             height,
                                             format,
                                             _usage,
                                             _bindFlags,
                                             format,
                                             _mipLevels,
                                             _miscFlags,
                                             reinterpret_cast<unsigned char*>(data));

  // if creating the texture failed
  if (!temptTexture) {
    const String msg = "Failed to create a texture.";
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kError);
    return nullptr;
  }

  // free the texture data if there's data to release
  if (data) { stbi_image_free(data); }

  // set the path
  temptTexture->setName(_fileName);

  // return the texture
  return temptTexture;
}

uint32
getBitsFromFormat(PK_TEXTURE_FORMAT::E _format)
{
  // RGBA of value 32
  if (_format == PK_TEXTURE_FORMAT::kPK_FORMAT_R32G32B32A32_FLOAT ||
      _format == PK_TEXTURE_FORMAT::kPK_FORMAT_R32G32B32A32_TYPELESS ||
      _format == PK_TEXTURE_FORMAT::kPK_FORMAT_R32G32B32A32_UINT ||
      _format == PK_TEXTURE_FORMAT::kPK_FORMAT_R32G32B32A32_SINT) {
    return 32 * 4;
  }
  // RGB of value 32
  if (_format == PK_TEXTURE_FORMAT::kPK_FORMAT_R32G32B32_FLOAT ||
      _format == PK_TEXTURE_FORMAT::kPK_FORMAT_R32G32B32_TYPELESS ||
      _format == PK_TEXTURE_FORMAT::kPK_FORMAT_R32G32B32_UINT ||
      _format == PK_TEXTURE_FORMAT::kPK_FORMAT_R32G32B32_SINT) {
    return 32 * 3;
  }
  // RGBA of value 16
  if (_format == PK_TEXTURE_FORMAT::kPK_FORMAT_R16G16B16A16_FLOAT ||
      _format == PK_TEXTURE_FORMAT::kPK_FORMAT_R16G16B16A16_TYPELESS ||
      _format == PK_TEXTURE_FORMAT::kPK_FORMAT_R16G16B16A16_UINT ||
      _format == PK_TEXTURE_FORMAT::kPK_FORMAT_R16G16B16A16_SINT) {
    return 16 * 4;
  }
  // RG of value 16
  if (_format == PK_TEXTURE_FORMAT::kPK_FORMAT_R16G16_FLOAT ||
      _format == PK_TEXTURE_FORMAT::kPK_FORMAT_R16G16_TYPELESS ||
      _format == PK_TEXTURE_FORMAT::kPK_FORMAT_R16G16_UINT ||
      _format == PK_TEXTURE_FORMAT::kPK_FORMAT_R16G16_SINT) {
    return 16 * 2;
  }
  // RGBA of value 8
  if (_format == PK_TEXTURE_FORMAT::kPK_FORMAT_R8G8B8A8_UNORM ||
      _format == PK_TEXTURE_FORMAT::kPK_FORMAT_R8G8B8A8_TYPELESS ||
      _format == PK_TEXTURE_FORMAT::kPK_FORMAT_R8G8B8A8_UINT ||
      _format == PK_TEXTURE_FORMAT::kPK_FORMAT_R8G8B8A8_SINT || 
      _format == PK_TEXTURE_FORMAT::kPK_FORMAT_B8G8R8A8_UNORM ||
      _format == PK_TEXTURE_FORMAT::kPK_FORMAT_B8G8R8A8_TYPELESS) {
    return 8 * 4;
  }
  // R of value 8
  if (_format == PK_TEXTURE_FORMAT::kPK_FORMAT_R8_TYPELESS ||
      _format == PK_TEXTURE_FORMAT::kPK_FORMAT_R8_UNORM || 
      _format == PK_TEXTURE_FORMAT::kPK_FORMAT_R8_SINT || 
      _format == PK_TEXTURE_FORMAT::kPK_FORMAT_R8_UINT) {
    return 8;
  }
}

SPtr<Texture>
DX11GraphicsAPI::createTexture(uint32 _bpp,
                               uint32 _width,
                               uint32 _height,
                               int32 _format,
                               int32 _usage,
                               int32 _bindFlags,
                               int32 _shaderResourceFormat,
                               int32 _mipLevels,
                               int32 _miscFlags,
                               unsigned char* _data)
{
  PK_ASSERT(m_pDevice);
  Logger& log = g_Logger().instance();
  // verify that the device is a directX 11 device
  auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
  if (!device) {
    const String msg = "Failed to utilize the DX device in the creation of a texture.";
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kError);
    return nullptr;
  }

  // if the texture is supposed to generate mipMaps
  // uint32 mips = 0;
  // if ((_miscFlags & PK_RESOURCE_MISC_FLAG::kPK_RESOURCE_MISC_GENERATE_MIPS) == 
  //     PK_RESOURCE_MISC_FLAG::kPK_RESOURCE_MISC_GENERATE_MIPS) {
  //   mips = 1;
  // }

  // create the texture
  SPtr<DX11Texture> tex = make_shared<DX11Texture>();
  tex->setSize(Vector2(_width, _height));

  // texture description
  D3D11_TEXTURE2D_DESC desc;
  memset(&desc, 0, sizeof(desc));
  desc.Width = _width;
  desc.Height = _height;
  desc.MipLevels = 1;
  desc.ArraySize = 1;
  desc.Format = static_cast<DXGI_FORMAT>(_format);
  desc.SampleDesc.Count = 1;
  desc.SampleDesc.Quality = 0;
  desc.Usage = static_cast<D3D11_USAGE>(_usage);
  desc.BindFlags = _bindFlags;
  desc.CPUAccessFlags = 0;
  desc.MiscFlags = static_cast<D3D11_RESOURCE_MISC_FLAG>(_miscFlags);

  // data of the texture
  D3D11_SUBRESOURCE_DATA* initData = nullptr;
  if (_data) {
    initData = new D3D11_SUBRESOURCE_DATA();
    initData->pSysMem = _data;
    uint32 bytesPerPixel = getBitsFromFormat(static_cast<PK_TEXTURE_FORMAT::E>(_format)) / 8;
    initData->SysMemPitch = _width * bytesPerPixel;
    initData->SysMemSlicePitch = 0;
  }

  // create the texture
  int32 hr = 0;
  hr = device->m_pd3dDevice->CreateTexture2D(&desc, initData, &tex->m_t2d);

  // if texture creation failed
  if (FAILED(hr)) {
    const String errMsg = log.getMessageError(hr);
    const String msg = "Failed to create a texture. Error message: " + errMsg;
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kError);
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
    sDesc.Texture2D.MipLevels = -1;
    sDesc.Texture2D.MostDetailedMip = 0;
    sDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

    // create the shader resource view
    hr = device->m_pd3dDevice->CreateShaderResourceView(tex->m_t2d, &sDesc, &tex->m_sRV);
    // if failed to create shader resource view
    if (FAILED(hr)) {
      const String errMsg = log.getMessageError(hr);
      const String msg = "Failed to create a shader resource view. Error message: " + errMsg;
      log.print(msg);
      log.registerMessage(msg, LOG_MSG_TYPE::kError);
      return nullptr;
    }
    // device->m_pImmediateContext->GenerateMips(tex->m_sRV); // this is hell on earth
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

    hr = device->m_pd3dDevice->CreateDepthStencilView(tex->m_t2d, &dsvDesc, &tex->m_dSV);
    if (!tex->m_dSV) {
      const String errMsg = log.getMessageError(hr);
      const String msg = "Failed to create the depth stencil. Error message: " + errMsg;
      log.print(msg);
      log.registerMessage(msg, LOG_MSG_TYPE::kError);
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
    rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    rtvDesc.Texture2D.MipSlice = 0;

    // create the render target
    hr = device->m_pd3dDevice->CreateRenderTargetView(tex->m_t2d, &rtvDesc, &tex->m_rTV);
    if (FAILED(hr)) {
      const String errMsg = log.getMessageError(hr);
      const String msg = "Failed to create a render target view. Error message: " + errMsg;
      log.print(msg);
      log.registerMessage(msg, LOG_MSG_TYPE::kError);
    }
  }
  /**
   * Create unordered access
   */
  if ((_bindFlags & D3D11_BIND_UNORDERED_ACCESS) == D3D11_BIND_UNORDERED_ACCESS) {
    D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
    memset(&uavDesc, 0, sizeof(uavDesc));
    uavDesc.Format = desc.Format;
    uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
    uavDesc.Texture2D.MipSlice = 0;

    hr = device->m_pd3dDevice->CreateUnorderedAccessView(tex->m_t2d, &uavDesc, &tex->m_uAV);
    if (FAILED(hr)) {
      const String errMsg = log.getMessageError(hr);
      const String msg = "Failed to create an unordered access view. Error message: " + errMsg;
      log.print(msg);
      log.registerMessage(msg, LOG_MSG_TYPE::kError);
    }
  }
  return tex;
}

void
DX11GraphicsAPI::setInputLayout( const SPtr<InputLayout> _pInputLayout)
{
  Logger& log = g_Logger().instance();
  // reinterpret to a DirectX input layout
  const SPtr<DX11InputLayout> dxIL = reinterpret_pointer_cast<DX11InputLayout>(_pInputLayout);
  // if the layout is not null but failed to reinterpret
  if (_pInputLayout && !dxIL) {
    const String msg = "Failed to set the input layout.";
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kError);
    return;
  }
  // get the dx11 device
  auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
  if (!device) {
    const String msg = "Failed to get the DirectX device when setting an input layout.";
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kError);
    return;
  }
  // get the dx11 input layout
  const auto inputLayout = reinterpret_pointer_cast<DX11InputLayout>(_pInputLayout);
  // set the input layout
  device->m_pImmediateContext->IASetInputLayout(inputLayout ? inputLayout->m_pVertexLayout : 
                                                              nullptr);
}

SPtr<VertexBuffer>
DX11GraphicsAPI::createVertexBuffer(const Vector<SimpleVertex>& _vertex,
                                    uint32 _usage)
{
  Logger& log = g_Logger().instance();
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

  auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
  if (!device) {
    const String msg = "Failed to utilize the DX device in the creation of a vertex buffer.";
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kError);
    return nullptr;
  }
  const int32 hr = device->m_pd3dDevice->CreateBuffer(&bd, &InitData, &dxVB->pBuffer);
  if (FAILED(hr)) {
    const String errMsg = log.getMessageError(hr);
    const String msg = "Failed to create a vertex buffer. Error message: " + errMsg;
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kError);
    return nullptr;
  }
  return dxVB;
}

void
DX11GraphicsAPI::setVertexBuffer(const SPtr<VertexBuffer>& _pVertexB,
                                 uint32 _start,
                                 uint32 _bufferCount,
                                 uint32 _offset)
{
  Logger& log = g_Logger().instance();
  // reinterpret pointer
  const auto dxVB = reinterpret_pointer_cast<DX11VertexBuffer>(_pVertexB);
  // if failed to cast to DX11VertexBuffer
  if (!dxVB) {
    const String msg = "Failed to set the vertex buffer.";
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kError);
    return;
  }
  
  // get the offset
  const uint32 stride = sizeof(SimpleVertex);
  //set the buffer
  const auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
  if (!device) {
    const String msg = "Failed to utilize the DX device in the setting of a vertex buffer.";
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kError);
    return;
  }
  device->m_pImmediateContext->IASetVertexBuffers(_start,
                                                  _bufferCount,
                                                  &dxVB->pBuffer,
                                                  &stride,
                                                  &_offset);
}

SPtr<IndexBuffer>
DX11GraphicsAPI::createIndexBuffer(const Vector<uint32>& _index,
                                   uint32 _usage)
{
  Logger& log = g_Logger().instance();
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
  auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
  if (!device) {
    const String msg = "Failed to utilize the DX device in the creation of an index buffer.";
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kError);
    return nullptr;
  }
  int32 hr = device->m_pd3dDevice->CreateBuffer(&bd, &InitData, &dxIB->pBuffer);
  if (FAILED(hr)) {
    const String errMsg = log.getMessageError(hr);
    const String msg = "failed to create an index buffer. Error message: " + errMsg;
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kError);
    return nullptr;
  }
  return dxIB;
}

void
DX11GraphicsAPI::setIndexBuffer(const SPtr<IndexBuffer>& _pIndexB,
                                uint32 _format,
                                uint32 _offset)
{
  Logger& log = g_Logger().instance();
  // reinterpret pointer
  auto dxIB = reinterpret_pointer_cast<DX11IndexBuffer>(_pIndexB);
  // if failed to cast to DX11IndexBuffer
  if (!dxIB) {
    const String msg = "Failed to set the index buffer.";
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kError);
    return;
  }
  auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
  if (!device) {
    const String msg = "Failed to utilize the DX device in the setting of an index buffer.";
    log.print(msg);
    log.registerMessage(msg, LOG_MSG_TYPE::kError);
    return;
  }
  device->m_pImmediateContext->IASetIndexBuffer(dxIB->pBuffer,
                                                static_cast<DXGI_FORMAT>(_format),
                                                _offset);
}
}
#endif