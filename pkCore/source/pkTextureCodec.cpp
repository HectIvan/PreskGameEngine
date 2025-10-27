/*****************************************************************************/
/**
 * @file    pkStbiTextureCodec.cpp
 * @author  Héctor  Iván Muñoz Ceballos
 * @date    24/10/2025
 * @brief   Codec for creating pkt Texures.
 *
 * @bug    No known bugs.
 */
 /*****************************************************************************/

 /*********************************************/
/**
 * Includes
 */
/*********************************************/
#include "pkTextureCodec.h"

namespace pkEngineSDK
{

PK_CORE_EXPORT TextureCodec&
g_TextureCodec()
{
  return TextureCodec::instance();
}
}