/*****************************************************************************/
/**
 * @file    MaterialInspector.cpp
 * @author  Héctor Iván Muñoz Ceballos
 * @date    17/01/2026
 * @brief   Inspector for Materials to show in the UI.
 *
 * @bug    No known bugs.
 */
 /*****************************************************************************/
#include "MaterialInspector.h"
#include "pkUInterface.h"
#include "pkTextureManager.h"
#include "pkShader.h"

using pkEngineSDK::g_TextureManager;
using pkEngineSDK::g_uInterface;
using pkEngineSDK::MaterialProps;
using pkEngineSDK::Path;
using pkEngineSDK::Shader;
using pkEngineSDK::String;
using pkEngineSDK::Texture;
using pkEngineSDK::TextureManager;
using pkEngineSDK::UInterface;
using pkEngineSDK::UUID;
using pkEngineSDK::Vector2;

void
MaterialInspector::createMaterialWindow()
{
  // if no material is selected.
  if (!m_material) {
    return;
  }

  UInterface& im = g_uInterface();
  TextureManager& tm = g_TextureManager();

  im.createText("Texture UI Image Size");
  im.sameLine();
  im.createDrag("##TextureUIImageSize", m_iconSize, 1.0f, 1.0f);

  const Vector2 texSize = Vector2(m_iconSize, m_iconSize);

  String matName = m_material->getName();
  im.createText("Material Name: ");
  im.sameLine();
  im.createInputText("##MaterialName: ", &matName);

  SPtr<Shader> shader = m_material->m_shader.lock();
  if (shader) {
    im.createText("Shader - ");
    im.sameLine();
    im.createText(shader->getShaderName().c_str());
    im.createText("Model - ");
    im.sameLine();
    im.createText(shader->getShaderModel());
    im.createText("Entry Point - ");
    im.sameLine();
    im.createText(shader->getEntryPoint());
  }
  else {
    im.createText("Warning!!! No shader assigned to this material.");
  }

  // get material
  MaterialProps& matProps = m_material->m_properties;
  // get the textures
  const SPtr<Texture> albedo = m_material->m_albedo.lock();
  const SPtr<Texture> normal = m_material->m_normal.lock();
  const SPtr<Texture> oclussion = m_material->m_oclussion.lock();
  const SPtr<Texture> rough = m_material->m_roughness.lock();
  const SPtr<Texture> metallic = m_material->m_metallic.lock();
  const SPtr<Texture> emissive = m_material->m_emissive.lock();
  /***************************************************************/
  /*------------------------albedo button-----------------------*/
  /***************************************************************/
  if (albedo) {
    const String albName = albedo->getName() + "diff";
    if (im.createButtonImage(albName.c_str(), albedo, texSize)) {}
    if (im.beginDragDropSource()) {
      const String dragText = "Dragging " + albName;
      im.createText(dragText.c_str());
      const UUID textureID = albedo->getID();
      im.setDragDropPayload("RESOURCE_PAYLOAD", &textureID, sizeof(UUID));
      im.endDragDropSource();
    }
    if (im.beginDragDropTarget()) {
      const UUID* id = reinterpret_cast<UUID*>(im.acceptDragDropPayload("RESOURCE_PAYLOAD"));
      if (id) {
        SPtr<Texture> texture = tm.createTexture(*id);
        m_material->setAlbedo(texture);
      }
      im.endDragDropTarget();
    }
    // hover tooltip.
    if (im.isItemHovered()) {
      im.setTooltip("Albedo Texture");
    }
    im.sameLine();
    im.colorEdit("Color Multiply", matProps.ColorMultiply);
  }
  else {
    if (im.createButton("##AlbedoButton")) {}
    if (im.beginDragDropTarget()) {
      const UUID* id = reinterpret_cast<UUID*>(im.acceptDragDropPayload("RESOURCE_PAYLOAD"));
      if (id) {
        SPtr<Texture> texture = tm.createTexture(*id);
        m_material->setAlbedo(texture);
      }
      im.endDragDropTarget();
    }
    if (im.isItemHovered()) {
      im.setTooltip("Warning!!! Albedo texture is null.");
    }
  }

  /***************************************************************/
  /*------------------------Normal button------------------------*/
  /***************************************************************/

  if (normal) {
    const String norName = normal->getName() + "norm";
    if (im.createButtonImage(norName.c_str(), normal, texSize)) {}
    if (im.beginDragDropSource()) {
      const String dragText = "Dragging " + norName;
      im.createText(dragText.c_str());
      const UUID textureID = normal->getID();
      im.setDragDropPayload("RESOURCE_PAYLOAD", &textureID, sizeof(UUID));
      im.endDragDropSource();
    }
    if (im.beginDragDropTarget()) {
      const UUID* id = reinterpret_cast<UUID*>(im.acceptDragDropPayload("RESOURCE_PAYLOAD"));
      if (id) {
        SPtr<Texture> texture = tm.createTexture(*id);
        m_material->setNormal(texture);
      }
      im.endDragDropTarget();
    }
    // hover tooltip.
    if (im.isItemHovered()) {
      im.setTooltip("Normal Texture");
    }
  }
  else {
    if (im.createButton("##NormalButton")) {}
    if (im.beginDragDropTarget()) {
      const UUID* id = reinterpret_cast<UUID*>(im.acceptDragDropPayload("RESOURCE_PAYLOAD"));
      if (id) {
        SPtr<Texture> texture = tm.createTexture(*id);
        m_material->setNormal(texture);
      }
      im.endDragDropTarget();
    }
    if (im.isItemHovered()) {
      im.setTooltip("Warning!!! Normal texture is null.");
    }
  }

  /***************************************************************/
  /*----------------------oclussion button-----------------------*/
  /***************************************************************/

  if (oclussion) {
    const String occName = oclussion->getName() + "ao";
    if (im.createButtonImage(occName.c_str(), oclussion, texSize)) {}
    if (im.beginDragDropSource()) {
      const String dragText = "Dragging " + occName;
      im.createText(dragText.c_str());
      const UUID textureID = oclussion->getID();
      im.setDragDropPayload("RESOURCE_PAYLOAD", &textureID, sizeof(UUID));
      im.endDragDropSource();
    }
    if (im.beginDragDropTarget()) {
      const UUID* id = reinterpret_cast<UUID*>(im.acceptDragDropPayload("RESOURCE_PAYLOAD"));
      if (id) {
        SPtr<Texture> texture = tm.createTexture(*id);
        m_material->setOclussion(texture);
      }
      im.endDragDropTarget();
    }
    // hover tooltip.
    if (im.isItemHovered()) {
      im.setTooltip("Ambient Occlusion Texture");
    }
  }
  else {
    if (im.createButton("##OclussionButton")) {}
    if (im.beginDragDropTarget()) {
      const UUID* id = reinterpret_cast<UUID*>(im.acceptDragDropPayload("RESOURCE_PAYLOAD"));
      if (id) {
        SPtr<Texture> texture = tm.createTexture(*id);
        m_material->setOclussion(texture);
      }
      im.endDragDropTarget();
    }
    if (im.isItemHovered()) {
      im.setTooltip("Warning!!! Oclussion texture is null.");
    }
  }

  /***************************************************************/
  /*-----------------------roughness button----------------------*/
  /***************************************************************/

  if (rough) {
    const String roughName = rough->getName() + "rough";
    if (im.createButtonImage(roughName.c_str(), rough, texSize)) {}
    if (im.beginDragDropSource()) {
      const String dragText = "Dragging " + roughName;
      im.createText(dragText.c_str());
      const UUID textureID = rough->getID();
      im.setDragDropPayload("RESOURCE_PAYLOAD", &textureID, sizeof(UUID));
      im.endDragDropSource();
    }
    if (im.beginDragDropTarget()) {
      const UUID* id = reinterpret_cast<UUID*>(im.acceptDragDropPayload("RESOURCE_PAYLOAD"));
      if (id) {
        SPtr<Texture> texture = tm.createTexture(*id);
        m_material->setRoughness(texture);
      }
      im.endDragDropTarget();
    }
    // hover tooltip.
    if (im.isItemHovered()) {
      im.setTooltip("Roughness Texture");
    }
    im.sameLine();
    im.createDrag("Roughness Strength", matProps.roughnessMultiply, 0.01f, 0.0f, 1.0f);
  }
  else {
    if (im.createButton("##RoughnessButton")) {}
    if (im.beginDragDropTarget()) {
      const UUID* id = reinterpret_cast<UUID*>(im.acceptDragDropPayload("RESOURCE_PAYLOAD"));
      if (id) {
        SPtr<Texture> texture = tm.createTexture(*id);
        m_material->setRoughness(texture);
      }
      im.endDragDropTarget();
    }
    if (im.isItemHovered()) {
      im.setTooltip("Warning!!! Roughness texture is null.");
    }
  }

  /***************************************************************/
  /*-----------------------metallic button-----------------------*/
  /***************************************************************/

  if (metallic) {
    const String metName = metallic->getName() + "metal";
    if (im.createButtonImage(metName.c_str(), metallic, texSize)) {}
    if (im.beginDragDropSource()) {
      const String dragText = "Dragging " + metName;
      im.createText(dragText.c_str());
      const UUID textureID = metallic->getID();
      im.setDragDropPayload("RESOURCE_PAYLOAD", &textureID, sizeof(UUID));
      im.endDragDropSource();
    }
    if (im.beginDragDropTarget()) {
      const UUID* id = reinterpret_cast<UUID*>(im.acceptDragDropPayload("RESOURCE_PAYLOAD"));
      if (id) {
        SPtr<Texture> texture = tm.createTexture(*id);
        m_material->setMetallic(texture);
      }
      im.endDragDropTarget();
    }
    // hover tooltip.
    if (im.isItemHovered()) {
      im.setTooltip("Metallic Texture");
    }
    im.sameLine();
    im.createDrag("Metallic Strength", matProps.metallicMultiply, 0.01f, 0.0f, 1.0f);
  }
  else {
    if (im.createButton("##MetallicButton")) {}
    if (im.beginDragDropTarget()) {
      const UUID* id = reinterpret_cast<UUID*>(im.acceptDragDropPayload("RESOURCE_PAYLOAD"));
      if (id) {
        SPtr<Texture> texture = tm.createTexture(*id);
        m_material->setMetallic(texture);
      }
      im.endDragDropTarget();
    }
    if (im.isItemHovered()) {
      im.setTooltip("Warning!!! Metallic texture is null.");
    }
  }

  /***************************************************************/
  /*-----------------------emissive button-----------------------*/
  /***************************************************************/

  if (emissive) {
    const String emissName = emissive->getName() + "emissive";
    if (im.createButtonImage(emissName.c_str(), emissive, texSize)) {}
    if (im.beginDragDropSource()) {
      const String dragText = "Dragging " + emissName;
      im.createText(dragText.c_str());
      const UUID textureID = emissive->getID();
      im.setDragDropPayload("RESOURCE_PAYLOAD", &textureID, sizeof(UUID));
      im.endDragDropSource();
    }
    if (im.beginDragDropTarget()) {
      const UUID* id = reinterpret_cast<UUID*>(im.acceptDragDropPayload("RESOURCE_PAYLOAD"));
      if (id) {
        SPtr<Texture> texture = tm.createTexture(*id);
        m_material->setEmissive(texture);
      }
      im.endDragDropTarget();
    }
    // hover tooltip.
    if (im.isItemHovered()) {
      im.setTooltip("Emissive Texture");
    }
    im.sameLine();
    im.colorEdit("Emissive Color", matProps.EmissiveMultiply);
  }
  else {
    if (im.createButton("##EmissiveButton")) {}
    if (im.beginDragDropTarget()) {
      const UUID* id = reinterpret_cast<UUID*>(im.acceptDragDropPayload("RESOURCE_PAYLOAD"));
      if (id) {
        SPtr<Texture> texture = tm.createTexture(*id);
        m_material->setEmissive(texture);
      }
      im.endDragDropTarget();
    }
    if (im.isItemHovered()) {
      im.setTooltip("Warning!!! Emissive texture is null.");
    }
  }
}
