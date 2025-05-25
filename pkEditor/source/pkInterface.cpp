/*****************************************************************************/
/**
 * @file    pkInterface.cpp
 * @author  Héctor Iván Muñoz Ceballos
 * @date    01/04/2025
 * @brief   Interface for the Editor app.
 *
 * @bug    Interface class is fundamentally wrong.
 */
 /*****************************************************************************/

 /*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkInterface.h"

namespace pkEngineSDK
{

void
Interface::init(const WindowHandle& _hWnd)
{
  initAPI();

  ImGui::StyleColorsDark();
  //ImGui::StyleColorsClassic();
}

PK_CORE_EXPORT Interface&
g_interface()
{
  return Interface::instance();
}
}
