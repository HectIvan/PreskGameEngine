/*****************************************************************************/
/**
 * @file    ResourceInspector.cpp
 * @author  Héctor Iván Muñoz Ceballos
 * @date    17/01/2026
 * @brief   Inspector to show all resources in a window.
 *
 * @bug    No known bugs.
 */
 /*****************************************************************************/
#include "ResourceInspector.h"
#include "pkAssetResourceManager.h"
#include "pkModelCodec.h"
#include "pkTextureCodec.h"
#include "pkUInterface.h"
#include "pkMaterialManager.h"
#include "pkModelResource.h"
#include "pkTextureResource.h"
#include "pkWindow.h"

using pkEngineSDK::ANSICHAR;
using pkEngineSDK::AssetResourceManager;
using pkEngineSDK::BaseResource;
using pkEngineSDK::g_AssetResourceManager;
using pkEngineSDK::g_MaterialManager;
using pkEngineSDK::g_ModelCodec;
using pkEngineSDK::g_TextureCodec;
using pkEngineSDK::g_uInterface;
using pkEngineSDK::ModelCodec;
using pkEngineSDK::Path;
using pkEngineSDK::RESOURCE_TYPE::kMaterial;
using pkEngineSDK::RESOURCE_TYPE::kShader;
using RES_TYPE = pkEngineSDK::RESOURCE_TYPE::E;
using pkEngineSDK::SPtr;
using pkEngineSDK::stringToLower;
using pkEngineSDK::TextureCodec;
using pkEngineSDK::UInterface;
using pkEngineSDK::UUID;
using pkEngineSDK::Vector2;
using pkEngineSDK::UMap;

void
ResourceInspector::createResourceWindow(const Window& _window)
{
  AssetResourceManager& assetMan = g_AssetResourceManager();
  ModelCodec& modelCodec = g_ModelCodec();
  UInterface& im = g_uInterface();
  TextureCodec& textureCodec = g_TextureCodec();

  
  if (im.createButton("Model Resource")) {
    const Path path = _window.openFileFromExplorer();
    if (path.toString() != "") {
      SPtr<BaseResource> resource = modelCodec.createResourceFromFile(path);
      if (resource) {
        assetMan.insertNewResource(resource);
      }
    }
  }
  im.sameLine();
  if (im.createButton("Texture Resource")) {
    const Path path = _window.openFileFromExplorer();
    if (path.toString() != "") {
      SPtr<BaseResource> resource = textureCodec.createResourceFromFile(path);
      if (resource) {
        assetMan.insertNewResource(resource);
      }
    }
  }
  im.sameLine();
  im.createText("Search:");
  im.sameLine();
  im.createInputText("##Search", &m_searchResource);
  im.sameLine();
  // resoruces header
  if (im.beginTable("resources params", 4)) {
    im.tableNextRow();

    im.tableNextColumn();
    im.createText("Item Size:");

    im.tableNextColumn();
    im.createDragU("##ItemSize", m_itemSize, 1, 1, 999);

    im.tableNextColumn();
    im.createText("Column Count:");

    im.tableNextColumn();
    im.createDragU("##ColumnCount", m_columnCount, 1, 1, 64);

    im.endTable();
  }
  im.endTabItem();
  // -------------------------- //
  // window for displaying resources
  // -------------------------- //
  uint32 column = 0;
  if (im.beginTable("Editor App", m_columnCount)) {
    im.tableNextRow();
    const UMap<UUID, SPtr<BaseResource>> allResources = assetMan.getAllResources();
    for (auto& asset : allResources) {
      // search filter
      const Path assetPath = String(asset.second->m_resourcePath);
      const String assetName = assetPath.getFileName();
      const String searchResLower = stringToLower(m_searchResource);
      const String assetNameLower = stringToLower(assetName);
    
      // resource search filter result
      const RES_TYPE assetType = asset.second->getType();
      if (assetType != kShader && assetNameLower.find(searchResLower) != String::npos) {
        im.tableSetColumnIndex(column);
        const ANSICHAR* assetNameCstr = assetName.c_str();
        // create selectable image for each resource
        if (im.selectable2(assetNameCstr, Vector2(m_itemSize))) {}
        if (im.beginDragDropSource()) {
          const String dragText = "Dragging " + assetName;
          im.createText(dragText.c_str());
          const UUID* data = &asset.first;
          im.setDragDropPayload("RESOURCE_PAYLOAD", data, sizeof(UUID));
          im.endDragDropSource();
        }
        if (im.isItemHovered()) {
          const String tooltip = "Name: " + assetName + "\n" +
                                 "Asset ID: " + asset.first.toString() + "\n" +
                                 "Asset type: " + asset.second->getTypeString() + "\n" +
                                 "Loaded: " + (asset.second->m_isLoaded ? "Yes" : "No");
          im.setTooltip(tooltip.c_str());
        }
        // pop-up menu for each resource
        if (im.beginPopUpItem(assetNameCstr)) {
          if (im.menuItem("Load")) {
            asset.second->load();
            assetMan.insertLoadedResource(asset.second);
          }
          if (im.menuItem("Unload")) {
            assetMan.removeLoadedResource(asset.first);
            asset.second->unload();
          }
          if (im.menuItem("Remove")) {
            assetMan.deleteResource(asset.first);
          }
          if (im.menuItem("_______________________")) {

          }
          if (im.menuItem("Delete (Non-Functional)")) {

          }
          if (im.isItemHovered()) {
            im.setTooltip("THIS ACTION CANNOT BE UNDONE");
          }
          im.endPopUpItem();
        }

        // if the resource window is full, jump to next row.
        ++column;
        if (column >= m_columnCount) {
          column = 0;
          im.tableNextRow();
        }
      }
    }
    im.endTable();
  }
}
