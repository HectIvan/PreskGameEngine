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
#define STB_IMAGE_IMPLEMENTATION

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include <stb_image.h>

#include "pkLogger.h"
#include "pkDX11BlendState.h"
#include "pkDX11GraphicsAPI.h"
#include "pkDX11IndexBuffer.h"
#include "pkDX11PixelShader.h"
#include "pkDX11Prerequisites.h"
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

  setViewport(width, height);
  // uint32 vpX = static_cast<uint32>(getViewportSize(1).x);
  // uint32 vpY = static_cast<uint32>(getViewportSize(1).y);

  // create the render targets
  auto dxSCh = reinterpret_pointer_cast<DX11SwapChain>(m_pSwapChain);
  dxSCh->createRenderTargetView(m_pDevice);
  
  auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
  if (!device) {
    g_Logger().print("Failed to utilize the DX device in the API Initialization");
  }
  device->setPrimitiveTopology();
}

SPtr<ConstantBuffer>
DX11GraphicsAPI::createConstantBuffer(uint32 _size, const void* _pData, uint32 _usage)
{
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
    g_Logger().print("Failed to utilize the DX device in the creation of a CBuffer.");
  }
  hr = device->pd3dDevice->CreateBuffer(&bDesc, _pData ? &subData : nullptr, &dxCB->pCBuffer);
  if (FAILED(hr)) {
    String errMsg = g_Logger().getMessageError(hr);
    g_Logger().print("Failed to create the constant buffer. Error: " + errMsg);
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
    g_Logger().print("Failed to update constant buffers.");
    return;
  }
  // update with the new data
  auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
  if (!device) {
    g_Logger().print("Failed to utilize the DX device in the update of a CBuffer");
  }
  device->pImmediateContext->UpdateSubresource(dxCB->pCBuffer, 0, nullptr, _pNewData, _size, 0);
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
  }
  device->pImmediateContext->DrawIndexed(_indexCount,
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
  }
  device->pImmediateContext->Draw(_indexCount,
                                  _startIndexLocation);
}

void
DX11GraphicsAPI::dispatch(uint32 _countX, uint32 _countY, uint32 _countZ)
{
  // draw the data
  auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
  if (!device) {
    g_Logger().print("Failed to utilize the DX device in the Dispatch call");
  }
  device->pImmediateContext->Dispatch(_countX, _countY, _countZ);
}

void
DX11GraphicsAPI::clearRenderTargetView(float _color[], SPtr<Texture> _rtv)
{
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
  }
  device->pImmediateContext->ClearRenderTargetView(dxRTV->m_rTV, _color);
}

void
DX11GraphicsAPI::clearDepthBuffer(float _depth, SPtr<DepthStencilView> _depthSV)
{
  // Cast to a DirectX depth stencil.
  auto dxDSV = reinterpret_pointer_cast<DX11DepthStencilView>(_depthSV);
  // If the casting failed.
  if (!dxDSV) {
    g_Logger().print("Failed to clear depth stencil view.");
    return;
  }
  // Clear the depth buffer to 1.0 (max depth)
  auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
  if (!device) {
    g_Logger().print("Failed to utilize the DX device in the clearing of the depth buffer.");
  }
  device->pImmediateContext->ClearDepthStencilView(dxDSV->pDepthSV,
                                                      D3D11_CLEAR_DEPTH,
                                                      _depth,
                                                      0);
}

void
DX11GraphicsAPI::createPShader(SPtr<Shader> _pShader)
{
  // convert from shader to dx pixel shader
  SPtr<DX11PixelShader> dxPShader = reinterpret_pointer_cast<DX11PixelShader>(_pShader);
  uint32 hr;
  // create the pixel shader
  auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
  if (!device) {
    g_Logger().print("Failed to utilize the DX device in the creation of a pixel shader.");
  }
  hr = device->pd3dDevice->CreatePixelShader(dxPShader->pSBlob->GetBufferPointer(),
                                             dxPShader->pSBlob->GetBufferSize(),
                                             nullptr, &dxPShader->pShader);
  // check if the creation was successful
  if (FAILED(hr)) {
    String errMsg = g_Logger().getMessageError(hr);
    g_Logger().print("Failed to create pixel shader. Error: " + errMsg);
    dxPShader->pSBlob->Release();
    return;
  }
  return;
}

