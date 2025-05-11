/*****************************************************************************/
/**
 * @file    Obstacle.h
 * @author  Héctor  Iván Muñoz Ceballos
 * @date    08/02/2025
 * @brief Obstacle file for physics simulation.
 *
 * @bug Obstacle will "suck in" the projectiles most of the time.
 */
 /*****************************************************************************/
#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkActor.h"
#include "pkPrerequisitesCore.h"
#include "pkPlatformMath.h"
#include "pkSphere.h"

using pkEngineSDK::Actor;
using pkEngineSDK::Math;
using pkEngineSDK::Sphere;
using pkEngineSDK::SPtr;
using pkEngineSDK::Vector3;

class Obstacle
{
 public:
  Obstacle() = default;
  virtual ~Obstacle() = default;

  /**
   * @brief Start the obstacle object.
   * @param _pos Position of the object; x0 & y0 by default.
   * @param _radius Radius of the obstacle; 1.0f by default.
   * @param _bounciness How much energy an object loses when colliding with the object;
   *                    1.0f by default.
   * @param _pActor Actor of the obstacle; null by default.
   */
  void
  start(Vector3 _pos = Vector3(0.0f),
        float _radius = 1.0f,
        float _bounciness = 1.0f,
        SPtr<Actor> _pActor = nullptr);

  Sphere m_sphere;
  SPtr<Actor> m_actor;
  float m_bounciness;
};