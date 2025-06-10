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

Pass::Pass() {
  m_pVShader = g_GraphicAPI().internalCreateShader();
  m_pPShader = g_GraphicAPI().internalCreateShader();
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
Pass::createVShader(const WString _directory, const char* _entry, const char* _sModel)
{
  m_pVShader->setData(_directory, _entry, _sModel);
  m_pVShader->compile();
  g_GraphicAPI().createVShader(m_pVShader);
}

void
Pass::createPShader(const WString _directory, const char* _entry, const char* _sModel)
{
  m_pPShader->setData(_directory, _entry, _sModel);
  m_pPShader->compile();
  g_GraphicAPI().createPShader(m_pPShader);
}

void
Pass::compileShaders()
{
  m_pVShader->compile();
  m_pPShader->compile();
}

SPtr<ConstantBuffer>
Pass::createCBuffer(SIZE_T _size, const void* _data, uint32 _usage)
{
  // get the api
  GraphicsAPI& api = g_GraphicAPI().instance();
  // create the constant buffer with the parameters given
  SPtr<ConstantBuffer> cb = api.createConstantBuffer(static_cast<uint32>(_size),
                                                     _data,
                                                     _usage);
  // store into the constant buffer vector
  addToCBuffers(cb);
  // return the pointer
  return cb;
}

void
Pass::addToCBuffers(SPtr<ConstantBuffer> _pCBuffer)
{
  m_cBuffers.push_back(_pCBuffer);
}
}
