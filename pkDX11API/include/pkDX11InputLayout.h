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
#include "pkInputLayout.h"

namespace pkEngineSDK
{

class Device;
class Shader;

using std::dynamic_pointer_cast;

class DX11InputLayout : public InputLayout
{
 public:
  DX11InputLayout() = default;
  virtual ~DX11InputLayout()
  {
    safeRelease(pVertexLayout);
  }

  /**
  * Set the input layout.
  * 
  * @param _pDevice
  * Device where the input layout is located
  **/
  void
  set(SPtr<Device> _pDevice);

 public:
  uint32 numElements = 0;
  ID3D11InputLayout* pVertexLayout = nullptr;
};
}