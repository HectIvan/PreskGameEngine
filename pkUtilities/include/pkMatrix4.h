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

class Quaternion;

class PK_UTILITY_EXPORT Matrix4
{
 public:
  Matrix4() = default;
  Matrix4(const float& val);
  Matrix4(const Matrix4& _matrix);
  Matrix4(const Vector4& R0, const Vector4& R1, const Vector4& R2, const Vector4& R3);
  Matrix4(const float& m00, const float& m01, const float& m02, const float& m03,
          const float& m10, const float& m11, const float& m12, const float& m13,
          const float& m20, const float& m21, const float& m22, const float& m23,
          const float& m30, const float& m31, const float& m32, const float& m33);
  // --------------------------------------------------------------//
  // Matrix4 = Matrix4
  // --------------------------------------------------------------//
  PKFORCEINLINE Matrix4&
  operator=(const Matrix4& other)
  {
    for (int i = 0; i < 4; ++i) {
      for (int j = 0; j < 4; ++j) {
        matrix[i][j] = other.matrix[i][j];
      }
    }
    return *this;
  }
  // --------------------------------------------------------------//
  // Matrix4 + Matrix4
  // --------------------------------------------------------------//
  PKFORCEINLINE Matrix4
  operator+(const Matrix4& other) const
  {
    Matrix4 result;
    for (int i = 0; i < 4; ++i) {
      for (int j = 0; j < 4; ++j) {
        result.matrix[i][j] = matrix[i][j] + other.matrix[i][j];
      }
    }
    return result;
  }
  // --------------------------------------------------------------//
  // Matrix4 + float
  // --------------------------------------------------------------//
  PKFORCEINLINE Matrix4
  operator+(const float& other) const
  {
    Matrix4 result;
    for (int i = 0; i < 4; ++i) {
      for (int j = 0; j < 4; ++j) {
        result.matrix[i][j] = matrix[i][j] + other;
      }
    }
    return result;
  }

