/*****************************************************************************/
/**
 * @file    GraphicsInspector.cpp
 * @author  Héctor Iván Muñoz Ceballos
 * @date    11/03/2026
 * @brief   Inspector for the Editor App graphics pannel.
 *
 * @bug     No known bugs.
 */
 /*****************************************************************************/

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "GraphicsInspector.h"
#include "pkPath.h"
#include "pkRendererManager.h"
#include "pkUInterface.h"
#include "pkUUID.h"
#include "pkTextureManager.h"
#include "pkShaderManager.h"
#include "pkPrerequisitesCore.h"

using pkEngineSDK::g_uInterface;
using pkEngineSDK::g_RenderManager;
using pkEngineSDK::g_ShaderManager;
using pkEngineSDK::g_TextureManager;
using pkEngineSDK::Path;
using pkEngineSDK::PK_TREENODE_FLAGS::kPK_DefaultOpen;
using pkEngineSDK::RendererManager;
using pkEngineSDK::Shader;
using pkEngineSDK::ShaderManager;
using pkEngineSDK::SPtr;
using pkEngineSDK::String;
using pkEngineSDK::Texture;
using pkEngineSDK::TextureManager;
using pkEngineSDK::uint32;
using pkEngineSDK::UInterface;
using pkEngineSDK::UUID;
using pkEngineSDK::Vector;
using pkEngineSDK::WPtr;

void
GraphicsInspector::init(Window& _window,
                        bool& _ibl,
                        float& _iblInt,
                        float& _exp,
                        float& _blurRad,
                        float& _blurStrength,
                        float& _lumThreshold,
                        float& _emissRad,
                        float& _emissStr,
                        bool& _ssao,
                        float& _ssaoRad,
                        float& _ssaoScale,
                        float& _ssaoBias,
                        float& _ssaoInt)
{
  UInterface& im = g_uInterface();
  RendererManager& rm = g_RenderManager();
  ShaderManager& shaderMan = g_ShaderManager();
  TextureManager& tm = g_TextureManager();
  // table parameters
  if (im.beginTable("Graphics")) {

    // IBL
    im.tableNextColumn();
    im.createCheckBox("IBL Active", _ibl);
    if (_ibl) {
      // Slider for IBL intensity.
      if (im.createButtonImage("Skybox", rm.m_mainSkybox)) {
        Path path = _window.openFileFromExplorer("Texture Files",
          "*.png;*.jpeg;*.jpg;*.tga;*.hdr;*.exr");
        if (path.toString() != "") {
          // SPtr<Texture> texture = tm.loadTexture(path);
          // rm.m_mainSkybox->copyFrom(texture);
        }
      }
      if (im.beginDragDropTarget()) {
        const UUID* id = reinterpret_cast<UUID*>(im.acceptDragDropPayload("RESOURCE_PAYLOAD"));
        if (id) {
          SPtr<Texture> texture = tm.createTexture(*id);
          rm.m_mainSkybox->copyFrom(texture);
        }
        im.endDragDropTarget();
      }
      if (im.isItemHovered()) {
        im.setTooltip("Skybox");
      }
      im.tableNextColumn();
      im.createDrag("##iblIntensity", _iblInt, 0.1f, 0.0f, 1.0f);
    }

    // EXPOSURE
    im.tableJumpRow();
    im.createText("Exposure");
    im.tableNextColumn();
    im.createDrag("##Exposure", _exp, 0.1f, 0.0f);

    // LUMINANCE
    im.tableJumpRow();
    im.createText("----------Luminance----------");
    im.tableJumpRow();
    im.createText("Radius");
    im.tableNextColumn();
    im.createDrag("##LumRadius", _blurRad, 0.1f, 0.001f);
    im.tableJumpRow();
    im.createText("Strength");
    im.tableNextColumn();
    im.createDrag("##LumStrength", _blurStrength, 0.1f, 0.001f);
    im.tableJumpRow();
    im.createText("Threshhold");
    im.tableNextColumn();
    im.createDrag("##LumThreshold", _lumThreshold, 0.1f, 0.0f);
    im.tableJumpRow();

    // EMISSIVE
    im.createText("----------Emissive----------");
    im.tableJumpRow();
    im.createText("Radius");
    im.tableNextColumn();
    im.createDrag("##EmRadius", _emissRad, 1.0f, 0.001f);
    im.tableJumpRow();
    im.createText("Strength");
    im.tableNextColumn();
    im.createDrag("##EmStrength", _emissStr, 0.1f, 0.001f);
    im.tableJumpRow();

    // SSAO
    im.createText("----------SSAO----------");
    im.tableJumpRow();
    im.createCheckBox("SSAO", _ssao);
    im.tableJumpRow();
    if (_ssao) {
      im.createText("Sample Radius");
      im.tableNextColumn();
      im.createDrag("##SSAO Radius", _ssaoRad, 0.1f, 0.0f);
      im.tableJumpRow();
      im.createText("Scale");
      im.tableNextColumn();
      im.createDrag("##SSAOScale", _ssaoScale, 0.1f, 0.0f);
      im.tableJumpRow();
      im.createText("Bias");
      im.tableNextColumn();
      im.createDrag("##SSAOBias", _ssaoBias, 0.001f, 0.0f);
      im.tableJumpRow();
      im.createText("Intensity");
      im.tableNextColumn();
      im.createDrag("##SSAOIntensity", _ssaoInt, 0.1f, 0.0f);
    }
    im.tableNextRow();
  }
  im.endTable();

  // --- Post-Process window --- //
  if (im.collapsingHeader("Shaders", kPK_DefaultOpen)) {
    if (im.beginTable("Shaders table")) {
      im.tableNextColumn();
      // display all compiled shaders.
      Vector<SPtr<Shader>> shaders = shaderMan.getShaders();
      const uint32 shaderCount = static_cast<uint32>(shaders.size());
      for (uint32 i = 0; i < shaderCount; ++i) {
        WPtr<Shader> shader = shaders[i];
        String shaderName = shader.lock()->getShaderDirectory().getFileName();
        im.createText(shaderName.c_str());
        im.tableNextColumn();
        im.pushID(i);
        if (im.createButton("Compile")) {
          //shader->compileFromFile();
        }
        im.popID();
        im.tableJumpRow();
      }
      im.endTable();
    }
  }
}