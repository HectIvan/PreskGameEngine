#include <iostream>

#include "pkLogger.h"

using std::cout;
using std::endl;

namespace pkEngineSDK
{

void
Logger::print(String _text)
{
  cout << _text << endl << endl;
}

void
Logger::print(float _num)
{
  cout << _num << endl << endl;
}

void
Logger::print(Vector2 _vec)
{
  cout << _vec.x << " " << _vec.y << endl << endl;
}

void
Logger::print(Vector3 _vec)
{
  cout << _vec.x << " " << _vec.y << " " << _vec.z << endl << endl;
}

void
Logger::print(Vector4 _vec)
{
  cout << _vec.x << " " << _vec.y << " " << _vec.z << " " << _vec.w << endl << endl;
}

void
Logger::print(Matrix4 _matrix)
{
  // First row.
  cout << _matrix.matrix[0][0] << " "
       << _matrix.matrix[0][1] << " "
       << _matrix.matrix[0][2] << " "
       << _matrix.matrix[0][3] << " " << endl;
  // Second row.
  cout << _matrix.matrix[1][0] << " "
       << _matrix.matrix[1][1] << " "
       << _matrix.matrix[1][2] << " "
       << _matrix.matrix[1][3] << " " << endl;
  // Third row.
  cout << _matrix.matrix[2][0] << " "
       << _matrix.matrix[2][1] << " "
       << _matrix.matrix[2][2] << " "
       << _matrix.matrix[2][3] << " " << endl;
  // Fourth row.
  cout << _matrix.matrix[3][0] << " "
       << _matrix.matrix[3][1] << " "
       << _matrix.matrix[3][2] << " "
       << _matrix.matrix[3][3] << " " << endl << endl;
}
PK_CORE_EXPORT Logger&
g_Logger()
{
  return Logger::instance();
}
}