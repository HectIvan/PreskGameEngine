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
using pkEngineSDK::Window;

class MaterialInspector
{
 public:
  MaterialInspector() = default;
  MaterialInspector(const SPtr<Material>& _pMaterial) : m_material(_pMaterial) {};
  virtual ~MaterialInspector() = default;

  /**
   * @brief Create a window for the ui based on the material properties.
   * @param _window Main window.
   * @param _imgTexSize Size of the texture preview image.
   */
  void
  createMaterialWindow(const Window& _window,
                       const float& _imgTexSize);

 public:
  SPtr<Material> m_material;
};
