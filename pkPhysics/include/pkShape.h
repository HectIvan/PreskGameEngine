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

 public:
  Vector<Vector3> m_vertex;
};
}