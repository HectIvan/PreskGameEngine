/*****************************************************************************/
/**
 * @file    pkLogger.cpp
 * @author  Héctor Iván Muñoz Ceballos
 * @date    03/03/2025
 * @brief   Logger used for printing messages in the console.
 *
 * @bug    No known bugs.
 */
 /*****************************************************************************/

 /*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkLogger.h"
#include "pkMatrix4.h"
#include "pkPrerequisitesCore.h"
#include "pkQuaternion.h"
#include "pkTimeManager.h"
#include "pkVector2.h"

#if PK_PLATFORM == PK_PLATFORM_WIN32
#include <Windows.h>

namespace pkEngineSDK
{

const String
Logger::getMessageError(const int32& _hr) const
{
  // error log
  ANSICHAR* errorMsg = nullptr;

  // convert from int32 to HRESULT
  HRESULT hr = static_cast<HRESULT>(_hr);

  // get the error message
  FormatMessageA(
    FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
    nullptr,
    hr,
    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
    (LPSTR)&errorMsg,
    0,
    nullptr);

  // if no message is found, set the string to unknown error
  const String message = (errorMsg) ? errorMsg : "Unknown Error.";
  LocalFree(errorMsg);

  return message;
}
}
#endif

namespace pkEngineSDK
{

void
Logger::print(const Vector2& _vec)
{
  cout << _vec.x << " " << _vec.y << '\n' << '\n';
}

void
Logger::print(const Vector3& _vec)
{
  cout << _vec.x << " " << _vec.y << " " << _vec.z << '\n' << '\n';
}

void
Logger::print(const Vector4& _vec)
{
  cout << _vec.x << " " << _vec.y << " " << _vec.z << " " << _vec.w << '\n' << '\n';
}

void
Logger::print(const Quaternion& _quat)
{
  cout << _quat.w << " " << _quat.x << " " << _quat.y << " " << _quat.z << '\n' << '\n';
}

void
Logger::print(const Matrix4& _matrix)
{
  // First row.
  cout << _matrix.matrix[0][0] << " "
       << _matrix.matrix[0][1] << " "
       << _matrix.matrix[0][2] << " "
       << _matrix.matrix[0][3] << " " << '\n';
  // Second row.
  cout << _matrix.matrix[1][0] << " "
       << _matrix.matrix[1][1] << " "
       << _matrix.matrix[1][2] << " "
       << _matrix.matrix[1][3] << " " << '\n';
  // Third row.
  cout << _matrix.matrix[2][0] << " "
       << _matrix.matrix[2][1] << " "
       << _matrix.matrix[2][2] << " "
       << _matrix.matrix[2][3] << " " << '\n';
  // Fourth row.
  cout << _matrix.matrix[3][0] << " "
       << _matrix.matrix[3][1] << " "
       << _matrix.matrix[3][2] << " "
       << _matrix.matrix[3][3] << " " << '\n' << '\n';
}

const String
Logger::toString(const Vector2& _vec)
{
  return String(to_string(_vec.x) + " " + to_string(_vec.y));
}

const String
Logger::toString(const Vector3& _vec)
{
   return String(to_string(_vec.x) + " " + to_string(_vec.y) + " " + to_string(_vec.z));
}

void
Logger::throwError(const String& _errorMSG) const
{
  throw Runtime_error(_errorMSG);
}

void
Logger::registerMessage(const String& _msg,
                        const ANSICHAR* _file,
                        const uint32 _line,
                        const LOG_MSG_TYPE::E _type)
{
  // register the message.
  const String time = g_TimeManager().getCurrentTime();
  const LogMSG message(_msg, _file, _line, time, _type);
  m_messages.emplace_back(message);
}

void
Logger::printMessage(const LogMSG& _msg) const
{
  const String message = getStringFromLog(_msg);
  print(message);
}

const String
Logger::getStringFromLog(const LogMSG& _msg) const
{
  return String("[" + 
                _msg.time +
                "] " +
                "[" + String(_msg.file) +
                " : " +
                to_string(_msg.line) +
                "]" +
                " " +
                _msg.message);
}

Vector<LogMSG>
Logger::getMessageLogOfType(const LOG_MSG_TYPE::E& _type)
{
  Vector<LogMSG> messages;
  const uint32 logCount = static_cast<uint32>(m_messages.size());
  for (uint32 i = 0; i < logCount; ++i) {
    const LogMSG& msg = m_messages[i];
    if (msg.type == _type) {
      messages.emplace_back(msg);
    }
  }
  return messages;
}

void
Logger::printMessageLogOfType(const LOG_MSG_TYPE::E& _type)
{
  const Vector<LogMSG> messages = getMessageLogOfType(_type);
  const uint32 logCount = static_cast<uint32>(messages.size());
  for (uint32 i = 0; i < logCount; ++i) {
    printMessage(messages[i]);
  }
}

void
Logger::clearLogsOfType(const LOG_MSG_TYPE::E& _type)
{
  const uint32 logCount = static_cast<uint32>(m_messages.size());
  for (uint32 i = 0; i < logCount; ++i) {
    if (m_messages[i].type == _type) {
      m_messages.erase(m_messages.begin() + i);
    }
  }
}

void
Logger::createLogFiles() const
{
  const Vector<LogMSG> logs = getMessageLog();

  // create log file
  ofstream logFile("log/Log.txt", ios::out | ios::trunc);
  const uint32 logCount = static_cast<uint32>(logs.size());
  for (uint32 i = 0; i < logCount; ++i) {
    const LogMSG& msg = logs[i];
    String message = "";
    if (msg.type == LOG_MSG_TYPE::kLog) {
      message += "[LOG] ";
    }
    if (msg.type == LOG_MSG_TYPE::kError) {
      message += "[ERROR] ";
    }
    if (msg.type == LOG_MSG_TYPE::kWarning) {
      message += "[WARNING] ";
    }
    if (msg.type == LOG_MSG_TYPE::kFatal) {
      message += "[FATAL] ";
    }

    message += getStringFromLog(msg);
    logFile << message << '\n';
  }
  logFile.close();
}

PK_CORE_EXPORT Logger&
g_Logger()
{
  return Logger::instance();
}
}