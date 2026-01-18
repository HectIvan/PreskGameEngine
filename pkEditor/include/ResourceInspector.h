/*****************************************************************************/
/**
 * @file    ResourceInspector.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    17/01/2026
 * @brief   Inspector to show all resources in a window.
 *
 * @bug    No known bugs.
 */
 /*****************************************************************************/
#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkPrerequisitesCore.h"
#include "pkWindow.h"
#include "pkMaterial.h"

using pkEngineSDK::String;
using pkEngineSDK::uint32;
using pkEngineSDK::SPtr;
using pkEngineSDK::Window;
using pkEngineSDK::Material;

class ResourceInspector
{
 public:
  ResourceInspector() = default;
  virtual ~ResourceInspector() = default;
  /**
   * @brief Create the resource inspector window.
   * @param _window Window to use.
   * @param _pSelectedMaterial Pointer to the selected material.
   */
  void
  createResourceWindow(const Window& _window, SPtr<Material>& _pSelectedMaterial);

 private:
  String m_searchResource;
  uint32 m_columnCount = 8;
  uint32 m_itemSize = 128;
};