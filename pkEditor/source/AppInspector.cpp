/*****************************************************************************/
/**
 * @file    AppInspector.cpp
 * @author  Héctor Iván Muñoz Ceballos
 * @date    11/03/2026
 * @brief   Inspector for the Editor App.
 *
 * @bug     No known bugs.
 */
 /*****************************************************************************/

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "AppInspector.h"
#include "pkUInterface.h"
#include "pkTimeManager.h"

using pkEngineSDK::g_TimeManager;
using pkEngineSDK::g_uInterface;
using pkEngineSDK::UInterface;
using pkEngineSDK::uint32;
using pkEngineSDK::String;
using pkEngineSDK::to_string;

void
AppInspector::init(float& _camSpeed, float& _sensX, float& _sensY, bool& _vSync)
{ 
  UInterface& im = g_uInterface();
  // get framerate
  float f_fps = 1.0f / g_TimeManager().m_deltaTime;
  uint32 fps = static_cast<uint32>(f_fps);
  String fpsStr = "FPS: " + to_string(fps);
  String camSpeed = "Camera Speed: " + to_string(static_cast<uint32>(_camSpeed));

  // FPS parameters
  static const uint32 fpsListSize = 100;
  static float fpsHistory[fpsListSize] = {};
  static uint32 fpsOffset = 0;

  // Record the current FPS
  fpsHistory[fpsOffset] = f_fps;
  fpsOffset = (fpsOffset + 1) % fpsListSize;
  // --- Camera window --- //
  im.createText("vSync");
  im.sameLine();
  im.createCheckBox("##vSync", _vSync);
  // fps graph
  im.createText(fpsStr.c_str());
  im.sameLine();
  im.plotLines("##LinesFPS", fpsHistory, fpsListSize, fpsOffset);
  // editor app settings
  if (im.beginTable("Editor App")) {
    im.tableJumpRow();

    im.tableJumpRow();
    im.createText("Camera Speed");
    im.tableNextColumn();
    im.createDrag("##CamSpeed", _camSpeed);
    im.tableJumpRow();
    im.createText("X Sensitivity");
    im.tableNextColumn();
    im.createDrag("##XSens", _sensX, 0.1f);
    im.tableJumpRow();
    im.createText("Y Sensitivity");
    im.tableNextColumn();
    im.createDrag("##YSens", _sensY, 0.1f);
    im.tableJumpRow();
    im.endTable();
  }
}