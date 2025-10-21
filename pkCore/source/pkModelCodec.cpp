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
#include "pkModelCodec.h"

namespace pkEngineSDK
{

PK_CORE_EXPORT ModelCodec&
g_ModelCodec()
{
  return ModelCodec::instance();
}
}
