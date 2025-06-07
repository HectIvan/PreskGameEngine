/************************************************************************/
/**
* @pkOBB pkOBB.h
* @Hector Ivan Muñoz Ceballos
* @date 27/09/2024
* @OBB file for the Presk Game Engine.
*
* This file contains the OBB class of the engine
*
* @bug No bug known.
*
* @HectIvan 27/09/2024
* Added comments and class
*/
/************************************************************************/
#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkPrerequisitesUtilities.h"
#include "pkMatrix4.h"
#include "pkVector3.h"
#include "pkFigure.h"

namespace pkEngineSDK {

class PK_UTILITY_EXPORT OBB : public Figure
{
 public:
  OBB() = default;
  OBB(Matrix4 _transform, Vector3 _halfSize)
  : m_transform(_transform),
    m_halfSize(_halfSize) {};
  virtual ~OBB() = default;

  /**
   * @brief Get the support point of a shape in a direction.
   * @param _direction Direction to take the point from.
   * @return The final support point.
   */
  Vector3
  supportPoint(Vector3& _direction) override;

  Matrix4 m_transform;
  Vector3 m_halfSize;
};
}