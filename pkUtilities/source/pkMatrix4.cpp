/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkMath.h"
#include "pkMatrix4.h"
#include "pkQuaternion.h"

namespace pkEngineSDK {

const Matrix4 Matrix4::IDENTITY = Matrix4(1.0f, 0.0f, 0.0f, 0.0f,
                                          0.0f, 1.0f, 0.0f, 0.0f,
                                          0.0f, 0.0f, 1.0f, 0.0f,
                                          0.0f, 0.0f, 0.0f, 1.0f);
const Matrix4 Matrix4::ZERO = Matrix4(0);

Matrix4
Matrix4::operator=(const Matrix4& _other)
{
  for (uint32 i = 0; i < 4; ++i) {
    for (uint32 j = 0; j < 4; ++j) {
      matrix[i][j] = _other.matrix[i][j];
    }
  }
  return *this;
}

bool
Matrix4::operator==(const Matrix4& _other) const
{
  for (uint32 i = 0; i < 4; ++i) {
    for (uint32 j = 0; j < 4; ++j) {
      if (matrix[i][j] != _other.matrix[i][j]) {
        return false;
      }
    }
  }
  return true;
}

Matrix4
Matrix4::operator+(const Matrix4& _other) const
{
  Matrix4 result;
  for (uint32 i = 0; i < 4; ++i) {
    for (uint32 j = 0; j < 4; ++j) {
      result.matrix[i][j] = matrix[i][j] + _other.matrix[i][j];
    }
  }
  return result;
}

Matrix4
Matrix4::operator+(const float& _other) const
{
  Matrix4 result;
  for (uint32 i = 0; i < 4; ++i) {
    for (uint32 j = 0; j < 4; ++j) {
      result.matrix[i][j] = matrix[i][j] + _other;
    }
  }
  return result;
}

Matrix4
Matrix4::operator-(const Matrix4& _other) const
{
  Matrix4 result;
  for (uint32 i = 0; i < 4; ++i) {
    for (uint32 j = 0; j < 4; ++j) {
      result.matrix[i][j] = matrix[i][j] - _other.matrix[i][j];
    }
  }
  return result;
}

Matrix4
Matrix4::operator-(const float& _other) const
{
  Matrix4 result;
  for (uint32 i = 0; i < 4; ++i) {
    for (uint32 j = 0; j < 4; ++j) {
      result.matrix[i][j] = matrix[i][j] - _other;
    }
  }
  return result;
}

Matrix4
Matrix4::operator*(const Matrix4& other) const
{
  Matrix4 result;
  for (uint32 row = 0; row < 4; ++row) {
    for (uint32 col = 0; col < 4; ++col) {
      result.matrix[row][col] = matrix[row][0] * other.matrix[0][col] +
                                matrix[row][1] * other.matrix[1][col] +
                                matrix[row][2] * other.matrix[2][col] +
                                matrix[row][3] * other.matrix[3][col];
    }
  }
  return result;
}

Vector4
Matrix4::operator*(const Vector4& _other) const
{
  const float x00 = matrix[0][0] * _other.x;
  const float x10 = matrix[1][0] * _other.x;
  const float x20 = matrix[2][0] * _other.x;
  const float x30 = matrix[3][0] * _other.x;

  const float y01 = matrix[0][1] * _other.y;
  const float y11 = matrix[1][1] * _other.y;
  const float y21 = matrix[2][1] * _other.y;
  const float y31 = matrix[3][1] * _other.y;

  const float z02 = matrix[0][2] * _other.z;
  const float z12 = matrix[1][2] * _other.z;
  const float z22 = matrix[2][2] * _other.z;
  const float z32 = matrix[3][2] * _other.z;

  const float w03 = matrix[0][3] * _other.w;
  const float w13 = matrix[1][3] * _other.w;
  const float w23 = matrix[2][3] * _other.w;
  const float w33 = matrix[3][3] * _other.w;

  const float x = x00 + y01 + z02 + w03;
  const float y = x10 + y11 + z12 + w13;
  const float z = x20 + y21 + z22 + w23;
  const float w = x30 + y31 + z32 + w33;

  return Vector4(x, y, z, w);
}

Matrix4
Matrix4::operator*(const float& _other) const
{
  Matrix4 result;
  for (uint32 i = 0; i < 4; ++i) {
    for (uint32 j = 0; j < 4; ++j) {
      result.matrix[i][j] = matrix[i][j] * _other;
    }
  }
  return result;
}

Matrix4::Matrix4(const float& val)
{
  for (uint32 i = 0; i < 4; ++i) {
    for (uint32 j = 0; j < 4; ++j) {
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

Matrix3
Matrix4::getMatrix3(const uint32 _x, const uint32 _y) const
{
  Matrix3 result;
  for (uint32 i = 0; i < 3; ++i) {
    for (uint32 j = 0; j < 3; ++j) {
      result.matrix[i][j] = matrix[i + _y][j + _x];
    }
  }
  return result;
}

const Quaternion
Matrix4::getLocalRotation() const
{
  Quaternion localRotation;
  localRotation.x = Math::atan2(matrix[2][1], matrix[2][2]);
  localRotation.y = Math::asin(-matrix[2][0]);
  localRotation.z = Math::atan2(matrix[1][0], matrix[0][0]);
  localRotation.w = 1.0f; // Assuming no scaling, w can be set to 1.0f
  return localRotation;
}

const Vector3
Matrix4::getForwardVector() const
{
  return Vector3(matrix[0][2], matrix[1][2], matrix[2][2]);
}

const Vector3
Matrix4::getUpVector() const
{
  return Vector3(matrix[0][1], matrix[1][1], matrix[2][1]);
}

const Vector3
Matrix4::getRightVector() const
{
  return Vector3(matrix[0][0], matrix[1][0], matrix[2][0]);
}

const Vector3
Matrix4::getViewPosition() const
{
  return Vector3(matrix[0][3], matrix[1][3], matrix[2][3]);
}

const Matrix4
Matrix4::inverse() const
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

const Matrix4
Matrix4::getTransposed() const
{
  Matrix4 result;
  for (uint32 i = 0; i < 4; ++i) {
    for (uint32 j = 0; j < 4; ++j) {
      result.matrix[i][j] = matrix[j][i];
    }
  }
  return result;
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
Matrix4::getTranslation() const
{
  Matrix4 M = Matrix4::IDENTITY;
  M.matrix[0][3] = matrix[0][3];
  M.matrix[1][3] = matrix[1][3];
  M.matrix[2][3] = matrix[2][3];
  return M;
}

const Vector3
Matrix4::getTranslation3() const
{
  return Vector3(matrix[0][3], matrix[1][3], matrix[2][3]);
}

const Matrix4
Matrix4::scale(const Vector3& _scale)
{
  Matrix4 M = Matrix4::IDENTITY;
  M.matrix[0][0] = _scale.x;
  M.matrix[1][1] = _scale.y;
  M.matrix[2][2] = _scale.z;
  return M;
}

const Matrix4
Matrix4::scale(const float& _scaleX, const float& _scaleY, const float& _scaleZ)
{
  return scale(Vector3(_scaleX, _scaleY, _scaleZ));
}

const Matrix4
Matrix4::getScale()
{
  Matrix4 M = Matrix4::IDENTITY;
  const Vector3 scale = getScale3();
  M.matrix[0][0] = scale.x;
  M.matrix[1][1] = scale.y;
  M.matrix[2][2] = scale.z;
  return M;
}

const Vector3
Matrix4::getScale3() const
{
  return Vector3(matrix[0][0], matrix[1][1], matrix[2][2]);
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
Matrix4::MatrixRotationAxis(const Vector3& _axis, const float& _angle)
{
  PK_ASSERT(!_axis.isZero());
  PK_ASSERT(!Math::isNan(_axis));

  const Vector3 axis = _axis.normalized();
  
  //Compute rotation matrix from axis and angle
  const float s = Math::sin(_angle);
  const float c = Math::cos(_angle);
  const float t = 1.0f - c;

  const float x = axis.x;
  const float y = axis.y;
  const float z = axis.z;

  const float tx = t * x;
  const float ty = t * y;
  const float tz = t * z;

  const float txy = tx * y;
  const float txz = tx * z;
  const float tyz = ty * z;

  const float sx = s * x;
  const float sy = s * y;
  const float sz = s * z;

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
Matrix4::getRotation() const
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
Matrix4::getRotation(const Vector3& _scale) const
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

/*
 * Rotation matrix from quaternion
    1.0f - 2.0f*qy*qy - 2.0f*qz*qz,     2.0f*qx*qy - 2.0f*qz*qw,        2.0f*qx*qz + 2.0f*qy*qw,        0.0f,
    2.0f*qx*qy + 2.0f*qz*qw,            1.0f - 2.0f*qx*qx - 2.0f*qz*qz, 2.0f*qy*qz - 2.0f*qx*qw,        0.0f,
    2.0f*qx*qz - 2.0f*qy*qw,            2.0f*qy*qz + 2.0f*qx*qw,        1.0f - 2.0f*qx*qx - 2.0f*qy*qy, 0.0f,
    0.0f,                               0.0f,                           0.0f,                           1.0f);
*/
const Matrix4
Matrix4::rotation(const Quaternion& _quat)
{
  const Quaternion quat = _quat.normalized();

  const float w = quat.w;
  const float x = quat.x;
  const float y = quat.y;
  const float z = quat.z;

  Matrix4 M = Matrix4::IDENTITY;
  M.matrix[0][0] = 1.0f - 2.0f * y * y - 2.0f * z * z;
  M.matrix[0][1] = 2.0f * x * y - 2.0f * z * w;
  M.matrix[0][2] = 2.0f * x * z + 2.0f * y * w;

  M.matrix[1][0] = 2.0f * x * y + 2.0f * z * w;
  M.matrix[1][1] = 1.0f - 2.0f * x * x - 2.0f * z * z;
  M.matrix[1][2] = 2.0f * y * z - 2.0f * x * w;

  M.matrix[2][0] = 2.0f * x * z - 2.0f * y * w;
  M.matrix[2][1] = 2.0f * y * z + 2.0f * x * w;
  M.matrix[2][2] = 1.0f - 2.0f * x * x - 2.0f * y * y;

  return M;
}

const Matrix4
Matrix4::rotation(const float& _angleX, const float& _angleY, const float& _angleZ)
{
  const Matrix4 X = rotationX(_angleX);
  const Matrix4 Y = rotationY(_angleY);
  const Matrix4 Z = rotationZ(_angleZ);
  return Z * X * Y;
}

/**

Rz(A) =   | cos(A) -sin(A)  0  0 |
          | sin(A)  cos(A)  0  0 |
          |   0       0     1  0 |
          |   0       0     0  1 |

*/
Matrix4
Matrix4::rotationZ(const float& _angle)
{
  Matrix4 M = Matrix4::IDENTITY;

  const float fSinAngle = sinf(_angle);
  const float fCosAngle = cosf(_angle);

  M.matrix[0][0] = fCosAngle;
  M.matrix[0][1] = -fSinAngle;

  M.matrix[1][0] = fSinAngle;
  M.matrix[1][1] = fCosAngle;
  return M;
}


/**

Ry(B) =  | cos(B)  0  sin(B)  0 |
         |   0     1    0     0 |
         | -sin(B) 0  cos(B)  0 |
         |   0     0    0     1 |

*/
Matrix4
Matrix4::rotationY(const float& _angle)
{
  Matrix4 M = Matrix4::IDENTITY;

  const float fSinAngle = sinf(_angle);
  const float fCosAngle = cosf(_angle);

  M.matrix[0][0] = fCosAngle;
  M.matrix[0][2] = fSinAngle;

  M.matrix[2][0] = -fSinAngle;
  M.matrix[2][2] = fCosAngle;

  return M;
}

/**

Rx(Y) =  | 1    0         0     0 |
         | 0  cos(Y)   -sin(Y)  0 |
         | 0  sin(Y)    cos(Y)  0 |
         | 0    0         0     1 |

*/
Matrix4
Matrix4::rotationX(const float& _angle)
{
  Matrix4 M = IDENTITY;

  const float fSinAngle = sinf(_angle);
  const float fCosAngle = cosf(_angle);

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

const Matrix4
Matrix4::lookAtLH(const Vector4& _eyePos, const Vector4& _atPos, const Vector3& _upDir)
{
  const Vector4 EyeDirection = _atPos - _eyePos;
  const Vector4 upDir = Vector4(_upDir, 0.0f);
  const Matrix4 M = lookToLH(_eyePos, EyeDirection, upDir);

  return M;
}

const Matrix4
Matrix4::lookToLH(const Vector4& _eyePos, const Vector4& _eyeDir, const Vector4& _upDir)
{
  Vector4 R0, R1, R2;

  // forward vector
  R2 = _eyeDir.normalized();
  // right vector
  R0 = (_upDir ^ R2).normalized();
  // up vector
  R1 = R2 ^ R0;

  // get the rows dot product
  float R0Dot = -Math::dotProd(_eyePos, R0);
  float R1Dot =  Math::dotProd(_eyePos, R1);
  float R2Dot =  Math::dotProd(_eyePos, R2);

  return Matrix4(Vector4(R0.x, R0.y, R0.z, R0Dot),
                 Vector4(R1.x, R1.y, R1.z, R1Dot),
                 Vector4(R2.x, R2.y, R2.z, R2Dot),
                 Vector4(0.0f, 0.0f, 0.0f, 1.0f));
}

const Matrix4
Matrix4::perspectiveFOVLH(const float& _halfFOV,
                          const float& _width,
                          const float& _height,
                          const float& _nearZ,
                          const float& _farZ)
{
  Matrix4 M = Matrix4::IDENTITY;

  const float tHFov = tanf(_halfFOV);
  const float far_near = _farZ - _nearZ;
  const float aspect = _width / _height;

  M.matrix[0][0] = 1.0f / (tHFov * aspect);
  M.matrix[1][1] = 1.0f / tHFov;
  M.matrix[2][2] = _farZ / far_near;
  M.matrix[2][3] = 1.0f;
  M.matrix[3][2] = -_nearZ * _farZ / far_near;

  return M;
}

const Matrix4
Matrix4::orthographicFOVLH(const float& _left,
                           const float& _right,
                           const float& _top,
                           const float& _bottom,
                           const float& _nearZ,
                           const float& _farZ)
{
  const float far_near = _farZ - _nearZ;
  const float top_bottom = _top - _bottom;
  const float right_left = _right - _left;

  Matrix4 M = Matrix4::IDENTITY;
  M.matrix[0][0] = 2.0f / right_left;
  M.matrix[0][3] = -(_right + _left) / right_left;

  M.matrix[1][1] = 2.0f / top_bottom;
  M.matrix[1][3] = -(_top + _bottom) / top_bottom;

  M.matrix[2][2] = 1.0f / far_near;
  M.matrix[2][3] = -_nearZ / far_near;
  return M;
}
}