void
DX11GraphicsAPI::createVShader(SPtr<Shader> _pShader)
{
  // convert from shader to dx vertex shader
  SPtr<DX11VertexShader> dxVShader = reinterpret_pointer_cast<DX11VertexShader>(_pShader);
  // SPtr<DX11VertexShader> dxVShader = make_shared<DX11VertexShader>();
  // create the vertex shader
  uint32 hr;
  auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
  if (!device) {
    g_Logger().print("Failed to utilize the DX device in the");
  }
  hr = device->pd3dDevice->CreateVertexShader(dxVShader->pSBlob->GetBufferPointer(),
                                              dxVShader->pSBlob->GetBufferSize(),
                                              nullptr,
                                              &dxVShader->pShader);
  // check if the creation was successful
  if (hr != 0x00000000) {
    String errMsg = g_Logger().getMessageError(hr);
    g_Logger().print("Failed to create vertex shader. Error: " + errMsg);
    dxVShader->pSBlob->Release();
    return;
  }
  // _pShader = dxVShader;
  return;
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
  SPtr<DX11SwapChain> pSwapChain = make_shared<DX11SwapChain>();

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
  auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
  if (!device) {
    g_Logger().print("Failed to utilize the DX device in the device creation.");
  }
  for (uint32 driverTypeIndex = 0; driverTypeIndex < _numDriverTypes; driverTypeIndex++) {
    // try and create the device and swap chain with the current driver type
    device->pDriverType = new D3D_DRIVER_TYPE(_driverTypes[driverTypeIndex]);
    int32 hr = D3D11CreateDeviceAndSwapChain(nullptr,
                                             *device->pDriverType,
                                             nullptr,
                                             _createDeviceFlags,
                                             _featureLevels,
                                             _numFeatureLevels,
                                             D3D11_SDK_VERSION,
                                             &sd,
                                             &pSwapChain->m_pSch,
                                             &device->pd3dDevice,
                                             &device->featureLevel,
                                             &device->pImmediateContext);

    // if creation was successful
    if (hr == 0x00000000) {
      // end the entire process, no need to continue
      m_pSwapChain = pSwapChain;
      break;
    }
  }
}

void
DX11GraphicsAPI::setRenderTargets(Vector<SPtr<Texture>> _rTargets,
                                  SPtr<DepthStencilView> _DepthSV)
{
  // reinterpret the depth stencil view to a DirectX texture
  auto pDSV = reinterpret_pointer_cast<DX11DepthStencilView>(_DepthSV);
  // get a texture vector for DX11 textures
  Vector<SPtr<DX11Texture>> txVector;
  // render target vector
  Vector<ID3D11RenderTargetView*> rTVector;
  // get the vector size
  uint32 RTCount = static_cast<uint32>(_rTargets.size());
  // reinterpret each of the targets as a DX11 texture and store in the texture vector
  for (uint32 i = 0; i < RTCount; ++i) {
    SPtr<DX11Texture> dxTx = reinterpret_pointer_cast<DX11Texture>(_rTargets[i]);
    // store the render target and the texture
    if (dxTx->m_rTV) { rTVector.push_back(dxTx->m_rTV); }
    txVector.push_back(dxTx);
  }
  // set the render targets
  auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
  if (!device) {
    g_Logger().print("Failed to utilize the DX device in the setting of the render targets.");
  }
  device->pImmediateContext->OMSetRenderTargets(static_cast<uint32>(rTVector.size()),
                                                rTVector.data(),
                                                pDSV->pDepthSV);
}

SPtr<SamplerState>
DX11GraphicsAPI::createSamplerState(const uint32 _mode, const uint32 _filter)
{
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
    g_Logger().print("Failed to utilize the DX device in the sampler state creation.");
  }
  int32 hr = device->pd3dDevice->CreateSamplerState(&sampDesc, &pSamState->m_pSampler);
  if (hr != 0x00000000) {
    String errMsg = g_Logger().getMessageError(hr);
    g_Logger().print("Failed to create a sampler state. Error: " + errMsg);
    return nullptr;
  }
  return pSamState;
}

