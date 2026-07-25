#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkPrerequisitesCore.h"

#define DX_VERSION_11_0 IN_USE
#define DX_VERSION_11_1 NOT_IN_USE
#define DX_VERSION_11_2 NOT_IN_USE
#define DX_VERSION_11_3 NOT_IN_USE
#define DX_VERSION_11_4 NOT_IN_USE

#if USING(DX_VERSION_11_0)
#   include <d3d11.h>
using D3DDevice = ID3D11Device;
using D3DDeviceContext = ID3D11DeviceContext;
using PKSwapChain = IDXGISwapChain;
using DXFactory = IDXGIFactory1;
using DXGIDevice = IDXGIDevice1;
using PKRenderTargetView = ID3D11RenderTargetView;
using PKRasterizerState = ID3D11RasterizerState;
using PKBlendState = ID3D11BlendState;

#elif USING(DX_VERSION_11_1)
#   include <d3d11_1.h>
using D3DDevice = ID3D11Device1;
using D3DDeviceContext = ID3D11DeviceContext1;
using PKSwapChain = IDXGISwapChain1;
using DXFactory = IDXGIFactory2;
using DXGIDevice = IDXGIDevice2;
using PKRenderTargetView = ID3D11RenderTargetView;
using PKRasterizerState = ID3D11RasterizerState1;
using PKBlendState = ID3D11BlendState1;

#elif USING(DX_VERSION_11_2)
#   include <d3d11_2.h>
using D3DDevice = ID3D11Device2;
using D3DDeviceContext = ID3D11DeviceContext2;
using PKSwapChain = IDXGISwapChain1;
using DXFactory = IDXGIFactory2;
using DXGIDevice = IDXGIDevice3;
using PKRenderTargetView = ID3D11RenderTargetView;
using PKRasterizerState = ID3D11RasterizerState1;
using PKBlendState = ID3D11BlendState1;

#elif USING(DX_VERSION_11_3)
#   include <d3d11_3.h>
using D3DDevice = ID3D11Device3;
using D3DDeviceContext = ID3D11DeviceContext3;
using PKSwapChain = IDXGISwapChain2;
using DXFactory = IDXGIFactory2;
using DXGIDevice = IDXGIDevice3;
using PKRenderTargetView = ID3D11RenderTargetView1;
using PKRasterizerState = ID3D11RasterizerState2;
using PKBlendState = ID3D11BlendState1;

#elif USING(DX_VERSION_11_4)
#   include <d3d11_4.h>
using D3DDevice = ID3D11Device4;
using D3DDeviceContext = ID3D11DeviceContext4;
using PKSwapChain = IDXGISwapChain4;
using DXFactory = IDXGIFactory2;
using DXGIDevice = IDXGIDevice3;
using PKRenderTargetView = ID3D11RenderTargetView1;
using PKRasterizerState = ID3D11RasterizerState2;
using PKBlendState = ID3D11BlendState1;

#else
#   error "No DirectX version defined. Please define a DirectX version to use."
#endif

using D3DResource = ID3D11Resource;
using D3DTexture1D = ID3D11Texture1D;
using D3DTexture2D = ID3D11Texture2D;
using D3DTexture3D = ID3D11Texture3D;

using PKShaderResourceView = ID3D11ShaderResourceView;
using PKDepthStencilView = ID3D11DepthStencilView;
using PKUnorderedAccessView = ID3D11UnorderedAccessView;
