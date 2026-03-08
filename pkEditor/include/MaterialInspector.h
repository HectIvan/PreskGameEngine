/*****************************************************************************/
/**
 * @file    MaterialInspector.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    17/01/2026
 * @brief   Inspector for Materials to show in the UI.
 *
 * @bug    .- User is unable to close this window.
 */
 /*****************************************************************************/
#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkMaterial.h"
#include "pkWindow.h"

using pkEngineSDK::Material;
using pkEngineSDK::SPtr;
using pkEngineSDK::uint32;
using pkEngineSDK::Window;

class MaterialInspector
{
 public:
  MaterialInspector() = default;
  MaterialInspector(const SPtr<Material>& _pMaterial) : m_material(_pMaterial) {};
  virtual ~MaterialInspector() = default;

  /**
   * @brief Set the material to inspect.
   * @param _pMaterial Pointer to the material.
   */
  void
  setMaterial(const SPtr<Material>& _pMaterial) { m_material = _pMaterial; }

  /**
   * @brief Get the material being inspected.
   * @return Pointer to the material.
   */
  SPtr<Material>&
  getMaterial() { return m_material; }

  /**
   * @brief Create a window for the ui based on the material properties.
   */
  void
  createMaterialWindow();

 private:
  SPtr<Material> m_material;
  float m_iconSize = 45.0f;
};
