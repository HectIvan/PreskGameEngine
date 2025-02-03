#pragma once
/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkMatrix4.h"
#include "pkPrerequisitesCore.h"
#include "pkVector2.h"

namespace pkEngineSDK
{

class PK_CORE_EXPORT Debug
{
 public:
  Debug() = default;
  virtual ~Debug() = default;

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
   * @brief Print a matrux4.
   * @param _matrix Matrix to print to the console.
   */
  static void
  print(Matrix4 _matrix);
};
}
