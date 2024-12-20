/************************************************************************/
/**
* @pkMaterial pkMaterial.h
* @Hector Ivan Muñoz Ceballos
* @date 14/12/2024
* @Material file for the Presk Game Engine.
*
* This file will contain the Material class used for the engine
*
* @bug No bug known.
*
*/
/************************************************************************/
#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkComponent.h"

namespace pkEngineSDK
{

class Texture;

class Material : public Component
{
 public:
  Material() = default;
  virtual ~Material() = default;

  /**
  * Set the diffuse texture of the material.
  **/
  void
  setTexture(SPtr<Texture>& _pTexture, SPtr<Texture>& _pNewTexture);

 public:
  SPtr<Texture> diffuse;
  SPtr<Texture> metallic;
  SPtr<Texture> normal;
  SPtr<Texture> height;
  SPtr<Texture> occlusion;
};
}