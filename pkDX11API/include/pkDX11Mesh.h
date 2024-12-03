/************************************************************************/
/**
* @pkDX11Mesh pkDX11Mesh.h
* @Hector Ivan Muñoz Ceballos
* @date 22/11/2024
* @DirectX mesh file for the Presk Game Engine.
*
* DirectX mesh base class for the engine
*
* @bug No bug known.
*
* @HectIvan 02/12/2024
* Created the file
*/
/************************************************************************/
#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkDX11Prerequisites.h"
#include "pkMesh.h"

namespace pkEngineSDK
{

class DX11Mesh : public PkMesh
{
  DX11Mesh() = default;
  virtual ~DX11Mesh() = default;

  /**
  * Render the mesh.
  * 
  * @param _pDevice
  * Device that will be used to render.
  **/
  void
  render(Device* _pDevice) override;
};
}