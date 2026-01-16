/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkMath.h"
#include "pkMatrix4.h"

namespace pkEngineSDK {

const Matrix4 Matrix4::IDENTITY = Matrix4(Vector4(1.0f, 0.0f, 0.0f, 0.0f),
                                          Vector4(0.0f, 1.0f, 0.0f, 0.0f),
                                          Vector4(0.0f, 0.0f, 1.0f, 0.0f),
                                          Vector4(0.0f, 0.0f, 0.0f, 1.0f));
const Matrix4 Matrix4::ZERO = Matrix4(0);

Matrix4::Matrix4(const float& val)
{
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
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

Matrix4::Matrix4(const Vector4& R0, const Vector4& R1, const Vector4& R2, const Vector4& R3)
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

Matrix4::Matrix4(const float& m00, const float& m01, const float& m02, const float& m03,
                 const float& m10, const float& m11, const float& m12, const float& m13,
                 const float& m20, const float& m21, const float& m22, const float& m23,
                 const float& m30, const float& m31, const float& m32, const float& m33)
{
  matrix[0][0] = m00; matrix[0][1] = m01; matrix[0][2] = m02; matrix[0][3] = m03;
  matrix[1][0] = m10; matrix[1][1] = m11;	matrix[1][2] = m12; matrix[1][3] = m13;
  matrix[2][0] = m20; matrix[2][1] = m21;	matrix[2][2] = m22; matrix[2][3] = m23;
  matrix[3][0] = m30; matrix[3][1] = m31; matrix[3][2] = m32; matrix[3][3] = m33;
}

Vector3
Matrix4::getForwardVector()
{
  return Vector3(matrix[0][2], matrix[1][2], matrix[2][2]);
}

Vector3
Matrix4::getUpVector()
{
  return Vector3(matrix[0][1], matrix[1][1], matrix[2][1]);
}

Vector3
Matrix4::getRightVector()
{
  return Vector3(matrix[0][0], matrix[1][0], matrix[2][0]);
}

Vector3
Matrix4::getViewPosition()
{
  return Vector3(matrix[3][0], matrix[3][1], matrix[3][2]);
}

Matrix4
Matrix4::inverse()
{
  // Cofactors
  Vector<float> inv;
  inv.resize(16);
  float det;

  inv[0] = matrix[1][1] * matrix[2][2] * matrix[3][3] -
           matrix[1][1] * matrix[2][3] * matrix[3][2] -
           matrix[2][1] * matrix[1][2] * matrix[3][3] +
           matrix[2][1] * matrix[1][3] * matrix[3][2] +
           matrix[3][1] * matrix[1][2] * matrix[2][3] -
           matrix[3][1] * matrix[1][3] * matrix[2][2];

  inv[1] = -matrix[0][1] * matrix[2][2] * matrix[3][3] +
            matrix[0][1] * matrix[2][3] * matrix[3][2] +
            matrix[2][1] * matrix[0][2] * matrix[3][3] -
            matrix[2][1] * matrix[0][3] * matrix[3][2] -
            matrix[3][1] * matrix[0][2] * matrix[2][3] +
            matrix[3][1] * matrix[0][3] * matrix[2][2];

  inv[2] = matrix[0][1] * matrix[1][2] * matrix[3][3] -
           matrix[0][1] * matrix[1][3] * matrix[3][2] -
           matrix[1][1] * matrix[0][2] * matrix[3][3] +
           matrix[1][1] * matrix[0][3] * matrix[3][2] +
           matrix[3][1] * matrix[0][2] * matrix[1][3] -
           matrix[3][1] * matrix[0][3] * matrix[1][2];

  inv[3] = -matrix[0][1] * matrix[1][2] * matrix[2][3] +
            matrix[0][1] * matrix[1][3] * matrix[2][2] +
            matrix[1][1] * matrix[0][2] * matrix[2][3] -
            matrix[1][1] * matrix[0][3] * matrix[2][2] -
            matrix[2][1] * matrix[0][2] * matrix[1][3] +
            matrix[2][1] * matrix[0][3] * matrix[1][2];

  inv[4] = -matrix[1][0] * matrix[2][2] * matrix[3][3] +
            matrix[1][0] * matrix[2][3] * matrix[3][2] +
            matrix[2][0] * matrix[1][2] * matrix[3][3] -
            matrix[2][0] * matrix[1][3] * matrix[3][2] -
            matrix[3][0] * matrix[1][2] * matrix[2][3] +
            matrix[3][0] * matrix[1][3] * matrix[2][2];

  inv[5] = matrix[0][0] * matrix[2][2] * matrix[3][3] -
           matrix[0][0] * matrix[2][3] * matrix[3][2] -
           matrix[2][1] * matrix[0][2] * matrix[3][3] +
           matrix[2][1] * matrix[0][3] * matrix[3][2] +
           matrix[3][1] * matrix[0][2] * matrix[2][3] -
           matrix[3][1] * matrix[0][3] * matrix[2][2];

  inv[6] = -matrix[0][0] * matrix[2][2] * matrix[3][3] +
            matrix[0][0] * matrix[2][3] * matrix[3][2] +
            matrix[1][0] * matrix[0][2] * matrix[3][3] -
            matrix[1][0] * matrix[0][3] * matrix[3][2] -
            matrix[3][0] * matrix[0][2] * matrix[1][3] +
            matrix[3][0] * matrix[0][3] * matrix[1][2];

  inv[7] = matrix[0][0] * matrix[1][2] * matrix[2][3] -
           matrix[0][0] * matrix[1][3] * matrix[2][2] -
           matrix[1][0] * matrix[0][2] * matrix[2][3] +
           matrix[1][0] * matrix[0][3] * matrix[2][2] +
           matrix[2][0] * matrix[0][2] * matrix[1][3] -
           matrix[2][0] * matrix[0][3] * matrix[1][2];

  inv[8] = matrix[1][0] * matrix[2][1] * matrix[3][3] -
           matrix[1][0] * matrix[2][3] * matrix[3][1] -
           matrix[2][0] * matrix[1][1] * matrix[3][3] +
           matrix[2][0] * matrix[1][3] * matrix[3][1] +
           matrix[3][0] * matrix[1][1] * matrix[2][3] -
           matrix[3][0] * matrix[1][3] * matrix[2][1];

  inv[9] = -matrix[0][0] * matrix[2][1] * matrix[3][3] +
            matrix[0][0] * matrix[2][3] * matrix[3][1] +
            matrix[2][0] * matrix[0][1] * matrix[3][3] -
            matrix[2][0] * matrix[0][3] * matrix[3][1] -
            matrix[3][0] * matrix[0][1] * matrix[2][3] +
            matrix[3][0] * matrix[0][3] * matrix[2][1];

  inv[10] = matrix[0][0] * matrix[1][1] * matrix[3][3] -
            matrix[0][0] * matrix[1][3] * matrix[3][1] -
            matrix[1][0] * matrix[0][1] * matrix[3][3] +
            matrix[1][0] * matrix[0][3] * matrix[3][1] +
            matrix[3][0] * matrix[0][1] * matrix[1][3] -
            matrix[3][0] * matrix[0][3] * matrix[1][1];

  inv[11] = -matrix[0][0] * matrix[1][1] * matrix[2][3] +
             matrix[0][0] * matrix[1][3] * matrix[2][1] +
             matrix[1][0] * matrix[0][1] * matrix[2][3] -
             matrix[1][0] * matrix[0][3] * matrix[2][1] -
             matrix[2][0] * matrix[0][1] * matrix[1][3] +
             matrix[2][0] * matrix[0][3] * matrix[1][1];

  inv[12] = -matrix[1][0] * matrix[2][1] * matrix[3][2] +
             matrix[1][0] * matrix[2][2] * matrix[3][1] +
             matrix[2][0] * matrix[1][1] * matrix[3][2] -
             matrix[2][0] * matrix[1][2] * matrix[3][1] -
             matrix[3][0] * matrix[1][1] * matrix[2][2] +
             matrix[3][0] * matrix[1][2] * matrix[2][1];

  inv[13] = matrix[0][0] * matrix[2][1] * matrix[3][2] -
            matrix[0][0] * matrix[2][2] * matrix[3][1] -
            matrix[2][0] * matrix[0][1] * matrix[3][2] +
            matrix[2][0] * matrix[0][2] * matrix[3][1] +
            matrix[3][0] * matrix[0][1] * matrix[2][2] -
            matrix[3][0] * matrix[0][2] * matrix[2][1];

  inv[14] = -matrix[0][0] * matrix[1][1] * matrix[3][2] +
             matrix[0][0] * matrix[1][2] * matrix[3][1] +
             matrix[1][0] * matrix[0][1] * matrix[3][2] -
             matrix[1][0] * matrix[0][2] * matrix[3][1] -
             matrix[3][0] * matrix[0][1] * matrix[1][2] +
             matrix[3][0] * matrix[0][2] * matrix[1][1];

  inv[15] = matrix[0][0] * matrix[1][1] * matrix[2][2] -
            matrix[0][0] * matrix[1][2] * matrix[2][1] -
            matrix[1][0] * matrix[0][1] * matrix[2][2] +
            matrix[1][0] * matrix[0][2] * matrix[2][1] +
            matrix[2][0] * matrix[0][1] * matrix[1][2] -
            matrix[2][0] * matrix[0][2] * matrix[1][1];

  det = matrix[0][0] * inv[0] +
        matrix[0][1] * inv[4] +
        matrix[0][2] * inv[8] +
        matrix[0][3] * inv[12];

  det = 1.0f / det;

  for (uint32 i = 0; i < 16; ++i) {
    inv[i] = inv[i] * det;
  }

  return Matrix4(inv[0], inv[1], inv[2], inv[3],
                 inv[4], inv[5], inv[6], inv[7],
                 inv[8], inv[9], inv[10], inv[11],
                 inv[12], inv[13], inv[14], inv[15]);
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

const Matrix4
Matrix4::matrixScaling(const float& _scaleX, const float& _scaleY, const float& _scaleZ)
{
  return Matrix4(Vector4(_scaleX, 0.0f, 0.0f, 0.0f),
                 Vector4(0.0f, _scaleY, 0.0f, 0.0f),
                 Vector4(0.0f, 0.0f, _scaleZ, 0.0f),
                 Vector4(0.0f, 0.0f, 0.0f, 1.0f));
}

const Matrix4
Matrix4::translation(const Vector3& _position)
{
  return translation(_position.x, _position.y, _position.z);
}

const Matrix4
Matrix4::translation(const float& _offsetX, const float& _offsetY, const float& _offsetZ)
{
  Matrix4 M = Matrix4::IDENTITY;
  M.matrix[0][3] = _offsetX;
  M.matrix[1][3] = _offsetY;  
  M.matrix[2][3] = _offsetZ;
  return M;
}

void
Matrix4::setTranslation(const Vector3& _pos)
{
  matrix[0][3] = _pos.x;
  matrix[1][3] = _pos.y;
  matrix[2][3] = _pos.z;
}

void
Matrix4::setTranslation(const float& _x, const float& _y, const float& _z)
{
  matrix[0][3] = _x;
  matrix[1][3] = _y;
  matrix[2][3] = _z;
}

const Matrix4
Matrix4::getTranslation()
{
  Matrix4 M = Matrix4::IDENTITY;
  M.matrix[0][3] = matrix[0][3];
  M.matrix[1][3] = matrix[1][3];
  M.matrix[2][3] = matrix[2][3];
  return M;
}

const Vector3
Matrix4::getTranslation3()
{
  return Vector3(matrix[0][3], matrix[1][3], matrix[2][3]);
}

const Matrix4
Matrix4::scale(const Vector3& _scale)
{
  Matrix4 M = Matrix4::IDENTITY;
  M.setScale(_scale);
  return M;
}

const Matrix4
Matrix4::getScale()
{
  Matrix4 M = Matrix4::IDENTITY;
  const Vector3 scale = getScale3();
  M.matrix[0][3] = scale.x;
  M.matrix[1][3] = scale.y;
  M.matrix[2][3] = scale.z;
  return M;
}

const Vector3
Matrix4::getScale3()
{
  return Vector3(matrix[0][3],
                 matrix[1][3],
                 matrix[2][3]);
}

void
Matrix4::setScale(const Vector3& _scale)
{
  setScale(_scale.x, _scale.y, _scale.z);
}

void
Matrix4::setScale(Matrix4& _scale)
{
  setScale(_scale.getScale3());
}

void
Matrix4::setScale(const float& _val)
{
  setScale(_val, _val, _val);
}

void
Matrix4::setScale(const float& _x, const float& _y, const float& _z)
{
  matrix[0][0] *= _x;
  matrix[1][1] *= _y;
  matrix[2][2] *= _z;
}

Matrix4
Matrix4::MatrixRotationAxis(Vector3& _axis, const float& _angle)
{
  PK_ASSERT(!_axis.isZero());
  PK_ASSERT(!_axis.hasNan());

  _axis.normalize();
  
  //Compute rotation matrix from axis and angle
  float s = Math::sin(_angle);
  float c = Math::cos(_angle);
  float t = 1.0f - c;

  float x = _axis.x;  float y = _axis.y;  float z = _axis.z;

  float tx = t * x;  float ty = t * y;  float tz = t * z;
  float txy = tx * y;  float txz = tx * z;  float tyz = ty * z;
  float sx = s * x;  float sy = s * y;  float sz = s * z;

  Matrix4 Result = Matrix4::IDENTITY;
  Result.matrix[0][0] = tx * x + c;
  Result.matrix[0][1] = txy + sz;
  Result.matrix[0][2] = txz - sy;

  Result.matrix[1][0] = txy - sz;
  Result.matrix[1][1] = ty * y + c;
  Result.matrix[1][2] = tyz + sx;

  Result.matrix[2][0] = txz + sy;
  Result.matrix[2][1] = tyz - sx;
  Result.matrix[2][2] = tz * z + c;

  return Result;
}

const Matrix4
Matrix4::getRotation()
{
  Matrix4 rot = Matrix4::IDENTITY;
  rot.matrix[0][0] = matrix[0][0];
  rot.matrix[0][1] = matrix[0][1];
  rot.matrix[0][2] = matrix[0][2];

  rot.matrix[1][0] = matrix[1][0];
  rot.matrix[1][1] = matrix[1][1];
  rot.matrix[1][2] = matrix[1][2];

  rot.matrix[2][0] = matrix[2][0];
  rot.matrix[2][1] = matrix[2][1];
  rot.matrix[2][2] = matrix[2][2];
  return rot;
}

const Matrix4
Matrix4::getRotationNoScale(const Vector3& _scale)
{
  Matrix4 rotMat = getRotation();
  rotMat.matrix[0][0] /= _scale.x;
  rotMat.matrix[1][1] /= _scale.y;
  rotMat.matrix[2][2] /= _scale.z;
  return rotMat;
}

const Matrix4
Matrix4::rotation(const Vector3& _rot)
{
  return rotation(_rot.x, _rot.y, _rot.z);
}

const Matrix4
Matrix4::rotation(const float& _angleX, const float& _angleY, const float& _angleZ)
{
  Matrix4 M = Matrix4::IDENTITY;
  M = rotationZ(_angleZ) * rotationY(_angleY) * rotationX(_angleX);
  return M;
}

Matrix4
Matrix4::rotationZ(const float& _angle)
{
  Matrix4 M = Matrix4::IDENTITY;

  float fSinAngle = sinf(_angle);
  float fCosAngle = cosf(_angle);

  M.matrix[0][0] = fCosAngle;
  M.matrix[0][1] = -fSinAngle;

  M.matrix[1][0] = fSinAngle;
  M.matrix[1][1] = fCosAngle;
  return M;
}

Matrix4
Matrix4::rotationY(const float& _angle)
{
  Matrix4 M = Matrix4::IDENTITY;

  float fSinAngle = sinf(_angle);
  float fCosAngle = cosf(_angle);

  M.matrix[0][0] = fCosAngle;
  M.matrix[0][2] = fSinAngle;

  M.matrix[2][0] = -fSinAngle;
  M.matrix[2][2] = fCosAngle;

  return M;
}

Matrix4
Matrix4::rotationX(const float& _angle)
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
Matrix4::setRotation(const Matrix4& _rotation)
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
Matrix4::lookAtLH(const Vector4& _eyePos, const Vector4& _atPos, const Vector3& _upDir)
{
  const Vector4 EyeDirection = _atPos - _eyePos;
  Vector4 upDir = Vector4(_upDir, 0.0f);
  const Matrix4 M = lookToLH(_eyePos, EyeDirection, upDir);

  return M;
}

Matrix4
Matrix4::lookToLH(const Vector4& _eyePos, const Vector4& _eyeDir, Vector4& _upDir)
{
  Vector4 negEyePosition;
  Vector4 R0, R1, R2;
  Matrix4 M = Matrix4::IDENTITY;
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

  negEyePosition = _eyePos * -1.0f;

  // get the rows dot product
  float R0Dot = Vector4::dotProd(negEyePosition, R0);
  float R1Dot = Vector4::dotProd(negEyePosition, R1);
  float R2Dot = Vector4::dotProd(negEyePosition, R2);

  // set the matrix
  M.matrix[0][0] = R0.x;
  M.matrix[0][1] = R1.x;
  M.matrix[0][2] = R2.x;

  M.matrix[1][0] = R0.y;
  M.matrix[1][1] = R1.y;
  M.matrix[1][2] = R2.y;

  M.matrix[2][0] = R0.z;
  M.matrix[2][1] = R1.z;
  M.matrix[2][2] = R2.z;

  M.matrix[3][0] = R0Dot;
  M.matrix[3][1] = R1Dot;
  M.matrix[3][2] = R2Dot;

  return M;
}

Matrix4
Matrix4::perspectiveFOVLH(const float& _halfFOV,
                          const float& _width,
                          const float& _height,
                          const float& _nearZ,
                          const float& _farZ)
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
Matrix4::orthographicFOVLH(const float& _left,
                           const float& _right,
                           const float& _top,
                           const float& _bottom,
                           const float& _nearZ,
                           const float& _farZ)
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
