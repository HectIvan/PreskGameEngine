#include "pkShape.h"
#include "pkPlatformMath.h"

namespace pkEngineSDK 
{
Vector3
Shape::getSupportPoint(Vector3& _direction)
{
  return Math::supportPointConvex(_direction, m_vertex);
}

void
Shape::subexpressions(float _w0, float _w1, float _w2,
                      float& _f1, float& _f2, float& _f3, float& _g0, float& _g1, float& _g2)
{
  float temp0 = _w0 + _w1;
  _f1 = temp0 + _w2;
  float temp1 = _w0 * _w0;
  float temp2 = temp1 + _w1 * temp0;
  _f2 = temp2 + _w2 * _f1;
  _f3 = _w0 * temp1 + _w1 * temp2 + _w2 * _f2;
  _g0 = _f2 + _w0 * (_f1 + _w0);
  _g1 = _f2 + _w1 * (_f1 + _w1);
  _g2 = _f2 + _w2 * (_f1 + _w2);
}

Vector3
Shape::computeMassProperties(const Shape& _mesh, float _mass, Vector3 _cm, Matrix4& _inertia)
{
  const float mult[10] = {
        1.0f / 6, 1.0f / 24, 1.0f / 24, 1.0f / 24,
        1.0f / 60, 1.0f / 60, 1.0f / 60,
        1.0f / 120, 1.0f / 120, 1.0f / 120
  }; // integration constants

  float intg[10] = {};

  for (size_t t = 0; t < _mesh.m_index.size(); t += 3) {
    const Vector3& p0 = _mesh.m_vertex[_mesh.m_index[t]];
    const Vector3& p1 = _mesh.m_vertex[_mesh.m_index[t + 1]];
    const Vector3& p2 = _mesh.m_vertex[_mesh.m_index[t + 2]];

    float a1 = p1.x - p0.x, b1 = p1.y - p0.y, c1 = p1.z - p0.z;
    float a2 = p2.x - p0.x, b2 = p2.y - p0.y, c2 = p2.z - p0.z;

    float d0 = b1 * c2 - b2 * c1;
    float d1 = a2 * c1 - a1 * c2;
    float d2 = a1 * b2 - a2 * b1;

    float f1x, f2x, f3x, g0x, g1x, g2x;
    float f1y, f2y, f3y, g0y, g1y, g2y;
    float f1z, f2z, f3z, g0z, g1z, g2z;

    subexpressions(p0.x, p1.x, p2.x, f1x, f2x, f3x, g0x, g1x, g2x);
    subexpressions(p0.y, p1.y, p2.y, f1y, f2y, f3y, g0y, g1y, g2y);
    subexpressions(p0.z, p1.z, p2.z, f1z, f2z, f3z, g0z, g1z, g2z);

    intg[0] += d0 * f1x;
    intg[1] += d0 * f2x;
    intg[2] += d1 * f2y;
    intg[3] += d2 * f2z;
    intg[4] += d0 * f3x;
    intg[5] += d1 * f3y;
    intg[6] += d2 * f3z;
    intg[7] += d0 * (p0.y * g0x + p1.y * g1x + p2.y * g2x);
    intg[8] += d1 * (p0.z * g0y + p1.z * g1y + p2.z * g2y);
    intg[9] += d2 * (p0.x * g0z + p1.x * g1z + p2.x * g2z);

    for (size_t i = 0; i < 10; ++i) {
      intg[i] *= mult[i];
    }

    float volume = intg[0];
    float density = _mass / volume;

    _cm.x = intg[1] / volume;
    _cm.y = intg[2] / volume;
    _cm.z = intg[3] / volume;

    _inertia.matrix[0][0] = density * (intg[5] + intg[6] - volume *
                            (_cm.y * _cm.y + _cm.z * _cm.z)); // Iii
    _inertia.matrix[1][1] = density * (intg[4] + intg[6] - volume *
                            (_cm.z * _cm.z + _cm.x * _cm.x)); // Ijj
    _inertia.matrix[2][2] = density * (intg[4] + intg[5] - volume *
                            (_cm.x * _cm.x + _cm.y * _cm.y)); // Ikk

    _inertia.matrix[0][1] = -density * (intg[7] - volume * _cm.x * _cm.y);          // Iij
    _inertia.matrix[1][0] = _inertia.matrix[0][1];
    _inertia.matrix[1][2] = -density * (intg[8] - volume * _cm.y * _cm.z);          // Ijk
    _inertia.matrix[2][1] = _inertia.matrix[1][2];
    _inertia.matrix[0][2] = -density * (intg[9] - volume * _cm.z * _cm.x);          // Iik
    _inertia.matrix[2][0] = _inertia.matrix[0][2];
  }
}
}
