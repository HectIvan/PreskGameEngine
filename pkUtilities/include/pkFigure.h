/*************************************************************************/
/**
 * @file    pkFigure.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    05/06/2025
 * @brief   Basic figure from which the rest of shapes will inherit.
 *
 * @bug    no knon bugs.
 */
 /*****************************************************************************/
#pragma once

#include "pkPrerequisitesUtilities.h"
#include "pkVector3.h"

namespace pkEngineSDK
{

class PK_UTILITY_EXPORT Figure
{
 public:
  Figure() = default;
  virtual ~Figure() = default;

  /**
   * @brief Get the support point of a shape in a direction.
   * @param _direction Direction to take the point from.
   * @return The final support point.
   */
  virtual Vector3
  supportPoint(Vector3& _direction) = 0;
};
}