SPtr<DepthStencilView>
DX11GraphicsAPI::createDepthStencilView(SPtr<Texture> _depthRT)
{
  //// reinterpret as a directX texture
  SPtr<DX11Texture> dxDepthTx = reinterpret_pointer_cast<DX11Texture>(_depthRT);

  int32 hr;
  // create depth stencil with the generated 2D texture
  SPtr<DX11DepthStencilView> pDepthSView = make_shared<DX11DepthStencilView>();
  D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
  ZeroMemory(&descDSV, sizeof(descDSV));
  descDSV.Format = DXGI_FORMAT_D32_FLOAT; // DXGI_FORMAT_D24_UNORM_S8_UINT;
  descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
  descDSV.Texture2D.MipSlice = 0;

  auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
  if (!device) {
    g_Logger().print("Failed to utilize the DX device in the depth stencil view creation.");
  }
  hr = device->pd3dDevice->CreateDepthStencilView(dxDepthTx->m_t2d,
                                                  &descDSV,
                                                  &pDepthSView->pDepthSV);
  // if the creation was not succesful
  if (hr != 0x00000000) {
    String errMsg = g_Logger().getMessageError(hr);
    g_Logger().print("Failed to create a depth stencil. Error: " + errMsg);
    return nullptr;
  }
  // return the final value
  return pDepthSView;
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
  auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
  if (!device) {
    g_Logger().print("Failed to utilize the DX device in the setting of the viewport.");
  }
  device->pImmediateContext->RSSetViewports(1, &vp);
  // world = Matrix4::IDENTITY;
}

void
DX11GraphicsAPI::setPSShader(SPtr<Shader> _pShader)
{
  // reinterpret as a DirectX pixel shader
  SPtr<DX11PixelShader> dxPShader = reinterpret_pointer_cast<DX11PixelShader>(_pShader);
  if (!dxPShader) {
    g_Logger().print("Failed to set a pixel shader.");
    return;
  }
  // set the shader
  auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
  if (!device) {
    g_Logger().print("Failed to utilize the DX device in the setting of a pixel shader.");
  }
  device->pImmediateContext->PSSetShader(dxPShader->pShader, nullptr, 0);
}

void
DX11GraphicsAPI::setVSShader(SPtr<Shader> _pShader)
{
  // reinterpret as a DirectX vertex shader
  SPtr<DX11VertexShader> dxVShader = reinterpret_pointer_cast<DX11VertexShader>(_pShader);
  if (!dxVShader) {
    g_Logger().print("Failed to set a vertex shader.");
    return;
  }
  // set the shader
  auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
  if (!device) {
    g_Logger().print("Failed to utilize the DX device in the setting of a vertex shader.");
  }
  device->pImmediateContext->VSSetShader(dxVShader->pShader, nullptr, 0);
}

SPtr<BlendState>
DX11GraphicsAPI::createBlendState()
{
  // create the blend state
  SPtr<DX11BlendState> pBlendState = make_shared<DX11BlendState>();

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
    g_Logger().print("Failed to utilize the DX device in the creation of a blend state.");
  }
  int32 hr = device->pd3dDevice->CreateBlendState(&blendDesc, &pBlendState->m_pBlendState);

  if (FAILED(hr)) {
    String errMsg = g_Logger().getMessageError(hr);
    g_Logger().print("Failed to create Blend state. Error: " + errMsg);
    return nullptr;
  }
  return pBlendState;
}

void
DX11GraphicsAPI::setBlendState(SPtr<BlendState> _pBlendState)
{
  // Reinterpret to a DirectX Blend State
  SPtr<DX11BlendState> dxBS = reinterpret_pointer_cast<DX11BlendState>(_pBlendState);
  if (!dxBS) {
    g_Logger().print("Failed to set Blend State.");
    return;
  }
  // Set the blend state
  auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
  if (!device) {
    g_Logger().print("Failed to utilize the DX device in the setting of a blend state.");
  }
  device->pImmediateContext->OMSetBlendState(dxBS->m_pBlendState, nullptr, 0xFFFFFFFF);
}

