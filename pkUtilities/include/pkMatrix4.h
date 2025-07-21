/*****************************************************************************/
/**
 * @file    pkMatrix4.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    06/09/2024
 * @brief   This file contains the Matrix4 class of the engine.
 *          The Matrix4 is row major, meaning that the first value
 *          when searching in the matrix is the Y coordinate, while
 *          the second one is the X coordinate
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
#include "pkMatrix3.h"
#include "pkVector4.h"
#include "pkVector3.h"

namespace pkEngineSDK {

class PK_UTILITY_EXPORT Matrix4
{
 public:
  Matrix4() = default;
  Matrix4(const float& val);
  Matrix4(const Matrix4& _matrix);
  Matrix4(Vector4 R0, Vector4 R1, Vector4 R2, Vector4 R3);
  Matrix4(float m00, float m01, float m02, float m03,
          float m10, float m11, float m12, float m13,
          float m20, float m21, float m22, float m23,
          float m30, float m31, float m32, float m33);
  // --------------------------------------------------------------//
  // Matrix4 = Matrix4
  // --------------------------------------------------------------//
  FORCEINLINE Matrix4&
  operator=(const Matrix4& other)
  {
    for (int i = 0; i < 4; ++i)
    {
      for (int j = 0; j < 4; ++j)
      {
        matrix[i][j] = other.matrix[i][j];
      }
    }
    return *this;
  }
  // --------------------------------------------------------------//
  // Matrix4 + Matrix4
  // --------------------------------------------------------------//
  FORCEINLINE Matrix4
  operator+(const Matrix4& other) const
  {
    Matrix4 result;
    for (int i = 0; i < 4; ++i)
    {
      for (int j = 0; j < 4; ++j)
      {
        result.matrix[i][j] = matrix[i][j] + other.matrix[i][j];
      }
  	}
  	return result;
  }
  // --------------------------------------------------------------//
  // Matrix4 + float
  // --------------------------------------------------------------//
  FORCEINLINE Matrix4
  operator+(const float& other) const
  {
    Matrix4 result;
    for (int i = 0; i < 4; ++i)
    {
      for (int j = 0; j < 4; ++j)
      {
        result.matrix[i][j] = matrix[i][j] + other;
      }
    }
    return result;
  }

  // --------------------------------------------------------------//
  // Matrix4 += Matrix4
  // --------------------------------------------------------------//
  FORCEINLINE Matrix4
  operator+=(const Matrix4& other)
  {
    for (int i = 0; i < 4; ++i)
    {
      for (int j = 0; j < 4; ++j)
      {
        matrix[i][j] += other.matrix[i][j];
      }
    }
    return *this;
  }
  /**
   * @brief Subtract a float to the matrix4.
   * @param other The float to use.
   * @return The subtracted matrix.
   */
  FORCEINLINE Matrix4
  operator-(const float& other)
  {
    Matrix4 result;
    for (int i = 0; i < 4; ++i)
    {
      for (int j = 0; j < 4; ++j)
      {
        result.matrix[i][j] = matrix[i][j] - other;
      }
    }
    return result;
  }

  /**
   * @brief Subtract a float to the matrix4.
   * @param other The float to use.
   * @return The subtracted matrix.
   */
  FORCEINLINE Matrix4
  operator-(float& other)
  {
    Matrix4 result;
    for (int i = 0; i < 4; ++i)
    {
      for (int j = 0; j < 4; ++j)
      {
        result.matrix[i][j] = matrix[i][j] - other;
      }
    }
    return result;
  }

  /**
   * @brief Subtract a matrix4 to the matrix4.
   * @param other The matrix to use.
   * @return The subtracted matrix.
   */
  FORCEINLINE Matrix4
  operator-(Matrix4& other)
  {
    Matrix4 result;
    for (int i = 0; i < 4; ++i)
    {
      for (int j = 0; j < 4; ++j)
      {
        result.matrix[i][j] = matrix[i][j] - other.matrix[i][j];
      }
    }
    return result;
  }

  // --------------------------------------------------------------//
  // Matrix4 * float
  // --------------------------------------------------------------//
  FORCEINLINE Matrix4
  operator*(const float& other) const
  {
    Matrix4 result;
    for (int i = 0; i < 4; ++i)
    {
      for (int j = 0; j < 4; ++j)
      {
        result.matrix[i][j] = matrix[i][j] * other;
      }
    }
    return result;
  }

  /**
   * @brief Multiply a Vector3 by a Matrix
   * @param other The vector to multiply
   * @return
   */
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

  /**
   * @brief Multiply the Matrix by a float.
   * @param other The float to use.
   */
  FORCEINLINE Matrix4
  operator*(const float& other)
  {
    Matrix4 result;
    for (int i = 0; i < 4; ++i)
    {
      for (int j = 0; j < 4; ++j)
      {
        result.matrix[i][j] = matrix[i][j] * other;
      }
    }
    return result;
  }

  /**
   * @brief Multiply the Matrix by a float.
   * @param other The float to use.
   */
  FORCEINLINE Matrix4
  operator*(float& other)
  {
    Matrix4 result;
    for (int i = 0; i < 4; ++i)
    {
      for (int j = 0; j < 4; ++j)
      {
        result.matrix[i][j] = matrix[i][j] * other;
      }
    }
    return result;
  }

  /**
   * @brief Multiply a Vector4 by a Matrix
   * @param other The vector to multiply
   * @return
   */
  FORCEINLINE Vector4
  operator*(const Vector4& other)
  {
    float x00 = matrix[0][0] * other.x;
    float x10 = matrix[1][0] * other.x;
    float x20 = matrix[2][0] * other.x;

    float y01 = matrix[0][1] * other.y;
    float y11 = matrix[1][1] * other.y;
    float y21 = matrix[2][1] * other.y;

    float z02 = matrix[0][2] * other.z;
    float z12 = matrix[1][2] * other.z;
    float z22 = matrix[2][2] * other.z;
    
    return Vector4(x00 + y01 + z02,
                   x10 + y11 + z12,
                   x20 + y21 + z22,
                   other.w);
  }

  Matrix3
  getMatrix3(int32 _x = 0, int32 _y = 0)
  {
    Matrix3 newMat = Matrix3::IDENTITY;

    newMat.matrix[0 + _y][0 + _x] = matrix[0 + _y][0 + _x];
    newMat.matrix[1 + _y][0 + _x] = matrix[1 + _y][0 + _x];
    newMat.matrix[2 + _y][0 + _x] = matrix[2 + _y][0 + _x];

    newMat.matrix[0 + _y][1 + _x] = matrix[0 + _y][1 + _x];
    newMat.matrix[1 + _y][1 + _x] = matrix[1 + _y][1 + _x];
    newMat.matrix[2 + _y][1 + _x] = matrix[2 + _y][1 + _x];

    newMat.matrix[0 + _y][2 + _x] = matrix[0 + _y][2 + _x];
    newMat.matrix[1 + _y][2 + _x] = matrix[1 + _y][2 + _x];
    newMat.matrix[2 + _y][2 + _x] = matrix[2 + _y][2 + _x];

    return newMat;
  }

  /**
   * @brief get the forward vector from a view matrix.
   * @return the forward vector.
   */
  Vector3
  getForwardVector();

  /**
   * @brief get the up vector from a view matrix.
   * @return the up vector.
   */
  Vector3
  getUpVector();

  /**
   * @brief get the right vector from a view matrix.
   * @return the right vector.
   */
  Vector3
  getRightVector();

  /**
   * @brief Get the position of the view matrix.
   * @return The position of the view.
   */
  Vector3
  getViewPosition();

  /**
   * @brief Get the inverse matrix.
   * @return The inverse matrix.
   */
  Matrix4
  inverse();

  /**
   * @brief Multiply a matrix with another.
   * @param Other The other matrix.
   * @return The new matrix.
   */
  Matrix4
  operator*(const Matrix4& other) const
  {
    Matrix4 result;
    
    float x = matrix[0][0];
    float y = matrix[0][1];
    float z = matrix[0][2];
    float w = matrix[0][3];
    
    // first row
    result.matrix[0][0] = (other.matrix[0][0] * x) + (other.matrix[1][0] * y) + (other.matrix[2][0] * z) + (other.matrix[3][0] * w);
    result.matrix[0][1] = (other.matrix[0][1] * x) + (other.matrix[1][1] * y) + (other.matrix[2][1] * z) + (other.matrix[3][1] * w);
    result.matrix[0][2] = (other.matrix[0][2] * x) + (other.matrix[1][2] * y) + (other.matrix[2][2] * z) + (other.matrix[3][2] * w);
    result.matrix[0][3] = (other.matrix[0][3] * x) + (other.matrix[1][3] * y) + (other.matrix[2][3] * z) + (other.matrix[3][3] * w);
    // second row
    x = matrix[1][0];
    y = matrix[1][1];
    z = matrix[1][2];
    w = matrix[1][3];
    result.matrix[1][0] = (other.matrix[0][0] * x) + (other.matrix[1][0] * y) + (other.matrix[2][0] * z) + (other.matrix[3][0] * w);
    result.matrix[1][1] = (other.matrix[0][1] * x) + (other.matrix[1][1] * y) + (other.matrix[2][1] * z) + (other.matrix[3][1] * w);
    result.matrix[1][2] = (other.matrix[0][2] * x) + (other.matrix[1][2] * y) + (other.matrix[2][2] * z) + (other.matrix[3][2] * w);
    result.matrix[1][3] = (other.matrix[0][3] * x) + (other.matrix[1][3] * y) + (other.matrix[2][3] * z) + (other.matrix[3][3] * w);
    // third row
    x = matrix[2][0];
    y = matrix[2][1];
    z = matrix[2][2];
    w = matrix[2][3];
    result.matrix[2][0] = (other.matrix[0][0] * x) + (other.matrix[1][0] * y) + (other.matrix[2][0] * z) + (other.matrix[3][0] * w);
    result.matrix[2][1] = (other.matrix[0][1] * x) + (other.matrix[1][1] * y) + (other.matrix[2][1] * z) + (other.matrix[3][1] * w);
    result.matrix[2][2] = (other.matrix[0][2] * x) + (other.matrix[1][2] * y) + (other.matrix[2][2] * z) + (other.matrix[3][2] * w);
    result.matrix[2][3] = (other.matrix[0][3] * x) + (other.matrix[1][3] * y) + (other.matrix[2][3] * z) + (other.matrix[3][3] * w);
    // fourth row
    x = matrix[3][0];
    y = matrix[3][1];
    z = matrix[3][2];
    w = matrix[3][3];
    result.matrix[3][0] = (other.matrix[0][0] * x) + (other.matrix[1][0] * y) + (other.matrix[2][0] * z) + (other.matrix[3][0] * w);
    result.matrix[3][1] = (other.matrix[0][1] * x) + (other.matrix[1][1] * y) + (other.matrix[2][1] * z) + (other.matrix[3][1] * w);
    result.matrix[3][2] = (other.matrix[0][2] * x) + (other.matrix[1][2] * y) + (other.matrix[2][2] * z) + (other.matrix[3][2] * w);
    result.matrix[3][3] = (other.matrix[0][3] * x) + (other.matrix[1][3] * y) + (other.matrix[2][3] * z) + (other.matrix[3][3] * w);
    return result;
  }

  /**
   * @brief Multiply this matrix by another matrix.
   * @brief other The other matrix.
   * @return This matrix multiplied.
   */
  Matrix4
  operator*=(const Matrix4& other)
  {
    float x = matrix[0][0];
    float y = matrix[0][1];
    float z = matrix[0][2];
    float w = matrix[0][3];
    // first row
    matrix[0][0] = (other.matrix[0][0] * x) + (other.matrix[1][0] * y) + (other.matrix[2][0] * z) + (other.matrix[3][0] * w);
    matrix[0][1] = (other.matrix[0][1] * x) + (other.matrix[1][1] * y) + (other.matrix[2][1] * z) + (other.matrix[3][1] * w);
    matrix[0][2] = (other.matrix[0][2] * x) + (other.matrix[1][2] * y) + (other.matrix[2][2] * z) + (other.matrix[3][2] * w);
    matrix[0][3] = (other.matrix[0][3] * x) + (other.matrix[1][3] * y) + (other.matrix[2][3] * z) + (other.matrix[3][3] * w);
    
    // second row
    x = matrix[1][0];
    y = matrix[1][1];
    z = matrix[1][2];
    w = matrix[1][3];
    matrix[1][0] = (other.matrix[0][0] * x) + (other.matrix[1][0] * y) + (other.matrix[2][0] * z) + (other.matrix[3][0] * w);
    matrix[1][1] = (other.matrix[0][1] * x) + (other.matrix[1][1] * y) + (other.matrix[2][1] * z) + (other.matrix[3][1] * w);
    matrix[1][2] = (other.matrix[0][2] * x) + (other.matrix[1][2] * y) + (other.matrix[2][2] * z) + (other.matrix[3][2] * w);
    matrix[1][3] = (other.matrix[0][3] * x) + (other.matrix[1][3] * y) + (other.matrix[2][3] * z) + (other.matrix[3][3] * w);
    
    // third row
    x = matrix[2][0];
    y = matrix[2][1];
    z = matrix[2][2];
    w = matrix[2][3];
    matrix[2][0] = (other.matrix[0][0] * x) + (other.matrix[1][0] * y) + (other.matrix[2][0] * z) + (other.matrix[3][0] * w);
    matrix[2][1] = (other.matrix[0][1] * x) + (other.matrix[1][1] * y) + (other.matrix[2][1] * z) + (other.matrix[3][1] * w);
    matrix[2][2] = (other.matrix[0][2] * x) + (other.matrix[1][2] * y) + (other.matrix[2][2] * z) + (other.matrix[3][2] * w);
    matrix[2][3] = (other.matrix[0][3] * x) + (other.matrix[1][3] * y) + (other.matrix[2][3] * z) + (other.matrix[3][3] * w);
    
    // fourth row
    x = matrix[3][0];
    y = matrix[3][1];
    z = matrix[3][2];
    w = matrix[3][3];
    matrix[3][0] = (other.matrix[0][0] * x) + (other.matrix[1][0] * y) + (other.matrix[2][0] * z) + (other.matrix[3][0] * w);
    matrix[3][1] = (other.matrix[0][1] * x) + (other.matrix[1][1] * y) + (other.matrix[2][1] * z) + (other.matrix[3][1] * w);
    matrix[3][2] = (other.matrix[0][2] * x) + (other.matrix[1][2] * y) + (other.matrix[2][2] * z) + (other.matrix[3][2] * w);
    matrix[3][3] = (other.matrix[0][3] * x) + (other.matrix[1][3] * y) + (other.matrix[2][3] * z) + (other.matrix[3][3] * w);
  
    return *this;
  }

  /**
   * @brief The transposed of this matrix.
   * @return This matrix transposed.
   */
  Matrix4
  getTransposed();
  
  /**
   * @brief Scale a matrix to the desired scales.
   * @param _scaleX The scale on the X coordinate.
   * @param _scaleY The scale on the Y coordinate.
   * @param _scaleZ The scale on the Z coordinate.
   * @return This matrix transposed.
   */
  static Matrix4
  matrixScaling(float& _scaleX, float& _scaleY, float& _scaleZ);
  
  /**
   * @brief The translation matrix that is created from
   * the 3 coordinates given to it.
   * @param offsetX Coordinate at X.
   * @param offsetY Coordinate at Y.
   * @param offsetZ Coordinate at Z.
   * @return The translation matrix at the position given to it.
   */
  static Matrix4
  translation(float& _offsetX, float& _offsetY, float& _offsetZ);

  /**
   * @brief The translation matrix that is created from
   * the Vector3 given to it.
   * @param _position vector position.
   * @return The translation matrix at the position given to it.
   */
  static Matrix4
  translation(Vector3 _position);

  /**
   * @brief Set the translation
   * @param _pos vector position.
   */
  void
  setTranslation(Vector3 _pos);

  /**
   * @brief Set the translation
   * @param _pos vector position.
   */
  void
  setTranslation(float _x, float _y, float _z);

  /**
   * @brief Get the current matrix translation.
   * @return The matrix translation.
   */
  Matrix4
  getTranslation();

  /**
   * @brief Get the current Vector3 translation.
   * @return The Vector translation.
   */
  Vector3
  getTranslation3();
  
  /**
   * @brief The translation matrix that is created from
   * the Vector3 given to it.
   * @param _position Vector position.
   * @return The translation matrix at the position given to it.
   */
  static Matrix4
  scale(Vector3& _scale);

  /**
   * @brief Get the current scale matrix.
   * @return The scale matrix.
   */
  Matrix4
  getScale();

  /**
   * @brief Get the current scale vector.
   * @return The scale vector.
   */
  Vector3
  getScale3();

  /**
   * @brief Set the scale of the matrix.
   * @param _scale New scale.
   */
  void
  setScale(Matrix4 _scale);

  /**
   * @brief Set the scale of the matrix.
   * @param _scale New scale.
   */
  void
  setScale(Vector3 _scale);

  /**
   * @brief Set the scale of the matrix.
   * @param _x Scale in x.
   * @param _y Scale in y.
   * @param _z Scale in z.
   */
  void
  setScale(float _x, float _y, float _z);

  /**
   * @brief Set the scale of the matrix.
   * @param _val Value of the scale.
   */
  void
  setScale(float _val);

  /**
   * Get a matrix rotation from an axis and an angle
   * 
   */
  static Matrix4
  MatrixRotationAxis(Vector3 _axis, float _angle);

  /**
   * @brief Get the rotation matrix.
   * @return The rotation matrix.
   */
  Matrix4
  getRotation();

  /**
   * @brief Get the rotation matrix without a scale.
   * @param _scale Scale to extract from the rotation matrix.
   * @return The rotation matrix.
   */
  Matrix4
  getRotationNoScale(Vector3 _scale);
  
  /**
   * @brief The rotation matrix that is created from
   * the Vector3 given to it.
   * @param _angleX Angle at X.
   * @param _angleY Angle at Y.
   * @param _angleZ Angle at Z.
   * @return The rotation matrix at the rotations given to it.
   */
  static Matrix4
  rotation(float _angleX, float _angleY, float _angleZ);

  /**
   * @brief The rotation matrix that is created from
   * the Vector3 given to it.
   * @param _rot Vector rotations.
   * @return The rotation matrix at the rotations given to it.
   */
  static Matrix4
  rotation(Vector3& _rot);
  
  /**
   * @brief The rotation matrix at the z coordinate.
   * created from the angle given to it.
   * @param angle angle to rotate.
   * @return The rotation matrix at z with the rotation given to it.
   */
  FORCEINLINE static Matrix4
  rotationZ(float& _angle);
  
  /**
   * @brief The rotation matrix at the y coordinate.
   * created from the angle given to it.
   * @param angle angle to rotate.
   * @return The rotation matrix at y with the rotation given to it.
   */
  static Matrix4
  rotationY(float& _angle);
  
  /**
   * @brief The rotation matrix at the x coordinate.
   * created from the angle given to it.
   * @param angle angle to rotate.
   * @return The rotation matrix at x with the rotation given to it.
   */
  static Matrix4
  rotationX(float& _angle);

  /**
   * @brief Set the matrix rotation.
   * @param _rotation New matrix rotation.
   */
  void
  setRotation(Matrix4 _rotation);

  /**
   * @brief Look at matrix.
   * @param _eyePos Value of the eye vector.
   * @param _atPos Value of the at vector.
   * @param _upPos Direction of the up vector.
   */
  static Matrix4
  lookAtLH(Vector4 _eyePos, Vector4 _atPos, Vector3 _upDir);

  /**
   * @brief Matrix look to.
   * @param _eyePos Position of the eye.
   * @param _eyeDir Direction where its looking at.
   * @param _upDir Direction of the up vector.
   */
  static Matrix4
  lookToLH(Vector4 _eyePos, Vector4 _eyeDir, Vector4 _upDir);

  /**
   * @brief Get the perspective matrix.
   * @param _halfFOV Half value of the field of view.
   * @param _width Width of the camera view.
   * @param _height Height of the camera view.
   * @param _nearZ Near Z distance.
   * @param _farZ Far Z distance.
   * @return The return matrix.
   */
  static Matrix4
  perspectiveFOVLH(float _halfFOV,
                   float _width,
                   float _height,
                   float _nearZ,
                   float _farZ);

  /**
   * @brief Get the orthographic matrix.
   * @param _left Most left point of the view.
   * @param _right Most right point of the view.
   * @param _top Most top point of the view.
   * @param _bottom Most bottom point of the view.
   * @param _nearZ Near Z distance.
   * @param _farZ Far Z distance.
   * @return The orthographic matrix.
   */
  static Matrix4
  orthographicFOVLH(float _left,
                    float _right,
                    float _top,
                    float _bottom,
                    float _nearZ,
                    float _farZ);

  float matrix[4][4];
  static const Matrix4 IDENTITY;
  static const Matrix4 ZERO;
};
}