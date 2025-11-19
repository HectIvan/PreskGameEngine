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

struct MaterialProps
{
  // union {
  //   struct {
  //     uint32 m_hasAlbedo : 1;
  //     uint32 m_hasSpecular : 1;
  //     uint32 m_hasNormal : 1;
  //     uint32 m_opaque : 1;
  //     uint32 m_hasAlphaTest : 1;
  //     uint32 m_alphaBlend : 1;
  //     uint32 m_doubleSided : 1;
  //     uint32 m_wireFrame : 1;
  //     uint32 m_castShadow : 1;
  //     uint32 m_receiveShadow : 1;
  //     uint32 unused : 22;
  //   } flags;
  //   uint32 value = 0;
  // } properties;

  Vector3 ColorMultiply = Vector3(1.0f);
  float roughnessMultiply = 1.0f;
  Vector3 EmissiveMultiply = Vector3(1.0f);
  float metallicMultiply = 1.0f;
};

// check the bit in question in hex
// bool hasAlbedo(uint32 _props) {
//   return (_props & 0x1) != 0;
// }

class Texture;

class Material : public Component
{
 public:
  /**
   * @brief Default material constructor.
   */
  Material(const String name = "newMaterial");

  /**
   * @brief Default material destructor.
   */
  virtual ~Material() = default;

  /**
   * @brief Initialize the material on defaults.
   */
  void
  init();

  /**
   * @brief Get the component type of this component.
   * @return The component type.
   */
  COMPONENT_TYPE::E
  getType() override { return COMPONENT_TYPE::kMaterial; }

  /**
   * @brief Get the name of the component.
   */
  const ANSICHAR*
  getName() override { return m_name.c_str(); }

  /**
   * @brief Get the component type of this class.
   * @return The class type.
   */
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
   * @brief Set the albedo texture.
   * @param _pTexture New albedo texture.
   */
  PKFORCEINLINE void
  setAlbedo(const SPtr<Texture>& _pTexture) { m_albedo = _pTexture; }

  /**
   * @brief Set the normal texture.
   * @param _pTexture New normal texture.
   */
  PKFORCEINLINE void
  setNormal(const SPtr<Texture>& _pTexture) { m_normal = _pTexture; }

  /**
   * @brief Set the height texture.
   * @param _pTexture New height texture.
   */
  PKFORCEINLINE void
  setHeight(const SPtr<Texture>& _pTexture) { m_height = _pTexture; }

  /**
   * @brief Set the metallic texture.
   * @param _pTexture New metallic texture.
   */
  PKFORCEINLINE void
  setMetallic(const SPtr<Texture>& _pTexture) { m_metallic = _pTexture; }

  /**
   * @brief Set the occlusion texture.
   * @param _pTexture New occlusion texture.
   */
  PKFORCEINLINE void
  setOclussion(const SPtr<Texture>& _pTexture) { m_oclussion = _pTexture; }

  /**
   * @brief Set the roughness texture.
   * @param _pTexture New roughness texture.
   */
  PKFORCEINLINE void
  setRoughness(const SPtr<Texture>& _pTexture) { m_roughness = _pTexture; }
  
  /**
   * @brief Set the emissive texture.
   * @param _pTexture New emissive texture.
   */
  PKFORCEINLINE void
  setEmissive(const SPtr<Texture>& _pTexture) { m_emissive = _pTexture; }

  /**
   * @brief Set the material name.
   * @param _name The material name.
   */
  void
  setName(const String _name) { m_name = _name; }

  /**
   * @brief Get the material name.
   * @return The material name.
   */
  String
  getNameS() const { return m_name; }

 public:
  String m_name;
  bool m_castShadow;
  bool m_receiveShadows;
  MaterialProps m_properties;
  SPtr<Texture> m_albedo;
  SPtr<Texture> m_normal;
  SPtr<Texture> m_height;
  SPtr<Texture> m_metallic;
  SPtr<Texture> m_oclussion;
  SPtr<Texture> m_roughness;
  SPtr<Texture> m_emissive;
};
}