void
DX11GraphicsAPI::compileShaderFromFile(WString _szFileName,
                                       const char* _szEntryPoint,
                                       const char* _szShaderModel,
                                       SPtr<Shader> _pTargetShader)
{
  int32 hr = S_OK;

  DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
  // Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
  // Setting this flag improves the shader debugging experience, but still allows 
  // the shaders to be optimized and to run exactly the way they will run in 
  // the release configuration of this program.
  dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif
  ID3DBlob* pErrorBlob;
  auto dxShader = reinterpret_pointer_cast<DX11Shaders>(_pTargetShader);
  hr = D3DCompileFromFile(_szFileName.c_str(),
                          nullptr,
                          nullptr,
                          _szEntryPoint,
                          _szShaderModel,
                          dwShaderFlags,
                          0,
                          &dxShader->pSBlob,
                          &pErrorBlob);
  if (FAILED(hr))
  {
    if (pErrorBlob != nullptr)
    {
      // String errText(reinterpret_cast<char*>(pErrorBlob->GetBufferPointer()));
      String errMsg = g_Logger().getMessageError(hr);
      g_Logger().print("Failed to compile shader: " + errMsg);
    }
    safeRelease(pErrorBlob);
    return;
  }
  
  return;
}

SPtr<InputLayout>
DX11GraphicsAPI::createInputLayoutFromVShader(SPtr<Shader> _pShader)
{
  // create the input layout pointer
  SPtr<DX11InputLayout> pLayout = make_shared<DX11InputLayout>();
  // reinterpret to a DirectX vertex shader
  SPtr<DX11VertexShader> dxVShader = reinterpret_pointer_cast<DX11VertexShader>(_pShader);

  ID3D11ShaderReflection* pVShaderReflection = nullptr;
  throwIfFailed(D3DReflect(dxVShader->pSBlob->GetBufferPointer(),
                           dxVShader->pSBlob->GetBufferSize(),
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
    g_Logger().print("Failed to utilize the DX device in the creation of an input layout.");
  }
  device->pd3dDevice->CreateInputLayout(&inputLayoutDesc[0],
                                        static_cast<uint32>(inputLayoutDesc.size()),
                                        dxVShader->pSBlob->GetBufferPointer(),
                                        dxVShader->pSBlob->GetBufferSize(),
                                        &pLayout->pVertexLayout);
  if (!pLayout) {
    g_Logger().print("Failed to create the input layout.");
    return nullptr;
  }
  return pLayout;
}

SPtr<InputLayout>
DX11GraphicsAPI::createInputLayout(const Vector<InputDesc>& _vDesc,
                                   const SPtr<Shader> _pVShader)
{
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
    g_Logger().print("Failed to utilize the DX device in the creation of an input layout.");
  }
  hr = device->pd3dDevice->CreateInputLayout(dxLayout.data(),
                                             static_cast<uint32>(dxLayout.size()),
                                             dxVShader->pSBlob->GetBufferPointer(),
                                             dxVShader->pSBlob->GetBufferSize(),
                                             &pInputL->pVertexLayout);
  // failed to create the input layout
  if (hr != 0x00000000) {
    String errMsg = g_Logger().getMessageError(hr);
    g_Logger().print("Failed to create the input layout. Error: " + errMsg);
    return nullptr;
  }

  return pInputL;
}

void
DX11GraphicsAPI::VSSetConstantBuffer(SPtr<ConstantBuffer> _pCBuffer,
                                     uint32 _startSlot,
                                     uint32 _numBuffers)
{
  // Recast to a DirectX Constant buffer
  auto dxCB = reinterpret_pointer_cast<DX11ConstantBuffer>(_pCBuffer);
  if (!dxCB) {
    g_Logger().print("Failed to set a constant buffer.");
    return;
  } // casting failed
  auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
  if (!device) {
    g_Logger().print("Failed to utilize the DX device in the setting of a vertex CBuffer.");
  }
  device->pImmediateContext->VSSetConstantBuffers(_startSlot, _numBuffers, &dxCB->pCBuffer);
}

void
DX11GraphicsAPI::PSSetConstantBuffer(SPtr<ConstantBuffer> _pCBuffer,
                                     uint32 _startSlot,
                                     uint32 _numBuffers)
{
  // Recast to a DirectX Constant buffer.
  auto dxCB = reinterpret_pointer_cast<DX11ConstantBuffer>(_pCBuffer);
  // if the casting failed
  if (!dxCB) {
    g_Logger().print("Failed to set a constant buffer.");
    return;
  } // casting failed
  auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
  if (!device) {
    g_Logger().print("Failed to utilize the DX device in the setting of a pixel CBuffer.");
  }
  device->pImmediateContext->PSSetConstantBuffers(_startSlot, _numBuffers, &dxCB->pCBuffer);
}

