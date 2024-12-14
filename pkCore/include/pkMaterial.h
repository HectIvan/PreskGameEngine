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
#include "pkPrerequisitesCore.h"

namespace pkEngineSDK
{

class Texture;

class Material
{
 public:
  Material() = default;
  virtual ~Material() = default;

  /**
  * Insert a new texture to the material.
  **/
  void
  insertTexture(Texture* _texture) { textures.push_back(_texture); }

 public:
  Vector<Texture*> textures;
};
}