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
#include "pkShaderManager.h"

namespace pkEngineSDK
{

void
ShaderManager::insertShader(const ShaderKey& _key, SPtr<Shader> _pShader)
{
  m_shaders.insert({ _key, _pShader });
}

SPtr<Shader>
ShaderManager::getShader(const ShaderKey& _key)
{
  return m_shaders.find(_key)->second;
}

PK_CORE_EXPORT ShaderManager&
g_ShaderManager()
{
  return ShaderManager::instance();
}
}