void
DX11GraphicsAPI::CSSetConstantBuffer(SPtr<ConstantBuffer> _pCBuffer,
                                     uint32 _startSlot,
                                     uint32 _numBuffers)
{
  // Recast to a DirectX Constant buffer.
  auto dxCB = reinterpret_pointer_cast<DX11ConstantBuffer>(_pCBuffer);
  // if the casting failed
  if (!dxCB) {
    g_Logger().print("Failed to set a constant buffer.");
    return;
  } // casting failed
  auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
  if (!device) {
    g_Logger().print("Failed to utilize the DX device in the setting of a compute CBuffer.");
  }
  device->pImmediateContext->CSSetConstantBuffers(_startSlot, _numBuffers, &dxCB->pCBuffer);
}

void
DX11GraphicsAPI::present(uint32 _syncInterval, uint32 _flags)
{
  // reinterpret the swap chain to a DirectX swap chain
  auto dxSwapChain = reinterpret_pointer_cast<DX11SwapChain>(m_pSwapChain);
  if (!dxSwapChain) {
    g_Logger().print("Failed to present a result on the screen.");
    return;
  }
  // use the swap chain to present the result
  dxSwapChain->m_pSch->Present(_syncInterval, _flags);
}

Vector2
DX11GraphicsAPI::getViewportSize(uint32 _vpPos)
{
  auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
  D3D11_VIEWPORT viewport;
  device->pImmediateContext->RSGetViewports(&_vpPos, &viewport);
  
  return Vector2(viewport.Width, viewport.Height);
}

void
DX11GraphicsAPI::setSampler(SPtr<SamplerState> _pSamLinear,
                            uint32 _startSlot,
                            uint32 _numSamplers)
{
  // reinterpret to DirectX sampler state
  auto dxSS = reinterpret_pointer_cast<DX11SamplerState>(_pSamLinear);
  if (!dxSS) {
    g_Logger().print("Failed to set a sampler");
    return;
  }
  // set to the device
  auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
  if (!device) {
    g_Logger().print("Failed to utilize the DX device in the setting of a sampler.");
  }
  device->pImmediateContext->PSSetSamplers(_startSlot,
                                           _numSamplers,
                                           &dxSS->m_pSampler);
}

void
DX11GraphicsAPI::setShaderResourceView(SPtr<Texture> _pTexture,
                                       uint32 _start,
                                       uint32 _numViews)
{
  // cast to a directX texture
  auto dxTX = reinterpret_pointer_cast<DX11Texture>(_pTexture);
  // if failed to cast to the texture
  if (!dxTX) {
    return;
  }
  // set the shader resource view 
  auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
  if (!device) {
    g_Logger().print("Failed to utilize the DX device in the setting of a shader resource view.");
  }
  device->pImmediateContext->PSSetShaderResources(_start, _numViews, &dxTX->m_srv);
}

void
DX11GraphicsAPI::PSSetShaderResourceView(SPtr<Texture> _pTexture,
                                         uint32 _start,
                                         uint32 _numViews)
{
  // cast to a directX texture
  auto dxTX = reinterpret_pointer_cast<DX11Texture>(_pTexture);
  // if failed to cast to the texture
  if (!dxTX) {
    g_Logger().print("Failed to get a DX Texture in setting of a pixel shader resource");
    return;
  }
  // set the shader resource view 
  auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
  if (!device) {
    g_Logger().print("Failed to utilize the DX device in the setting of a pixel shader resource view.");
  }
  device->pImmediateContext->PSSetShaderResources(_start, _numViews, &dxTX->m_srv);
}

void
DX11GraphicsAPI::VSSetShaderResourceView(SPtr<Texture> _pTexture,
                                         uint32 _start,
                                         uint32 _numViews)
{
  // cast to a directX texture
  auto dxTX = reinterpret_pointer_cast<DX11Texture>(_pTexture);
  // if failed to cast to the texture
  if (!dxTX) {
    g_Logger().print("Failed to get a DX Texture in setting of a vertex shader resource");
    return;
  }
  // set the shader resource view 
  auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
  if (!device) {
    g_Logger().print("Failed to utilize the DX device in the setting of a vertex shader resource view.");
  }
  device->pImmediateContext->VSSetShaderResources(_start, _numViews, &dxTX->m_srv);
}

