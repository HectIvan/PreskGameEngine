/*****************************************************************************/
/**
 * @file    Obstacle.cpp
 * @author  Héctor  Iván Muñoz Ceballos
 * @date    08/02/2025
 * @brief Obstacle file for physics simulation.
 *
 * @bug    No known bugs.
 */
 /*****************************************************************************/

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "Obstacle.h"

void
Obstacle::start(Vector3 _pos, float _radius, float _bounciness, SPtr<Actor> _pActor)
{
  m_sphere.m_radius = _radius;
  m_sphere.m_origin = _pos;
  _pActor->setPosition(_pos);
  m_actor = _pActor;
  m_bounciness = _bounciness;
}