#include "pkMatrix3.h"

namespace pkEngineSDK
{
const Matrix3 Matrix3::IDENTITY = Matrix3(Vector3(1.0f, 0.0f, 0.0f),
                                          Vector3(0.0f, 1.0f, 0.0f),
                                          Vector3(0.0f, 0.0f, 1.0f));
const Matrix3 Matrix3::ZERO = Matrix3(0);

Matrix3::Matrix3(const float val)
{
  for (int i = 0; i < 4; ++i)
  {
    for (int j = 0; j < 4; ++j)
    {
      matrix[i][j] = val;
    }
  }
}

Matrix3::Matrix3(const Matrix3& _matrix)
{
  matrix[0][0] = _matrix.matrix[0][0];
  matrix[0][1] = _matrix.matrix[0][1];
  matrix[0][2] = _matrix.matrix[0][2];

  matrix[1][0] = _matrix.matrix[1][0];
  matrix[1][1] = _matrix.matrix[1][1];
  matrix[1][2] = _matrix.matrix[1][2];

  matrix[2][0] = _matrix.matrix[2][0];
  matrix[2][1] = _matrix.matrix[2][1];
  matrix[2][2] = _matrix.matrix[2][2];
}

Matrix3::Matrix3(Vector3 R0, Vector3 R1, Vector3 R2)
{
  matrix[0][0] = R0.x;
  matrix[0][1] = R0.y;
  matrix[0][2] = R0.z;

  matrix[1][0] = R1.x;
  matrix[1][1] = R1.y;
  matrix[1][2] = R1.z;

  matrix[2][0] = R2.x;
  matrix[2][1] = R2.y;
  matrix[2][2] = R2.z;
}

Matrix3::Matrix3(float m00, float m01, float m02,
                 float m10, float m11, float m12,
                 float m20, float m21, float m22)
{
  matrix[0][0] = m00; matrix[0][1] = m01; matrix[0][2] = m02;
  matrix[1][0] = m10; matrix[1][1] = m11;	matrix[1][2] = m12;
  matrix[2][0] = m20; matrix[2][1] = m21;	matrix[2][2] = m22;
}
}
