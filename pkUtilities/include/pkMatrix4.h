/************************************************************************/
/**
* @pkMatrix4 pkMatrix4.h
* @Hector Ivan Muñoz Ceballos
* @date 06/09/2024
* @Matrix4 file for the Presk Game Engine.
*
* This file contains the Matrix4 class of the engine
* 
* The Matrix4 is row major, meaning that the first value
* when searching in the matrix is the Y coordinate, while
* the second one is the X coordinate
*
* @bug No bug known.
*/
/************************************************************************/
#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
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
  
  // --------------------------------------------------------------//
  // Matrix4 * Vector4
  // --------------------------------------------------------------//
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
  // --------------------------------------------------------------//
  // Matrix4 * Matrix4
  // --------------------------------------------------------------//
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
  * Matrix4 *= Matrix4
  **/
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
  * transpose matrix.
  *
  * This function returns a copy of the transposed of this matrix.
  *
  * @return
  * This matrix transposed.
  **/
  Matrix4
  getTransposed();
  
  /**
  * scale matrix.
  *
  * This function scales a matrix to the desired scales.
  * 
  * @param _scaleX
  * The scale on the X coordinate.
  * 
  * @param _scaleY
  * The scale on the Y coordinate.
  * 
  * @param _scaleZ
  * The scale on the Z coordinate.
  *
  * @return
  * This matrix transposed.
  **/
  static Matrix4
  matrixScaling(float& _scaleX, float& _scaleY, float& _scaleZ);
  
  /**
  * Translation matrix.
  *
  * This function returns the translation matrix that is created from
  * the 3 coordinates given to it.
  * 
  * @param offsetX
  * coordinate at X
  * 
  * @param offsetY
  * coordinate at Y
  * 
  * @param offsetZ
  * coordinate at Z
  *
  * @return
  * The translation matrix at the position given to it.
  **/
  static Matrix4
  translation(float& _offsetX, float& _offsetY, float& _offsetZ);
  
  /**
  * Translation matrix.
  *
  * This function returns the translation matrix that is created from
  * the Vector3 given to it.
  *
  * @param _position
  * vector position
  *
  * @return
  * The translation matrix at the position given to it.
  **/
  static Matrix4
  translation(Vector3& _position);

  void
  setTranslation(Vector3 _pos);

  /**
  * Get the current matrix translation.
  * 
  * @return
  * The matrix translation.
  **/
  Matrix4
  getTranslation();

  /**
  * Get the current translation in a Vector3
  * 
  * @return
  * The translation Vector.
  **/
  Vector3
  getTranslationVector();
  
  /**
  * Translation matrix.
  *
  * This function returns the translation matrix that is created from
  * the Vector3 given to it.
  *
  * @param _position
  * vector position
  *
  * @return
  * The translation matrix at the position given to it.
  **/
  static Matrix4
  scale(Vector3& _scale);

  /**
  * Get the current scale matrix.
  * 
  * @return
  * The scale matrix.
  **/
  Matrix4
  getScale();

  /**
  * Set the scale of the matrix.
  * 
  * @param _scale
  * New scale.
  **/
  void
  setScale(Matrix4 _scale);

  /**
  * Set the scale of the matrix.
  * 
  * @param _scale
  * New scale.
  **/
  void
  setScale(Vector3 _scale);
  
  /**
  * rotation matrix.
  *
  * This function returns the rotation matrix that is created from
  * the Vector3 given to it.
  *
  * @param _angleX
  * angle at X
  * 
  * @param _angleY
  * angle at Y
  *
  * @param _angleZ
  * angle at Z
  *
  * @return
  * The rotation matrix at the rotations given to it.
  **/
  static Matrix4
  rotation(float& _angleX,
           float& _angleY,
           float& _angleZ);

  /**
  * rotation matrix.
  *
  * This function returns the rotation matrix that is created from
  * the Vector3 given to it.
  *
  * @param _rot
  * vector rotations
  *
  * @return
  * The rotation matrix at the rotations given to it.
  **/
  static Matrix4
  rotation(Vector3& _rot);
  
  /**
  * rotation matrix at Z.
  *
  * This function returns the rotation matrix at the z coordinate
  * created from the angle given to it.
  *
  * @param angle
  * angle to rotate
  *
  * @return
  * The rotation matrix at z with the rotation given to it.
  **/
  FORCEINLINE static Matrix4
  rotationZ(float& _angle);
  
  /**
  * rotation matrix at Y.
  *
  * This function returns the rotation matrix at the y coordinate
  * created from the angle given to it.
  *
  * @param angle
  * angle to rotate
  *
  * @return
  * The rotation matrix at y with the rotation given to it.
  **/
  FORCEINLINE static Matrix4
  rotationY(float& _angle);
  
  /**
  * rotation matrix at X.
  *
  * This function returns the rotation matrix at the x coordinate
  * created from the angle given to it.
  *
  * @param angle
  * angle to rotate
  *
  * @return
  * The rotation matrix at x with the rotation given to it.
  **/
  static Matrix4
  rotationX(float& _angle);

  /**
  * Set the matrix rotation.
  * 
  * @param _rotation
  * New matrix rotation.
  **/
  void
  setRotation(Matrix4 _rotation);

  /**
  * Look at matrix.
  * 
  * @param _eyePos
  * Value of the eye vector.
  * 
  * @param _atPos
  * Value of the at vector.
  * 
  * @param _upPos
  * Direction of the up vector.
  **/
  static Matrix4
  lookAtLH(Vector4 _eyePos, Vector4 _atPos, Vector4 _upDir);

  /**
  * Matrix look to.
  * 
  * @param _eyePos
  * Position of the eye.
  * 
  * @param _eyeDir
  * Direction where its looking at.
  * 
  * @param _upDir
  * Direction of the up vector.
  **/
  static Matrix4
  lookToLH(Vector4 _eyePos, Vector4 _eyeDir, Vector4 _upDir);

  /**
  * Get the perspective matrix.
  * 
  * @param _halfFOV
  * Half value of the field of view.
  * 
  * @param _width
  * Width of the camera view.
  * 
  * @param _height
  * Height of the camera view.
  * 
  * @param _nearZ
  * Near Z distance.
  * 
  * @param _farZ
  * Far Z distance.
  **/
  static Matrix4
  perspectiveFOVLH(float _halfFOV,
                   float _width,
                   float _height,
                   float _nearZ,
                   float _farZ);

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