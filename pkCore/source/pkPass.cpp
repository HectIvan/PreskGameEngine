/*****************************************************************************/
/**
 * @file    pkPass.cpp
 * @author  Héctor  Iván Muñoz Ceballos
 * @date    07/02/2025
 * @brief   Source file of the pass
 *
 * @bug    No known bugs.
 */
 /*****************************************************************************/

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkGraphicsAPI.h"
#include "pkPass.h"
// #include "pkShader.h"

#include <iostream>

namespace pkEngineSDK
{

void
Pass::create()
{
  m_pVShader = make_shared<Shader>();
  m_pPShader = make_shared<Shader>();
  m_pInputLayout = make_shared<InputLayout>();
  m_pSamplerState = make_shared<SamplerState>();
}

void
Pass::createInputLayout()
{
  m_pInputLayout = g_GraphicAPI().createInputLayoutFromVShader(m_pVShader);
}

void
Pass::createSamplerState(uint32 _mode, uint32 _filter)
{
  m_pSamplerState = g_GraphicAPI().createSamplerState(_mode, _filter);
}

void
Pass::setVSData(WString _fileName, const char* _entryPoint, const char* _model)
{
  m_VShaderDirectory = _fileName;
  m_VSEntryPoint = _entryPoint;
  m_VSModel = _model;
}

void
Pass::setPSData(WString _fileName, const char* _entryPoint, const char* _model)
{
  m_PShaderDirectory = _fileName;
  m_PSEntryPoint = _entryPoint;
  m_PSModel = _model;
}

void
Pass::createShaders()
{
  // create the shaders
  g_GraphicAPI().createVShader(m_pVShader);
  g_GraphicAPI().createPShader(m_pPShader);
}

void
Pass::compileShaders()
{
  compileVShader();
  compilePShader();
}

void
Pass::compileVShader()
{
  g_GraphicAPI().compileShaderFromFile(m_VShaderDirectory,
                                       m_VSEntryPoint,
                                       m_VSModel,
                                       m_pVShader);
}

void
Pass::compilePShader()
{
  g_GraphicAPI().compileShaderFromFile(m_PShaderDirectory,
                                       m_PSEntryPoint,
                                       m_PSModel,
                                       m_pPShader);
}

void
Pass::addToCBuffers(SPtr<ConstantBuffer> _pCBuffer)
{
  m_cBuffers.push_back(_pCBuffer);
}
}