  // --------------------------------------------------------------//
  // Matrix4 += Matrix4
  // --------------------------------------------------------------//
  PKFORCEINLINE Matrix4
  operator+=(const Matrix4& other)
  {
    for (int i = 0; i < 4; ++i) {
      for (int j = 0; j < 4; ++j) {
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
  PKFORCEINLINE Matrix4
  operator-(const float& other)
  {
    Matrix4 result;
    for (int i = 0; i < 4; ++i) {
      for (int j = 0; j < 4; ++j) {
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
  PKFORCEINLINE Matrix4
  operator-(float& other)
  {
    Matrix4 result;
    for (int i = 0; i < 4; ++i) {
      for (int j = 0; j < 4; ++j) {
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
  PKFORCEINLINE Matrix4
  operator-(Matrix4& other)
  {
    Matrix4 result;
    for (int i = 0; i < 4; ++i) {
      for (int j = 0; j < 4; ++j) {
        result.matrix[i][j] = matrix[i][j] - other.matrix[i][j];
      }
    }
    return result;
  }

  // --------------------------------------------------------------//
  // Matrix4 * float
  // --------------------------------------------------------------//
  PKFORCEINLINE Matrix4
  operator*(const float& other) const
  {
    Matrix4 result;
    for (int i = 0; i < 4; ++i) {
      for (int j = 0; j < 4; ++j) {
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
  PKFORCEINLINE Matrix4
  operator*(const float& other)
  {
    Matrix4 result;
    for (int i = 0; i < 4; ++i) {
      for (int j = 0; j < 4; ++j) {
        result.matrix[i][j] = matrix[i][j] * other;
      }
    }
    return result;
  }

  /**
   * @brief Multiply the Matrix by a float.
   * @param other The float to use.
   */
  PKFORCEINLINE Matrix4
  operator*(float& other)
  {
    Matrix4 result;
    for (int i = 0; i < 4; ++i) {
      for (int j = 0; j < 4; ++j) {
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
  PKFORCEINLINE Vector4
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
  * @brief Get the rotation matrix on 3 axis.
  * @return The rotation Quaternion.
  */
  const Quaternion
  getLocalRotation() const;

  /**
   * @brief get the forward vector from a view matrix.
   * @return the forward vector.
   */
  const Vector3
  getForwardVector() const;

  /**
   * @brief get the up vector from a view matrix.
   * @return the up vector.
   */
  const Vector3
  getUpVector() const;

  /**
   * @brief get the right vector from a view matrix.
   * @return the right vector.
   */
  const Vector3
  getRightVector() const;

  /**
   * @brief Get the position of the view matrix.
   * @return The position of the view.
   */
  const Vector3
  getViewPosition() const;

  /**
   * @brief Get the inverse matrix.
   * @return The inverse matrix.
   */
  const Matrix4
  inverse() const;

  /**
   * @brief Multiply a matrix with another.
   * @param Other The other matrix.
   * @return The new matrix.
   */
  Matrix4
  operator*(const Matrix4& other) const
  {
    Matrix4 result = *this;
    for (uint32 row = 0; row < 4; ++row) {
      for (uint32 col = 0; col < 4; ++col) {
        result.matrix[row][col] = result.matrix[row][0] * other.matrix[0][col] +
                                  result.matrix[row][1] * other.matrix[1][col] +
                                  result.matrix[row][2] * other.matrix[2][col] +
                                  result.matrix[row][3] * other.matrix[3][col];
      }
    }
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
    *this = *this * other;
    return *this;
  }

  /**
   * @brief The transposed of this matrix.
   * @return This matrix transposed.
   */
  const Matrix4
  getTransposed() const;
  
  /**
   * @brief Scale a matrix to the desired scales.
   * @param _scaleX The scale on the X coordinate.
   * @param _scaleY The scale on the Y coordinate.
   * @param _scaleZ The scale on the Z coordinate.
   * @return This matrix transposed.
   */
  static const Matrix4
  matrixScaling(const float& _scaleX, const float& _scaleY, const float& _scaleZ) ;
  
  /**
   * @brief The translation matrix that is created from
   * the Vector3 given to it.
   * @param _position vector position.
   * @return The translation matrix at the position given to it.
   */
  static const Matrix4
  translation(const Vector3& _position);

  /**
   * @brief The translation matrix that is created from
   * the 3 coordinates given to it.
   * @param offsetX Coordinate at X.
   * @param offsetY Coordinate at Y.
   * @param offsetZ Coordinate at Z.
   * @return The translation matrix at the position given to it.
   */
  static const Matrix4
  translation(const float& _offsetX, const float& _offsetY, const float& _offsetZ);

  /**
   * @brief Set the translation
   * @param _pos vector position.
   */
  void
  setTranslation(const Vector3& _pos);

  /**
   * @brief Set the translation
   * @param _x Coordinate at X.
   * @param _y Coordinate at Y.
   * @param _z Coordinate at Z.
   */
  void
  setTranslation(const float& _x, const float& _y, const float& _z);

  /**
   * @brief Get the current matrix translation.
   * @return The matrix translation.
   */
  const Matrix4
  getTranslation() const;

  /**
   * @brief Get the current Vector3 translation.
   * @return The Vector translation.
   */
  const Vector3
  getTranslation3() const;
  
  /**
   * @brief The translation matrix that is created from
   * the Vector3 given to it.
   * @param _position Vector position.
   * @return The translation matrix at the position given to it.
   */
  static const Matrix4
  scale(const Vector3& _scale);

  /**
   * @brief Get the current scale matrix.
   * @return The scale matrix.
   */
  const Matrix4
  getScale();

  /**
   * @brief Get the current scale vector.
   * @return The scale vector.
   */
  const Vector3
  getScale3() const;

  /**
   * @brief Set the scale of the matrix.
   * @param _scale New scale.
   */
  void
  setScale(const Vector3& _scale);

  /**
   * @brief Set the scale of the matrix.
   * @param _scale New scale.
   */
  void
  setScale(Matrix4& _scale);

  /**
   * @brief Set the scale of the matrix.
   * @param _val Value of the scale.
   */
  void
  setScale(const float& _val);

  /**
   * @brief Set the scale of the matrix.
   * @param _x Scale in x.
   * @param _y Scale in y.
   * @param _z Scale in z.
   */
  void
  setScale(const float& _x, const float& _y, const float& _z);

  /**
   * Get a matrix rotation from an axis and an angle
   * 
   */
  static Matrix4
  MatrixRotationAxis(const Vector3& _axis, const float& _angle);

  /**
   * @brief Get the rotation matrix.
   * @return The rotation matrix.
   */
  const Matrix4
  getRotation() const;

  /**
   * @brief Get the rotation matrix without a scale.
   * @param _scale Scale to extract from the rotation matrix.
   * @return The rotation matrix.
   */
  const Matrix4
  getRotationNoScale(const Vector3& _scale) const;
  
  /**
   * @brief The rotation matrix that is created from
   * the Vector3 given to it.
   * @param _angleX Angle at X.
   * @param _angleY Angle at Y.
   * @param _angleZ Angle at Z.
   * @return The rotation matrix at the rotations given to it.
   */
  static const Matrix4
  rotation(const float& _angleX, const float& _angleY, const float& _angleZ);

  /**
   * @brief The rotation matrix that is created from
   * the Vector3 given to it.
   * @param _rot Vector rotations.
   * @return The rotation matrix at the rotations given to it.
   */
  static const Matrix4
  rotation(const Vector3& _rot);

  /**
   * @brief The rotation matrix that is created from a rotation quaternion.
   * @param _quat Rotation quaternion.
   * @return The rotation matrix at the rotations given to it.
   */
  static const Matrix4
  rotation(const Quaternion& _quat);
  
  /**
   * @brief The rotation matrix at the z coordinate.
   * @param angle angle to rotate.
   * @return The rotation matrix at z with the rotation given to it.
   */
  static Matrix4
  rotationZ(const float& _angle);
  
  /**
   * @brief The rotation matrix at the y coordinate.
   * @param angle angle to rotate.
   * @return The rotation matrix at y with the rotation given to it.
   */
  static Matrix4
  rotationY(const float& _angle);
  
  /**
   * @brief The rotation matrix at the x coordinate.
   * @param angle angle to rotate.
   * @return The rotation matrix at x with the rotation given to it.
   */
  static Matrix4
  rotationX(const float& _angle);

  /**
   * @brief Set the matrix rotation.
   * @param _rotation New matrix rotation.
   */
  void
  setRotation(const Matrix4& _rotation);

  /**
   * @brief Look at matrix.
   * @param _eyePos Value of the eye vector.
   * @param _atPos Value of the at vector.
   * @param _upPos Direction of the up vector.
   */
  static const Matrix4
  lookAtLH(const Vector4& _eyePos, const Vector4& _atPos, const Vector3& _upDir);

  /**
   * @brief Matrix look to.
   * @param _eyePos Position of the eye.
   * @param _eyeDir Direction where its looking at.
   * @param _upDir Direction of the up vector.
   */
  static const Matrix4
  lookToLH(const Vector4& _eyePos, const Vector4& _eyeDir, Vector4& _upDir);

  /**
   * @brief Get the perspective matrix.
   * @param _halfFOV Half value of the field of view.
   * @param _width Width of the camera view.
   * @param _height Height of the camera view.
   * @param _nearZ Near Z distance.
   * @param _farZ Far Z distance.
   * @return The return matrix.
   */
  static const Matrix4
  perspectiveFOVLH(const float& _halfFOV,
                   const float& _width,
                   const float& _height,
                   const float& _nearZ,
                   const float& _farZ);

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
  static const Matrix4
  orthographicFOVLH(const float& _left,
                    const float& _right,
                    const float& _top,
                    const float& _bottom,
                    const float& _nearZ,
                    const float& _farZ);

  float matrix[4][4];
  static const Matrix4 IDENTITY;
  static const Matrix4 ZERO;
};
}