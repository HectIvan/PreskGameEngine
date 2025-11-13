/*****************************************************************************/
/**
 * @file    pkShaderCodec.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    07/11/2025
 * @brief   Codec for compiled shaders.
 *
 * @bug    No known bugs.
 *
 * @changes Created the file.
 */
 /*****************************************************************************/

/*********************************************/
/**
 * Includes
 */
 /*********************************************/
#include "pkShaderCodec.h"

namespace pkEngineSDK
{

PK_CORE_EXPORT ShaderCodec&
g_ShaderCodec()
{
  return ShaderCodec::instance();
}
}
