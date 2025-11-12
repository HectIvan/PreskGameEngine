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
#include "pkPrerequisitesCore.h"
#include "pkLogger.h"
#include "pkTimeManager.h"

#if PK_PLATFORM == PK_PLATFORM_WIN32
#include <Windows.h>

namespace pkEngineSDK
{

void
Logger::init(const bool _printLog, const bool _printWarning, const bool _printError)
{
  m_printLog = _printLog;
  m_printWarnings = _printWarning;
  m_printErrors = _printError;
}

String
Logger::getMessageError(int32 _hr)
{
  // error log
  char* errorMsg = nullptr;

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
  String message = (errorMsg) ? errorMsg : "Unknown Error.";
  LocalFree(errorMsg);

  return message;
}
}
#endif

namespace pkEngineSDK
{

void
Logger::print(String _text)
{
  cout << _text << endl << endl;
}

void
Logger::print(float _num)
{
  cout << _num << endl << endl;
}

void
Logger::print(Vector2 _vec)
{
  cout << _vec.x << " " << _vec.y << endl << endl;
}

void
Logger::print(Vector3 _vec)
{
  cout << _vec.x << " " << _vec.y << " " << _vec.z << endl << endl;
}

void
Logger::print(Vector4 _vec)
{
  cout << _vec.x << " " << _vec.y << " " << _vec.z << " " << _vec.w << endl << endl;
}

void
Logger::print(Matrix4 _matrix)
{
  // First row.
  cout << _matrix.matrix[0][0] << " "
       << _matrix.matrix[0][1] << " "
       << _matrix.matrix[0][2] << " "
       << _matrix.matrix[0][3] << " " << endl;
  // Second row.
  cout << _matrix.matrix[1][0] << " "
       << _matrix.matrix[1][1] << " "
       << _matrix.matrix[1][2] << " "
       << _matrix.matrix[1][3] << " " << endl;
  // Third row.
  cout << _matrix.matrix[2][0] << " "
       << _matrix.matrix[2][1] << " "
       << _matrix.matrix[2][2] << " "
       << _matrix.matrix[2][3] << " " << endl;
  // Fourth row.
  cout << _matrix.matrix[3][0] << " "
       << _matrix.matrix[3][1] << " "
       << _matrix.matrix[3][2] << " "
       << _matrix.matrix[3][3] << " " << endl << endl;
}

String
Logger::toString(const Vector3 _vec)
{
   return String(to_string(_vec.x) + " " + to_string(_vec.y) + " " + to_string(_vec.z));
}

void
Logger::throwError(const String _errorMSG)
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
  String time = g_TimeManager().getCurrentTime();
  LogMSG message(_msg, _file, _line, time, _type);
  m_messages.emplace_back(message);

  // check if the message should be print in the console.
  if (m_printLog && _type == LOG_MSG_TYPE::kLog) {
    printMessage(message);
  }
  if (m_printWarnings && _type == LOG_MSG_TYPE::kWarning) {
    printMessage(message);
  }
  if (m_printErrors && _type == LOG_MSG_TYPE::kError) {
    printMessage(message);
  }
}

void
Logger::printMessage(const LogMSG& _msg)
{
  String message = getStringFromLog(_msg);
  print(message);
}

String
Logger::getStringFromLog(const LogMSG& _msg)
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
Logger::getMessageLogOfType(const LOG_MSG_TYPE::E _type)
{
  Vector<LogMSG> messages;
  for (uint32 i = 0; i < m_messages.size(); ++i) {
    if (m_messages[i].type == _type) {
      messages.emplace_back(m_messages[i]);
    }
  }
  return messages;
}

void
Logger::printMessageLogOfType(const LOG_MSG_TYPE::E _type)
{
  Vector<LogMSG> messages = getMessageLogOfType(_type);
  for (uint32 i = 0; i < messages.size(); ++i) {
    printMessage(messages[i]);
  }
}

void
Logger::createLogFiles()
{
  Vector<LogMSG> logs = getMessageLog();

  // create log file
  ofstream logFile("log/Log.txt", ios::out);
  for (uint32 i = 0; i < logs.size(); ++i) {

    String message = "";
    if (logs[i].type == LOG_MSG_TYPE::kLog) {
      message += "[LOG] ";
    }
    if (logs[i].type == LOG_MSG_TYPE::kError) {
      message += "[ERROR] ";
    }
    if (logs[i].type == LOG_MSG_TYPE::kWarning) {
      message += "[WARNING] ";
    }
    if (logs[i].type == LOG_MSG_TYPE::kFatal) {
      message += "[FATAL] ";
    }

    message += getStringFromLog(logs[i]);
    logFile << message << endl;
  }
  logFile.close();
}

PK_CORE_EXPORT Logger&
g_Logger()
{
  return Logger::instance();
}
}