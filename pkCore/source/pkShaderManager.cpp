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
#include "pkShaderResource.h"
#include "pkGraphicsAPI.h"
#include "pkBaseResource.h"
#include "pkAssetResourceManager.h"
#include "pkLogger.h"

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
  ShaderKey BRDFShader("shaders/pkLightShader.hlsl", "PS", "ps_5_0");
  ShaderKey cBRDFShader("shaders/pkCLightShader.hlsl", "CSMain", "cs_5_0");
  ShaderKey skybox("shaders/pkSkyboxShader.hlsl", "PS", "ps_5_0");
  ShaderKey ssao("shaders/pkPSAOshader.hlsl", "PS", "ps_5_0");
  ShaderKey blur("shaders/pkBlur.hlsl", "PS", "ps_5_0");
  ShaderKey luminance("shaders/pkLuminanceQuad.hlsl", "PS", "ps_5_0");
  ShaderKey toneMap("shaders/pkToneMap.hlsl", "PS", "ps_5_0");
  ShaderKey cubeMap("shaders/pkCubeMapShader.hlsl", "PS", "ps_5_0");
  ShaderKey transparency("shaders/pkPSTransparency.hlsl", "PS", "ps_5_0");

  // initialize all shaders and create the resources.
  createShaderResource(baseVertex, PK_SHADER_TYPE::kVertex);
  createShaderResource(basePixel, PK_SHADER_TYPE::kPixel);
  createShaderResource(positions, PK_SHADER_TYPE::kPixel);
  createShaderResource(quad, PK_SHADER_TYPE::kVertex);
  createShaderResource(BRDFShader, PK_SHADER_TYPE::kPixel);
  createShaderResource(cBRDFShader, PK_SHADER_TYPE::kCompute);
  createShaderResource(skybox, PK_SHADER_TYPE::kPixel);
  createShaderResource(ssao, PK_SHADER_TYPE::kPixel);
  createShaderResource(blur, PK_SHADER_TYPE::kPixel);
  createShaderResource(luminance, PK_SHADER_TYPE::kPixel);
  createShaderResource(toneMap, PK_SHADER_TYPE::kPixel);
  createShaderResource(cubeMap, PK_SHADER_TYPE::kPixel);
  createShaderResource(transparency, PK_SHADER_TYPE::kPixel);
}

void
ShaderManager::createShaderResource(const ShaderKey& _shaderData, const PK_SHADER_TYPE::E _type)
{
  GraphicsAPI& api = g_GraphicAPI();
  // create the shader base.
  ShaderKey key(_shaderData.shaderPath, _shaderData._szEntryPoint, _shaderData._szShaderModel);
  SPtr<Shader> tempShader = api.internalCreateShader(_type);
  tempShader->setData(key);
  tempShader->compileFromFile();
  // create the shader based on its type.
  switch (_type) {
  case PK_SHADER_TYPE::kVertex: {
    tempShader = api.createVShader(tempShader);
    break;
  }
  case PK_SHADER_TYPE::kPixel: {
    tempShader = api.createPShader(tempShader);
    break;
  }
  case PK_SHADER_TYPE::kCompute: {
    tempShader = api.createCShader(tempShader);
    break;
  }
  case PK_SHADER_TYPE::kGeometry: {
    tempShader = api.createGShader(tempShader);
    break;
  }
  default: {
    break;
  }
  }

  // create the resource from the shader.
  SPtr<BaseResource> res = g_ShaderCodec().createResourceFromShader(tempShader);

  key.shaderPath = res->m_resourcePath;
  insertShader(res->m_id, tempShader);
}

void
ShaderManager::insertShader(const UUID& _id, const SPtr<Shader>& _pShader)
{
  // do the same with m_keyShaders.
  if (m_shaders.contains(_id)) {
    m_shaders.find(_id)->second = _pShader;
  }
  else {
    m_shaders.insert({ _id, _pShader });
  }
}

void
ShaderManager::insertShader(const ShaderKey& _key, const SPtr<Shader>& _pShader)
{
  const String searchString = Path(_key.shaderPath).getFileNameWithoutExtension() + 
                                   _key._szEntryPoint +
                                   _key._szShaderModel;
  const UUID searchID = UUID::generateRandomUUIDFromString(searchString);

  insertShader(searchID, _pShader);
}

void
ShaderManager::createShaders()
{
  AssetResourceManager& assetMan = g_AssetResourceManager();
  GraphicsAPI& api = g_GraphicAPI();
  ShaderManager& sm = g_ShaderManager();

  UMap<UUID, SPtr<BaseResource>>& resources = assetMan.getAllResources();
  // iterate through all resources.
  for (auto& resource : resources) {
    WPtr<BaseResource> res = resource.second;
    // if the resource is a shader resource, create the shader and compile it.
    if (RESOURCE_TYPE::kShader == res.lock()->getType()) {
      SPtr<ShaderResource> shaderRes = reinterpret_pointer_cast<ShaderResource>(res.lock());
      shaderRes->load();
      // attempt to get the shader.
      const ShaderKey key(shaderRes->m_shaderDirectory,
                          shaderRes->m_sEntryPoint,
                          shaderRes->m_sModel);
      SPtr<Shader> shader = sm.getShader(key);
      // if the shader is not found, create it and compile it.
      if (!shader) {
        shader->compileFromResource(shaderRes);
        switch (shaderRes->m_type) {
        case PK_SHADER_TYPE::kVertex: {
          shader = api.createVShader(shader);
          break;
        }
        case PK_SHADER_TYPE::kPixel: {
          shader = api.createPShader(shader);
          break;
        }
        case PK_SHADER_TYPE::kCompute: {
          shader = api.createCShader(shader);
          break;
        }
        case PK_SHADER_TYPE::kGeometry: {
          shader = api.createGShader(shader);
          break;
        }
        default: {
          break;
        }
        }
        insertShader(res.lock()->m_id, shader);
      }
      shader->setID(shaderRes->m_id);
    }
  }
}

SPtr<Shader>
ShaderManager::getShader(const UUID& _id)
{
  if (!m_shaders.contains(_id)) {
    const String msg = "Shader with the ID: " + _id.toString() + " not found.";
    LOG_WARNING(msg, __FILE__, __LINE__);
    return nullptr;
  }
  return m_shaders.find(_id)->second;
}

SPtr<Shader>
ShaderManager::getShader(const ShaderKey& _key)
{
  // generate a UUID based on the shader key data.
  const String searchString = Path(_key.shaderPath).getFileNameWithoutExtension() + 
                                   _key._szEntryPoint +
                                   _key._szShaderModel;
  const UUID searchID = UUID::generateRandomUUIDFromString(searchString);
  
  return getShader(searchID);
}

Vector<SPtr<Shader>>
ShaderManager::getShaders()
{
  // create the shader vector.
  const SIZE_T shaderCount = m_shaders.size();
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
    WPtr<Shader> shader = shaderIt.second;
    String shaderName = shader.lock()->getShaderDirectory().getFileName();
    // get the shader entry point if its set to be retrieved.
    if (_getEntry) {
      shaderName += " - " + String(shader.lock()->getEntryPoint());
    }
    // get the shader model if its set to be retrieved.
    if (_getModel) {
      shaderName += " - " + String(shader.lock()->getShaderModel());
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
