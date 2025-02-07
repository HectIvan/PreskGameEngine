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
* @HectIvan 06/02/2025
* added functions to set each texture in the material
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
   * @brief Set a texture of the material.
   * @param _pTexture Texture to change.
   * @param _pNewTexture New texture.
   */
  void
  setTexture(SPtr<Texture>& _pTexture, SPtr<Texture>& _pNewTexture);

  /**
   * @brief Set the diffuse texture.
   * @param _pTexture New diffuse texture.
   */
  FORCEINLINE void
  setDiffuse(SPtr<Texture> _pTexture) { diffuse = _pTexture; }

  /**
   * @brief Set the normal texture.
   * @param _pTexture New normal texture.
   */
  FORCEINLINE void
  setNormal(SPtr<Texture> _pTexture) { normal = _pTexture; }

  /**
   * @brief Set the height texture.
   * @param _pTexture New height texture.
   */
  FORCEINLINE void
  setHeight(SPtr<Texture> _pTexture) { height = _pTexture; }

  /**
   * @brief Set the metallic texture.
   * @param _pTexture New metallic texture.
   */
  FORCEINLINE void
  setMetallic(SPtr<Texture> _pTexture) { metallic = _pTexture; }

  /**
   * @brief Set the occlusion texture.
   * @param _pTexture New occlusion texture.
   */
  FORCEINLINE void
  setOcclusion(SPtr<Texture> _pTexture) { occlusion = _pTexture; }

 public:
  String m_name;
  bool m_castShadow;
  bool m_receiveShadows;
  SPtr<Texture> diffuse;
  SPtr<Texture> normal;
  SPtr<Texture> height;
  SPtr<Texture> metallic;
  SPtr<Texture> occlusion;
};
}