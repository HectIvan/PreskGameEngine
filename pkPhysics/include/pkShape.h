/*****************************************************************************/
/**
 * @file    pkShape.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    04/06/2025
 * @brief   Complex shape made of several vertex
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
#include "pkPrerequisitesCore.h"
#include "pkVector3.h"

namespace pkEngineSDK
{

class Shape
{
 public:
  Shape() = default;
  virtual ~Shape() = default;

  /**
   * @brief Get the support point of this shape in a direction.
   * @param _direction Direction to use.
   * @return The support point of the shape.
   */
  Vector3
  getSupportPoint(Vector3& _direction);

  void
  subexpressions(float _w0, float _w1, float _w2,
                 float& _f1, float& _f2, float& _f3, float& _g0, float& _g1, float& _g2);

  /**
   * @brief Get the mass property from a mesh.
   * @param _mesh Mesh to use.
   * @param _cm 
   * @param _inertia Computed inertia
   * @return Computed mass
   */
  Vector3
  computeMassProperties(const Shape& _mesh, float _mass, Vector3 _cm, Matrix4& _inertia);

 public:
  Vector<Vector3> m_vertex;
  Vector<int32> m_index;
  Vector3 m_position;
};
}