void
DX11GraphicsAPI::CSSetShaderResourceView(SPtr<Texture> _pTexture,
                                         uint32 _start,
                                         uint32 _numViews)
{
  // cast to a directX texture
  auto dxTX = reinterpret_pointer_cast<DX11Texture>(_pTexture);
  // if failed to cast to the texture
  if (!dxTX) {
    g_Logger().print("Failed to get a DX Texture in setting of a compute shader resource");
    return;
  }
  // cast to a directX device
  auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
  if (!device) {
    g_Logger().print("Failed to utilize the DX device in the setting of a compute shader resource view.");
  }
  device->pImmediateContext->CSSetShaderResources(_start, _numViews, &dxTX->m_srv);
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
  if (!data) {
    g_Logger().print("Data to create a texture was not found.");
    return make_shared<DX11Texture>();
  }

  // how wide each line of the texture will be
  if (bpp == 3) { ++bpp; }
  pitch = width * bpp;

  // create a default texture using the received parameters
  SPtr<Texture> temptTexture = createTexture(data,
                                             bpp,
                                             width,
                                             height,
                                             _format,
                                             0,
                                             _bindFlags,
                                             _mipLevels,
                                             _format);

  // if creating the texture failed
  if (!temptTexture) {
    g_Logger().print("Failed to create a texture.");
    return make_shared<DX11Texture>();
  }

  // cast to DX11Texture
  SPtr<DX11Texture> dxTX = reinterpret_pointer_cast<DX11Texture>(temptTexture);

  // if casting the texture failed
  if (!dxTX) {
    g_Logger().print("Failed to create a texture.");
    return make_shared<DX11Texture>();
  }

  // update the texture
  auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
  if (!device) {
    g_Logger().print("Failed to utilize the DX device in the creation of a texture.");
  }
  device->pImmediateContext->UpdateSubresource(dxTX->m_t2d, 0, nullptr, data, pitch, 0);

  // free the texture data if there's data to release
  if (data) { stbi_image_free(data); }

  // set the path
  dxTX->path = _fileName;

  // return the texture
  return dxTX;
}

