/*****************************************************************************/
/**
 * @file    pkModelCodec.cpp
 * @author  Héctor Iván Muñoz Ceballos
 * @date    08/10/2025
 * @brief   Codec for loading models.
 *
 * @bug    No known bugs.
 */
 /*****************************************************************************/

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkModel.h"
#include "pkModelCodec.h"
#include "pkPath.h"

namespace pkEngineSDK
{

bool
ModelCodec::savePKModel(const SPtr<Model>& _pModel, const Path _path)
{
  return true;
}

PK_CORE_EXPORT ModelCodec&
g_ModelCodec()
{
  return ModelCodec::instance();
}
}
