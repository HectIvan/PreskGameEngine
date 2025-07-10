/*************************************************************************/
/**
 * @file    pkMatrix3.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    10/07/2025
 * @brief   Matrix3 File for the game engine.
 *
 * @bug    no knon bugs.
 */
 /*****************************************************************************/
#pragma once


/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkVector4.h"
#include "pkVector3.h"

namespace pkEngineSDK {

class PK_UTILITY_EXPORT Matrix3
{
 public:
  Matrix3() = default;
  Matrix3(const float _val);
  Matrix3(Vector3 R0, Vector3 R1, Vector3 R2);
  Matrix3(const Matrix3& _matrix);
  Matrix3(float m00, float m01, float m02,
          float m10, float m11, float m12,
          float m20, float m21, float m22);

  FORCEINLINE Matrix3&
  operator=(const Matrix3& other)
  {
    for (int i = 0; i < 3; ++i)
    {
      for (int j = 0; j < 3; ++j)
      {
        matrix[i][j] = other.matrix[i][j];
      }
    }
    return *this;
  }

  FORCEINLINE Matrix3
  operator+(const Matrix3& other) const
  {
    Matrix3 result;
    for (int i = 0; i < 3; ++i)
    {
      for (int j = 0; j < 3; ++j)
      {
        result.matrix[i][j] = matrix[i][j] + other.matrix[i][j];
      }
    }
    return result;
  }

  FORCEINLINE Matrix3
  operator+(const float& other) const
  {
    Matrix3 result;
    for (int i = 0; i < 3; ++i)
    {
      for (int j = 0; j < 3; ++j)
      {
        result.matrix[i][j] = matrix[i][j] + other;
      }
    }
    return result;
  }

  FORCEINLINE Matrix3
  operator+=(const Matrix3& other)
  {
    for (int i = 0; i < 3; ++i)
    {
      for (int j = 0; j < 3; ++j)
      {
        matrix[i][j] += other.matrix[i][j];
      }
    }
    return *this;
  }

  FORCEINLINE Matrix3
  operator-(const float& other)
  {
    Matrix3 result;
    for (int i = 0; i < 3; ++i)
    {
      for (int j = 0; j < 3; ++j)
      {
        result.matrix[i][j] = matrix[i][j] - other;
      }
    }
    return result;
  }

  FORCEINLINE Matrix3
  operator-(float& other)
  {
    Matrix3 result;
    for (int i = 0; i < 3; ++i)
    {
      for (int j = 0; j < 3; ++j)
      {
        result.matrix[i][j] = matrix[i][j] - other;
      }
    }
    return result;
  }

  FORCEINLINE Matrix3
  operator-(Matrix3& other)
  {
    Matrix3 result;
    for (int i = 0; i < 3; ++i)
    {
      for (int j = 0; j < 3; ++j)
      {
        result.matrix[i][j] = matrix[i][j] - other.matrix[i][j];
      }
    }
    return result;
  }

  FORCEINLINE Matrix3
  operator*(const float& other) const
  {
    Matrix3 result;
    for (int i = 0; i < 3; ++i)
    {
      for (int j = 0; j < 3; ++j)
      {
        result.matrix[i][j] = matrix[i][j] * other;
      }
    }
    return result;
  }

  Vector3
  operator*(const Vector3& other)
  {
    matrix[0][0] *= other.x;
    matrix[1][0] *= other.x;
    matrix[2][0] *= other.x;

    matrix[0][1] *= other.y;
    matrix[1][1] *= other.y;
    matrix[2][1] *= other.y;

    matrix[0][2] *= other.z;
    matrix[1][2] *= other.z;
    matrix[2][2] *= other.z;

    return Vector3(matrix[0][0] + matrix[1][0] + matrix[2][0],
                   matrix[0][1] + matrix[1][1] + matrix[1][1],
                   matrix[0][2] + matrix[1][2] + matrix[2][2]);
  }

  FORCEINLINE Matrix3
  operator*(const float& other)
  {
    Matrix3 result;
    for (int i = 0; i < 3; ++i)
    {
      for (int j = 0; j < 3; ++j)
      {
        result.matrix[i][j] = matrix[i][j] * other;
      }
    }
    return result;
  }

  FORCEINLINE Matrix3
  operator*(float& other)
  {
    Matrix3 result;
    for (int i = 0; i < 3; ++i)
    {
      for (int j = 0; j < 3; ++j)
      {
        result.matrix[i][j] = matrix[i][j] * other;
      }
    }
    return result;
  }

