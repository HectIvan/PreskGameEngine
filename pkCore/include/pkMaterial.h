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
* @HectIvan 21/11/2024
* added a way to get the material current component type.
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
   * @brief Get the component type of this component.
   * @return The component type.
   */
  COMPONENT_TYPE::E
  getType() override { return COMPONENT_TYPE::kMaterial; }

  static COMPONENT_TYPE::E
  getObjType() { return COMPONENT_TYPE::kMaterial; }

  /**
   * @brief Set the diffuse texture of the material.
   */
  void
  setTexture(SPtr<Texture>& _pTexture, SPtr<Texture>& _pNewTexture);

 public:
  String m_name;
  bool m_castShadow;
  bool m_receiveShadows;
  SPtr<Texture> diffuse;
  SPtr<Texture> metallic;
  SPtr<Texture> normal;
  SPtr<Texture> height;
  SPtr<Texture> occlusion;
};
}