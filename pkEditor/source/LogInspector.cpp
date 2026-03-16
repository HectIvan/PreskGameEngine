/*****************************************************************************/
/**
 * @file    ActorInspector.cpp
 * @author  Héctor Iván Muñoz Ceballos
 * @date    12/03/2026
 * @brief   Inspector for the Logger.
 *
 * @bug     No known bugs.
 */
 /*****************************************************************************/

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "LogInspector.h"
#include "pkLogger.h"
#include "pkUInterface.h"
#include "pkPrerequisitesCore.h"

using pkEngineSDK::g_Logger;
using pkEngineSDK::g_uInterface;
using pkEngineSDK::Logger;
using pkEngineSDK::LogMSG;
using pkEngineSDK::LOG_MSG_TYPE::E;
using pkEngineSDK::LOG_MSG_TYPE::kError;
using pkEngineSDK::LOG_MSG_TYPE::kWarning;
using pkEngineSDK::LOG_MSG_TYPE::kLog;
using pkEngineSDK::String;
using pkEngineSDK::uint32;
using pkEngineSDK::UInterface;
using pkEngineSDK::Vector;

void
showLogType(bool& _active, uint32 _type);

void
LogInspector::init(bool& _showErrors, bool& _showWarnings, bool& _showActions)
{
  UInterface& im = g_uInterface();
  Logger& log = g_Logger();

  im.createCheckBox("Errors", _showErrors);
  im.sameLine();
  im.createCheckBox("Warnings", _showWarnings);
  im.sameLine();
  im.createCheckBox("Logs", _showActions);
  im.sameLine();
  if (im.createButton("Clear")) {
    if (_showErrors) {
      log.clearLogsOfType(kError);
    }
    if (_showWarnings) {
      log.clearLogsOfType(kWarning);
    }
  }
  showLogType(_showErrors, kError);
  showLogType(_showWarnings, kWarning);
  showLogType(_showActions, kLog);
}

void
showLogType(bool& _active, uint32 _type)
{
  UInterface& im = g_uInterface();
  Logger& log = g_Logger();
  if (_active) {
    Vector<LogMSG> messages = g_Logger().getMessageLogOfType(static_cast<E>(_type));
    const uint32 messageCount = static_cast<uint32>(messages.size());
    for (uint32 i = 0; i < messageCount; ++i) {
      const String msg = log.getStringFromLog(messages[i]);
      im.createText(msg.c_str());
    }
  }
}