  FORCEINLINE Vector4
  operator*(const Vector4& other)
  {
    matrix[0][0] *= other.x;
    matrix[1][0] *= other.x;
    matrix[2][0] *= other.x;
    
    matrix[0][1] *= other.y;
    matrix[1][1] *= other.y;
    matrix[2][1] *= other.y;
    
    matrix[0][2] *= other.z;
    matrix[1][2] *= other.z;
    matrix[2][2] *= other.z;
    
    return Vector4(matrix[0][0] + matrix[1][0] + matrix[2][0],
                   matrix[0][1] + matrix[1][1] + matrix[1][1],
                   matrix[0][2] + matrix[1][2] + matrix[2][2],
                   other.w);
  }

  /**
   * @brief Multiply a matrix with another.
   * @param Other The other matrix.
   * @return The new matrix.
   */
  Matrix3
  operator*(const Matrix3& other) const
  {
    Matrix3 result;
    
    float x = matrix[0][0];
    float y = matrix[0][1];
    float z = matrix[0][2];
    
    // first row
    result.matrix[0][0] = (other.matrix[0][0] * x) + (other.matrix[1][0] * y) + (other.matrix[2][0] * z);
    result.matrix[0][1] = (other.matrix[0][1] * x) + (other.matrix[1][1] * y) + (other.matrix[2][1] * z);
    result.matrix[0][2] = (other.matrix[0][2] * x) + (other.matrix[1][2] * y) + (other.matrix[2][2] * z);
    // second row
    x = matrix[1][0];
    y = matrix[1][1];
    z = matrix[1][2];
    result.matrix[1][0] = (other.matrix[0][0] * x) + (other.matrix[1][0] * y) + (other.matrix[2][0] * z);
    result.matrix[1][1] = (other.matrix[0][1] * x) + (other.matrix[1][1] * y) + (other.matrix[2][1] * z);
    result.matrix[1][2] = (other.matrix[0][2] * x) + (other.matrix[1][2] * y) + (other.matrix[2][2] * z);
    // third row
    x = matrix[2][0];
    y = matrix[2][1];
    z = matrix[2][2];
    result.matrix[2][0] = (other.matrix[0][0] * x) + (other.matrix[1][0] * y) + (other.matrix[2][0] * z);
    result.matrix[2][1] = (other.matrix[0][1] * x) + (other.matrix[1][1] * y) + (other.matrix[2][1] * z);
    result.matrix[2][2] = (other.matrix[0][2] * x) + (other.matrix[1][2] * y) + (other.matrix[2][2] * z);
    return result;
  }

  
  /**
   * @brief Multiply this matrix by another matrix.
   * @brief other The other matrix.
   * @return This matrix multiplied.
   */
  Matrix3
  operator*=(const Matrix3& other)
  {
    float x = matrix[0][0];
    float y = matrix[0][1];
    float z = matrix[0][2];
    // first row
    matrix[0][0] = (other.matrix[0][0] * x) + (other.matrix[1][0] * y) + (other.matrix[2][0] * z);
    matrix[0][1] = (other.matrix[0][1] * x) + (other.matrix[1][1] * y) + (other.matrix[2][1] * z);
    matrix[0][2] = (other.matrix[0][2] * x) + (other.matrix[1][2] * y) + (other.matrix[2][2] * z);
    
    // second row
    x = matrix[1][0];
    y = matrix[1][1];
    z = matrix[1][2];
    matrix[1][0] = (other.matrix[0][0] * x) + (other.matrix[1][0] * y) + (other.matrix[2][0] * z);
    matrix[1][1] = (other.matrix[0][1] * x) + (other.matrix[1][1] * y) + (other.matrix[2][1] * z);
    matrix[1][2] = (other.matrix[0][2] * x) + (other.matrix[1][2] * y) + (other.matrix[2][2] * z);
    
    // third row
    x = matrix[2][0];
    y = matrix[2][1];
    z = matrix[2][2];
    matrix[2][0] = (other.matrix[0][0] * x) + (other.matrix[1][0] * y) + (other.matrix[2][0] * z);
    matrix[2][1] = (other.matrix[0][1] * x) + (other.matrix[1][1] * y) + (other.matrix[2][1] * z);
    matrix[2][2] = (other.matrix[0][2] * x) + (other.matrix[1][2] * y) + (other.matrix[2][2] * z);
  
    return *this;
  }

  /**
   * @brief The transposed of this matrix.
   * @return This matrix transposed.
   */
  Matrix3
  getTransposed()
  {
    Matrix3 result;
    for (int i = 0; i < 3; ++i)
    {
      for (int j = 0; j < 3; ++j)
      {
        result.matrix[i][j] = matrix[j][i];
      }
    }
    return result;
  }

  float matrix[3][3];
  static const Matrix3 IDENTITY;
  static const Matrix3 ZERO;
};
}
