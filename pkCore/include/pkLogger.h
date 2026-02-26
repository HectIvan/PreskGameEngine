/*****************************************************************************/
/**
 * @file    pkLogger.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    03/03/2025
 * @brief   Logger used for printing messages in the console.
 *
 * @bug    No known bugs.
 */
 /*****************************************************************************/
#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkModule.h"
#include "pkPrerequisitesCore.h"

namespace pkEngineSDK
{

class Matrix4;

class Quaternion;

class Vector2;

class Vector3;

class Vector4;

#define LOG_REGISTER(_msg, _file, _line) g_Logger().registerMessage(_msg,                     \
                                                                    _file,                    \
                                                                    _line);

#define LOG_WARNING(_msg, _file, _line) g_Logger().registerMessage(_msg,                      \
                                                                   _file,                     \
                                                                   _line,                     \
                                                                   LOG_MSG_TYPE::kWarning);

#define LOG_ERROR(_msg, _file, _line) g_Logger().registerMessage(_msg,                        \
                                                                 _file,                       \
                                                                 _line,                       \
                                                                 LOG_MSG_TYPE::kError);

#define LOG_FATAL(_msg, _file, _line) g_Logger().registerMessage(_msg,                        \
                                                                 _file,                       \
                                                                 _line,                       \
                                                                 LOG_MSG_TYPE::kFatal);

#define THROW_ERROR(_msg) g_Logger().throwError(_msg);

#define LOG_PRINT(_msg) g_Logger().print(_msg);

namespace LOG_MSG_TYPE
{
  enum E {
    kNone = 0, // smth idk
    kWarning, // warning log registry.
    kError, // error log registry.
    kFatal, // fatal error log registry.
    kLog, // normal log registry.
  };
}

struct LogMSG
{
  LogMSG(const String& _message,
         const ANSICHAR* _file,
         const uint32& _line,
         const String& _time,
         const LOG_MSG_TYPE::E& _type) :
    message(_message),
    file(_file),
    line(_line),
    time(_time),
    type(_type)
  {}
  String message;
  const ANSICHAR* file;
  uint32 line;
  String time;
  LOG_MSG_TYPE::E type;
};

class PK_CORE_EXPORT Logger : public Module<Logger>
{
 public:
  Logger() = default;
  virtual ~Logger() = default;

  /**
   * @brief Get the error message from a HRESULT.
   * @param _hr HRESULT to pass.
   * @return The error message.
   */
  const String
  getMessageError(const int32& _hr) const;

  /**
   * @brief Print a string.
   * @param _text Text to print to the console.
   */
  static void
  print(const String& _text);
  
  /**
   * @brief Print a number.
   * @param _num Number to print to the console.
   */
  static void
  print(const float& _num);
  
  /**
   * @brief Print a vector.
   * @param _vec Vector to print to the console.
   */
  static void
  print(const Vector2& _vec);
  
  /**
   * @brief Print a vector.
   * @param _vec Vector to print to the console.
   */
  static void
  print(const Vector3& _vec);
  
  /**
   * @brief Print a vector.
   * @param _vec Vector to print to the console.
   */
  static void
  print(const Vector4& _vec);

  /**
   * @brief Print a quaternion.
   * @param _vec Quaternion to print to the console.
   */
  static void
  print(const Quaternion& _quat);
  
  /**
   * @brief Print a matrix4.
   * @param _matrix Matrix to print to the console.
   */
  static void
  print(const Matrix4& _matrix);

  /**
   * @brief Convert a vector3 to String.
   * @param _vec Vector to read.
   * @return A String.
   */
  static const String
  toString(const Vector3& _vec);

  /**
   * @brief Throw a handled crash.
   * @param _errorMSG Message to send the catch.
   */
  void
  throwError(const String& _errorMSG) const;

  /**
   * @brief Register a log message into the logger.
   * @param _msg Message.
   * @param _file File where the message is registered.
   * @param _type Message type.
   */
  void
  registerMessage(const String& _msg,
                  const ANSICHAR* _file,
                  const uint32 _line,
                  const LOG_MSG_TYPE::E _type = LOG_MSG_TYPE::kLog);

  /**
   * @brief Print a log into the console.
   * @param _msg Message to print.
   */
  void
  printMessage(const LogMSG& _msg) const;

  /**
   * @brief Get the full message from a log as a string.
   * @param _msg Message to interpret.
   */
  const String
  getStringFromLog(const LogMSG& _msg) const;

  /**
   * @brief Get the message logs.
   * @return All the messages.
   */
  const Vector<LogMSG>
  getMessageLog() const { return m_messages; }

  // to do: maybe swap this to a template or not.
  /**
   * @brief Get the messages from a specific type.
   * @param _type Message type.
   * @return All messages of a type.
   */
  Vector<LogMSG>
  getMessageLogOfType(const LOG_MSG_TYPE::E& _type);

  /**
   * @brief Print the messages from a specific type.
   * @param _type Message type.
   */
  void
  printMessageLogOfType(const LOG_MSG_TYPE::E& _type);

  /**
   * @brief Create log files for the messages registered.
   */
  void
  createLogFiles() const;

 private:
  Vector<LogMSG> m_messages;
};
PK_CORE_EXPORT Logger&
g_Logger();
}
