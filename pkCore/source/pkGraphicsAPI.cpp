/*****************************************************************************/
/**
 * @file    pkGraphicsAPI.cpp
 * @author  Héctor  Iván Muñoz Ceballos
 * @date    19/11/2024
 * @brief   API file using DirectX 11 for the Presk Game Engine.
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

namespace pkEngineSDK
{

PK_CORE_EXPORT GraphicsAPI&
g_GraphicAPI()
{
  return GraphicsAPI::instance();
}
}
