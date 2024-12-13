/************************************************************************/
/**
* @pkDx11InputLayout pkDx11InputLayout.h
* @Hector Ivan Muñoz Ceballos
* @date 11/11/2024
* @DirectX Input Layout file for the Presk Game Engine.
*
* This file contains the Input Layout class using DirectX11 of the engine
*
* @bug No bug known.
*/
/************************************************************************/
#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkDX11Prerequisites.h"

namespace pkEngineSDK
{

class DX11Device;
class DX11VertexShader;

class DX11InputLayout
{
 public:
  DX11InputLayout() = default;
  virtual ~DX11InputLayout()
  {
    safeRelease(pVertexLayout);
  }

  /**
  * Create.
  * 
  * Create the Input layout.
  * 
  * @param _pDevice
  * What device the input layout will be created in.
  * 
  * @param _vs
  * Vertex shader to use.
  * 
  * @return
  * If the creation was successful or not.
  **/
  HRESULT
  create(DX11Device* _pDevice, DX11VertexShader _vs);

  /**
  * Set the input layout.
  * 
  * @param _pDevice
  * Device where the input layout is located
  **/
  void
  set(DX11Device* _pDevice);

 public:
  uint32 numElements = 0;
  ID3D11InputLayout* pVertexLayout = nullptr;
};
}