SPtr<Texture>
DX11GraphicsAPI::createTexture(unsigned char* _data,
                               uint32 _bpp,
                               uint32 _width,
                               uint32 _height,
                               uint32 _format,
                               uint32 _usage,
                               uint32 _bindFlags,
                               bool _mipLevels,
                               uint32 _shaderResourceFormat)
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
  D3D11_SUBRESOURCE_DATA* initData = nullptr;
  if (_data) {
    initData = new D3D11_SUBRESOURCE_DATA();
    initData->pSysMem = _data;
    initData->SysMemPitch = static_cast<uint32>(_width * _bpp);
    initData->SysMemSlicePitch = 0;
  }

  // create the texture
  SPtr<DX11Texture> tex = make_shared<DX11Texture>();

  auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
  if (!device) {
    g_Logger().print("Failed to utilize the DX device in the creation of a texture.");
  }
  // hresult
  int32 hr = 0;
  /**
   * Set shader resource.
   */
  if ((_bindFlags & D3D11_BIND_SHADER_RESOURCE) == D3D11_BIND_SHADER_RESOURCE) {
    // Create the shader resource descriptor for the texture
    D3D11_SHADER_RESOURCE_VIEW_DESC sDesc;
    memset(&sDesc, 0, sizeof(sDesc));
    sDesc.Format = static_cast<DXGI_FORMAT>(_shaderResourceFormat); // desc.Format; // DXGI_FORMAT_R32_FLOAT
    sDesc.Texture2D.MipLevels = desc.MipLevels;
    sDesc.Texture2D.MostDetailedMip = 0;
    sDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

    // create the texture
    hr = device->pd3dDevice->CreateTexture2D(&desc, initData, &tex->m_t2d);
    // if texture creation failed
    if (FAILED(hr)) {
      String errMsg = g_Logger().getMessageError(hr);
      g_Logger().print("Failed to create a texture. Error: " + errMsg);
    }
    // create the shader resource view
    hr = device->pd3dDevice->CreateShaderResourceView(tex->m_t2d, &sDesc, &tex->m_srv);
    // if failed to create shader resource view
    if (FAILED(hr)) {
      String errMsg = g_Logger().getMessageError(hr);
      g_Logger().print("Failed to create a shader resource view. Error: " + errMsg);
    }
  }
  /**
   * Set Depth stencil
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

    // create the texture
    hr = device->pd3dDevice->CreateTexture2D(&descDepth, nullptr, &tex->m_t2d);
    if (!tex->m_t2d) {
      String errMsg = g_Logger().getMessageError(hr);
      g_Logger().print("Failed to create the depth stencil. Error: " + errMsg);
    }
  }
  /**
   * Set render target
   */
  if ((_bindFlags & D3D11_BIND_RENDER_TARGET) == D3D11_BIND_RENDER_TARGET) {
    // render target description
    D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
    memset(&rtvDesc, 0, sizeof(rtvDesc));
    rtvDesc.Format = desc.Format;
    rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    rtvDesc.Texture2D.MipSlice = 0;

    // create the render target texture
    hr = device->pd3dDevice->CreateTexture2D(&desc, initData, &tex->m_t2d);
    if (FAILED(hr)) {
      String errMsg = g_Logger().getMessageError(hr);
      g_Logger().print("Failed to create RT texture. Error: " + errMsg);
    }
    // create the render target itself
    hr = device->pd3dDevice->CreateRenderTargetView(tex->m_t2d, &rtvDesc, &tex->m_rTV);
    if (FAILED(hr)) {
      String errMsg = g_Logger().getMessageError(hr);
      g_Logger().print("Failed to create a render target view. Error: " + errMsg);
    }
    /**
     * Set unordered access
     */
    if ((_bindFlags & D3D11_BIND_UNORDERED_ACCESS) == D3D11_BIND_UNORDERED_ACCESS) {
      D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
      memset(&uavDesc, 0, sizeof(uavDesc));
      uavDesc.Format = desc.Format;
      // uavDesc.
    }
  }
  return tex;
}

void
DX11GraphicsAPI::setInputLayout(SPtr<InputLayout> _pInputLayout)
{
  // reinterpret to a DirectX input layout
  SPtr<DX11InputLayout> dxIL = reinterpret_pointer_cast<DX11InputLayout>(_pInputLayout);
  if (!dxIL) { g_Logger().print("Failed to set the input layout."); }
  // set the device
  dxIL->set(m_pDevice);
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

  auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
  if (!device) {
    g_Logger().print("Failed to utilize the DX device in the creation of a vertex buffer.");
  }
  int32 hr = device->pd3dDevice->CreateBuffer(&bd, &InitData, &dxVB->pBuffer);
  if (FAILED(hr)) {
    String errMsg = g_Logger().getMessageError(hr);
    g_Logger().print("Failed to create a vertex buffer. Error: " + errMsg);
  }
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
    g_Logger().print("Failed to set the vertex buffer.");
    return;
  }
  
  // get the offset
  uint32 stride = sizeof(SimpleVertex);
  //set the buffer
  auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
  if (!device) {
    g_Logger().print("Failed to utilize the DX device in the setting of a vertex buffer.");
  }
  device->pImmediateContext->IASetVertexBuffers(_start,
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
  auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
  if (!device) {
    g_Logger().print("Failed to utilize the DX device in the creation of an index buffer.");
  }
  int32 hr = device->pd3dDevice->CreateBuffer(&bd, &InitData, &dxIB->pBuffer);
  if (FAILED(hr)) {
    String errMsg = g_Logger().getMessageError(hr);
    g_Logger().print("failed to create an index buffer. Error: " + errMsg);
  }
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
    g_Logger().print("Failed to set the index buffer.");
    return;
  }
  auto device = reinterpret_pointer_cast<DX11Device>(m_pDevice);
  if (!device) {
    g_Logger().print("Failed to utilize the DX device in the setting of an index buffer.");
  }
  device->pImmediateContext->IASetIndexBuffer(dxIB->pBuffer,
                                              static_cast<DXGI_FORMAT>(_format),
                                              _offset);
}
}
#endif