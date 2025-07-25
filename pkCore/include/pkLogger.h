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
#include "pkMatrix4.h"
#include "pkPrerequisitesCore.h"
#include "pkVector2.h"
#include "pkModule.h"

namespace pkEngineSDK
{

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
  String
  getMessageError(int32 _hr);

  /**
   * @brief Print a string.
   * @param _text Text to print to the console.
   */
  static void
  print(String _text);
  
  /**
   * @brief Print a number.
   * @param _num Number to print to the console.
   */
  static void
  print(float _num);
  
  /**
   * @brief Print a vector.
   * @param _vec Vector to print to the console.
   */
  static void
  print(Vector2 _vec);
  
  /**
   * @brief Print a vector.
   * @param _vec Vector to print to the console.
   */
  static void
  print(Vector3 _vec);
  
  /**
   * @brief Print a vector.
   * @param _vec Vector to print to the console.
   */
  static void
  print(Vector4 _vec);
  
  /**
   * @brief Print a matrix4.
   * @param _matrix Matrix to print to the console.
   */
  static void
  print(Matrix4 _matrix);

  /**
   * @brief Convert a vector3 to String.
   * @param _vec Vector to read.
   * @return A String.
   */
  static String
  toString(const Vector3 _vec);
};
PK_CORE_EXPORT Logger&
g_Logger();
}
