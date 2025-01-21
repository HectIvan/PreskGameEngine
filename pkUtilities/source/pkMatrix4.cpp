/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkMatrix4.h"

namespace pkEngineSDK {

const Matrix4 Matrix4::IDENTITY = Matrix4(Vector4(1.0f, 0.0f, 0.0f, 0.0f),
                                          Vector4(0.0f, 1.0f, 0.0f, 0.0f),
                                          Vector4(0.0f, 0.0f, 1.0f, 0.0f),
                                          Vector4(0.0f, 0.0f, 0.0f, 1.0f));
const Matrix4 Matrix4::ZERO = Matrix4(0);

Matrix4::Matrix4(const float& val)
{
  for (int i = 0; i < 4; ++i)
  {
    for (int j = 0; j < 4; ++j)
    {
      matrix[i][j] = val;
    }
  }
}

Matrix4::Matrix4(const Matrix4& _matrix)
{
  matrix[0][0] = _matrix.matrix[0][0];
  matrix[0][1] = _matrix.matrix[0][1];
  matrix[0][2] = _matrix.matrix[0][2];
  matrix[0][3] = _matrix.matrix[0][3];

  matrix[1][0] = _matrix.matrix[1][0];
  matrix[1][1] = _matrix.matrix[1][1];
  matrix[1][2] = _matrix.matrix[1][2];
  matrix[1][3] = _matrix.matrix[1][3];

  matrix[2][0] = _matrix.matrix[2][0];
  matrix[2][1] = _matrix.matrix[2][1];
  matrix[2][2] = _matrix.matrix[2][2];
  matrix[2][3] = _matrix.matrix[2][3];

  matrix[3][0] = _matrix.matrix[3][0];
  matrix[3][1] = _matrix.matrix[3][1];
  matrix[3][2] = _matrix.matrix[3][2];
  matrix[3][3] = _matrix.matrix[3][3];
}

Matrix4::Matrix4(Vector4 R0, Vector4 R1, Vector4 R2, Vector4 R3)
{
  matrix[0][0] = R0.x;
  matrix[0][1] = R0.y;
  matrix[0][2] = R0.z;
  matrix[0][3] = R0.w;

  matrix[1][0] = R1.x;
  matrix[1][1] = R1.y;
  matrix[1][2] = R1.z;
  matrix[1][3] = R1.w;

  matrix[2][0] = R2.x;
  matrix[2][1] = R2.y;
  matrix[2][2] = R2.z;
  matrix[2][3] = R2.w;

  matrix[3][0] = R3.x;
  matrix[3][1] = R3.y;
  matrix[3][2] = R3.z;
  matrix[3][3] = R3.w;
}

Matrix4::Matrix4(float m00, float m01, float m02, float m03,
                 float m10, float m11, float m12, float m13,
                 float m20, float m21, float m22, float m23,
                 float m30, float m31, float m32, float m33)
{
  matrix[0][0] = m00; matrix[0][1] = m01; matrix[0][2] = m02; matrix[0][3] = m03;
  matrix[1][0] = m10; matrix[1][1] = m11;	matrix[1][2] = m12; matrix[1][3] = m13;
  matrix[2][0] = m20; matrix[2][1] = m21;	matrix[2][2] = m22; matrix[2][3] = m23;
  matrix[3][0] = m30; matrix[3][1] = m31; matrix[3][2] = m32; matrix[3][3] = m33;
}

Matrix4
Matrix4::getTransposed()
{
  Matrix4 result;
  for (int i = 0; i < 4; ++i)
  {
    for (int j = 0; j < 4; ++j)
    {
      result.matrix[i][j] = matrix[j][i];
    }
  }
  return result;
}

Matrix4
Matrix4::matrixScaling(float& _scaleX, float& _scaleY, float& _scaleZ)
{
  return Matrix4(Vector4(_scaleX, 0.0f, 0.0f, 0.0f),
                 Vector4(0.0f, _scaleY, 0.0f, 0.0f),
                 Vector4(0.0f, 0.0f, _scaleZ, 0.0f),
                 Vector4(0.0f, 0.0f, 0.0f, 1.0f));
}

Matrix4
Matrix4::translation(float& _offsetX, float& _offsetY, float& _offsetZ)
{
  Matrix4 M = Matrix4::IDENTITY;
  M.matrix[0][3] = _offsetX;
  M.matrix[1][3] = _offsetY;  
  M.matrix[2][3] = _offsetZ;
  return M;
} 

Matrix4
Matrix4::translation(Vector3& _position)
{
  Matrix4 M = Matrix4::IDENTITY;
  M.matrix[0][3] = _position.x;
  M.matrix[1][3] = _position.y;
  M.matrix[2][3] = _position.z;
  return M;
}

void
Matrix4::setTransation(Vector3 _pos)
{
  matrix[0][3] = _pos.x;
  matrix[1][3] = _pos.y;
  matrix[2][3] = _pos.z;
}

Matrix4
Matrix4::getTranslation()
{
  Matrix4 M = Matrix4::IDENTITY;
  M.matrix[0][3] = matrix[0][3];
  M.matrix[1][3] = matrix[1][3];
  M.matrix[2][3] = matrix[2][3];
  return M;
}

Vector3
Matrix4::getTranslationVector()
{
  return Vector3(matrix[0][3], matrix[1][3], matrix[2][3]);
}

Matrix4
Matrix4::scale(Vector3& _scale)
{
  Matrix4 M = Matrix4::IDENTITY;
  M.matrix[3][0] = _scale.x;
  M.matrix[3][1] = _scale.y;
  M.matrix[3][2] = _scale.z;
  return M;
}

Matrix4 Matrix4::getScale()
{
  Matrix4 M = Matrix4::IDENTITY;
  M.matrix[0][0] = matrix[3][0];
  M.matrix[1][1] = matrix[3][1];
  M.matrix[2][2] = matrix[3][2];
  return M;
}

void
Matrix4::setScale(Matrix4 _scale)
{
  matrix[3][0] = _scale.matrix[0][0];
  matrix[3][1] = _scale.matrix[1][1];
  matrix[3][2] = _scale.matrix[2][2];
}

Matrix4
Matrix4::rotation(float& _angleX, float& _angleY, float& _angleZ)
{
  Matrix4 M = Matrix4::IDENTITY;
  M = rotationX(_angleX) * rotationY(_angleY) * rotationZ(_angleZ);
  return M;
}

Matrix4
Matrix4::rotation(Vector3& _rot)
{
  return rotation(_rot.x, _rot.y, _rot.z);
}

Matrix4
Matrix4::rotationZ(float& _angle)
{
  Matrix4 M = Matrix4::IDENTITY;

  float fSinAngle = sinf(_angle);
  float fCosAngle = cosf(_angle);

  M.matrix[0][0] = fCosAngle;
  M.matrix[0][1] = fSinAngle;

  M.matrix[1][0] = -fSinAngle;
  M.matrix[1][1] = fCosAngle;
  return M;
}

Matrix4
Matrix4::rotationY(float& _angle)
{
  Matrix4 M = Matrix4::IDENTITY;

  float fSinAngle = sinf(_angle);
  float fCosAngle = cosf(_angle);

  M.matrix[0][0] = fCosAngle;
  M.matrix[0][2] = -fSinAngle;

  M.matrix[2][0] = fSinAngle;
  M.matrix[2][2] = fCosAngle;

  return M;
}

Matrix4
Matrix4::rotationX(float& _angle)
{
  Matrix4 M = IDENTITY;

  float fSinAngle = sinf(_angle);
  float fCosAngle = cosf(_angle);

  M.matrix[1][1] = fCosAngle;
  M.matrix[1][2] = -fSinAngle;

  M.matrix[2][1] = fSinAngle;
  M.matrix[2][2] = fCosAngle;

  return M;
}

void
Matrix4::setRotation(Matrix4 _rotation)
{
  matrix[0][0] = _rotation.matrix[0][0];
  matrix[0][1] = _rotation.matrix[0][1];
  matrix[0][2] = _rotation.matrix[0][2];

  matrix[1][0] = _rotation.matrix[1][0];
  matrix[1][1] = _rotation.matrix[1][1];
  matrix[1][2] = _rotation.matrix[1][2];

  matrix[2][0] = _rotation.matrix[2][0];
  matrix[2][1] = _rotation.matrix[2][1];
  matrix[2][2] = _rotation.matrix[2][2];
}

Matrix4
Matrix4::lookAtLH(Vector4 _eyePos, Vector4 _atPos, Vector4 _upDir)
{
  Vector4 EyeDirection;
  Matrix4 M;

  EyeDirection = _atPos - _eyePos;
  M = lookToLH(_eyePos, EyeDirection, _upDir);

  return M;
}

Matrix4
Matrix4::lookToLH(Vector4 _eyePos, Vector4 _eyeDir, Vector4 _upDir)
{
  Vector4 negEyePosition;
  Vector4 R0, R1, R2;
  Matrix4 M;
  // forward vector
  Vector4 eyeDirectionNormalized = _eyeDir;
  eyeDirectionNormalized.normalize();
  R2 = eyeDirectionNormalized;

  // right vector
  Vector4 R0CrossProduct;
  R0CrossProduct = _upDir ^ R2;
  R0CrossProduct.normalize();
  R0 = R0CrossProduct;

  // up vector
  Vector4 R1CrossProduct;
  R1CrossProduct = R2 ^ R0;
  R1 = R1CrossProduct;

  negEyePosition = -_eyePos;

  // get the rows dot product
  float R0Dot = Vector4::dotProd(negEyePosition, R0);
  float R1Dot = Vector4::dotProd(negEyePosition, R1);
  float R2Dot = Vector4::dotProd(negEyePosition, R2);

  // set the matrix
  M.matrix[0][0] = R0.x;
  M.matrix[0][1] = R1.x;
  M.matrix[0][2] = R2.x;
  M.matrix[0][3] = 0.0f;

  M.matrix[1][0] = R0.y;
  M.matrix[1][1] = R1.y;
  M.matrix[1][2] = R2.y;
  M.matrix[1][3] = 0.0f;

  M.matrix[2][0] = R0.z;
  M.matrix[2][1] = R1.z;
  M.matrix[2][2] = R2.z;
  M.matrix[2][3] = 0.0f;

  M.matrix[3][0] = R0Dot;
  M.matrix[3][1] = R1Dot;
  M.matrix[3][2] = R2Dot;
  M.matrix[3][3] = 1.0f;

  return M;
}

Matrix4
Matrix4::perspectiveFOVLH(float _halfFOV, float _width, float _height, float _nearZ, float _farZ)
{
  Matrix4 M(0.0f);

  M.matrix[0][0] = 1.0f / tanf(_halfFOV);
  M.matrix[1][1] = _width / tanf(_halfFOV) / _height;
  M.matrix[2][2] = _farZ / (_farZ - _nearZ);
  M.matrix[2][3] = 1.0f;
  M.matrix[3][2] = -_nearZ * (_farZ / (_farZ - _nearZ));

  return M;
}

Matrix4
Matrix4::orthographicFOVLH(float _left,
                           float _right,
                           float _top,
                           float _bottom,
                           float _nearZ,
                           float _farZ)
{
  Matrix4 M = Matrix4::IDENTITY;
  M.matrix[0][0] = 2.0f / (_right - _left);
  M.matrix[0][3] = -(_right + _left) / (_right - _left);

  M.matrix[1][1] = 2.0f / (_top - _bottom);
  M.matrix[1][3] = -(_top + _bottom) / (_top - _bottom);

  M.matrix[2][2] = 1.0f / (_farZ - _nearZ);
  M.matrix[2][3] = -_nearZ / (_farZ - _nearZ);
  return M;
}
}
