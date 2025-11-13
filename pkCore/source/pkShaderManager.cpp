/*****************************************************************************/
/**
 * @file    pkShaderManager.cpp
 * @author  Héctor Iván Muñoz Ceballos
 * @date    10/06/2026
 * @brief   Shader Manager cpp file for the Engine.
 *
 * @bug    No known bugs.
 */
 /*****************************************************************************/

/*********************************************/
/**
 * Includes
 */
 /*********************************************/
#include "pkShaderCodec.h"
#include "pkShaderManager.h"
#include "pkGraphicsAPI.h"

namespace pkEngineSDK
{

void
ShaderManager::createShaderResources()
{
  // insert all desired shaders here.
  ShaderKey baseVertex("shaders/pkVShader.hlsl", "VS", "vs_5_0");
  ShaderKey basePixel("shaders/pkPShader.hlsl", "PS", "ps_5_0");
  ShaderKey positions("shaders/pkPShaderDepth.hlsl", "PS", "ps_5_0");
  ShaderKey quad("shaders/pkQuadShader.hlsl", "VS", "vs_5_0");
  ShaderKey shadowMap("shaders/pkShadowMapping.hlsl", "PS", "ps_5_0");
  ShaderKey skybox("shaders/pkSkyboxShader.hlsl", "PS", "ps_5_0");
  ShaderKey ssao("shaders/pkPSAOshader.hlsl", "PS", "ps_5_0");
  ShaderKey ibl("shaders/pkIBLShader.hlsl", "PS", "ps_5_0");
  ShaderKey blur("shaders/pkBlur.hlsl", "PS", "ps_5_0");
  ShaderKey merge("shaders/pkMergeShader.hlsl", "PS", "ps_5_0");
  ShaderKey luminance("shaders/pkLuminanceQuad.hlsl", "PS", "ps_5_0");
  ShaderKey toneMap("shaders/pkToneMap.hlsl", "PS", "ps_5_0");

  // initialize all shaders and create the resources.
  initShaderResource(baseVertex, PK_SHADER_TYPE::kVertex);
  initShaderResource(basePixel, PK_SHADER_TYPE::kPixel);
  initShaderResource(positions, PK_SHADER_TYPE::kPixel);
  initShaderResource(quad, PK_SHADER_TYPE::kVertex);
  initShaderResource(shadowMap, PK_SHADER_TYPE::kPixel);
  initShaderResource(skybox, PK_SHADER_TYPE::kPixel);
  initShaderResource(ssao, PK_SHADER_TYPE::kPixel);
  initShaderResource(ibl, PK_SHADER_TYPE::kPixel);
  initShaderResource(blur, PK_SHADER_TYPE::kPixel);
  initShaderResource(merge, PK_SHADER_TYPE::kPixel);
  initShaderResource(luminance, PK_SHADER_TYPE::kPixel);
  initShaderResource(toneMap, PK_SHADER_TYPE::kPixel);
}

SPtr<Shader>
ShaderManager::initShaderResource(const ShaderKey& _shaderData, const PK_SHADER_TYPE::E _type)
{
  GraphicsAPI& api = g_GraphicAPI();
  // create the shader base.
  SPtr<Shader> shader = api.internalCreateShader();
  shader->setData(Path(_shaderData.shaderPath),
                  _shaderData._szEntryPoint,
                  _shaderData._szShaderModel);
  shader->compile();
  // create the shader based on its type.
  if (PK_SHADER_TYPE::kVertex == _type) {
    api.createVShader(shader);
  }
  if (PK_SHADER_TYPE::kPixel == _type) {
    api.createPShader(shader);
  }
  if (PK_SHADER_TYPE::kCompute == _type) {
    api.createCShader(shader);
  }
  // create the resource from the shader.
  g_ShaderCodec().createResourceFromShader(shader);

  insertShader(_shaderData, shader);

  return shader;
}

void
ShaderManager::insertShader(const ShaderKey& _key, SPtr<Shader> _pShader)
{
  m_shaders.insert({ _key, _pShader });
}

SPtr<Shader>
ShaderManager::getShader(const ShaderKey& _key)
{
  bool hasShader = m_shaders.contains(_key);
  if (hasShader) {
    return m_shaders.find(_key)->second;
  }
  return nullptr;
}

Vector<SPtr<Shader>>
ShaderManager::getShaders()
{
  // create the shader vector.
  SIZE_T shaderCount = m_shaders.size();
  Vector<SPtr<Shader>> shaders;
  shaders.resize(shaderCount);

  // iterate through the unordered map and store the shader.
  uint32 i = 0;
  for (const auto& shaderIt : m_shaders) {
    shaders[i] = shaderIt.second;
    ++i;
  }

  return shaders;
}

Vector<String>
ShaderManager::getShaderNames(const bool _getEntry, const bool _getModel)
{
  // create the name vector.
  SIZE_T shaderCount = m_shaders.size();
  Vector<String> names;
  names.resize(shaderCount);

  // iterate through all shaders in the manager.
  uint32 i = 0;
  for (const auto& shaderIt : m_shaders) {
    // get the shader name.
    SPtr<Shader> shader = shaderIt.second;
    String shaderName = shader->getShaderDirectory().getFileName();
    // get the shader entry point if its set to be retrieved.
    if (_getEntry) {
      shaderName += " - " + String(shader->getEntryPoint());
    }
    // get the shader model if its set to be retrieved.
    if (_getModel) {
      shaderName += " - " + String(shader->getShaderModel());
    }

    names[i] = shaderName;
    ++i;
  }
  return names;
}

PK_CORE_EXPORT ShaderManager&
g_ShaderManager()
{
  return ShaderManager::instance();
}
}
