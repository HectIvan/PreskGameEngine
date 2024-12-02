/************************************************************************/
/**
* @pkDX11Device pkDX11Device.h
* @Hector Ivan Muñoz Ceballos
* @date 21/10/2024
* @DirectX11 Device file for the Presk Game Engine.
*
* This file contains the Device of DX11 class of the engine
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
#include "pkDevice.h"
#include "pkDX11Prerequisites.h"

namespace pkEngineSDK
{

class DX11Device : public Device
{
 public:
  DX11Device() = default;
  virtual ~DX11Device()
  {
    safeRelease(m_pd3dDevice);
    safeRelease(m_pImmediateContext);
  }

  /**
  * Get the new device as the child class of the parent
  **/
  UPtr<Device>
  getDevice() override;
  
  /**
  * Initialize device.
  * 
  * Initializes the DX11 device.
  * 
  * @param _pDriverType
  * Type of driver we will be using for the device.
  * 
  * @param _pFeatureLevel
  * Feature levels of the device.
  * 
  * @return
  * What was the end result of the creation
  **/
  HRESULT
  init(D3D_DRIVER_TYPE* _pDriverType, D3D_FEATURE_LEVEL* _pFeatureLevel);

  /**
  * Clean Device.
  * 
  * Clears all the pointers of the device.
  **/
  void
  clean();

  /**
  * Set primitive topology
  * 
  * @param _topology
  * The kind of topology we will be using.
  **/
  void
  setPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY _topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

 public:
  ID3D11Device* m_pd3dDevice = nullptr;
  ID3D11DeviceContext* m_pImmediateContext = nullptr;
  D3D_DRIVER_TYPE* m_pDriverType;
  D3D_FEATURE_LEVEL   m_featureLevel;
};
}
