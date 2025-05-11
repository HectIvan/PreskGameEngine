#include "Spring.h"
#include "pkPlatformMath.h"
#include "pkLogger.h"

using pkEngineSDK::Math;
using pkEngineSDK::g_Logger;

void
Spring::move(float _deltaTime)
{
  // get the positions
  Vector3 weightPos = m_weight->m_transform.getTranslation3();
  Vector3 anchorPos = m_anchor->m_transform.getTranslation3();

  // get the directional vector between the 2 points
  Vector3 direction = weightPos - anchorPos;

  // get the length of the direction
  float length = direction.magnitude();

  // if the distance between anchor and weight is greater than the max stretch distance
  if (length > m_maxDistance) {
    g_Logger().print("weight out of range, resetting to max length");
    g_Logger().print(weightPos);
    Vector3 newWeightPos = direction.normalized() * m_maxDistance;
    newWeightPos += anchorPos;
    m_weight->setPosition(newWeightPos);

    weightPos = m_weight->m_transform.getTranslation3();
    anchorPos = m_anchor->m_transform.getTranslation3();
    // get the directional vector between the 2 points
    direction = weightPos - anchorPos;

    // get the length of the direction
    length = direction.magnitude();
  }

  // if the length is none
  if (length == 0.0f) { return; }

  // difference between current length and default length
  float displacement = length - m_length;
  // normalize the direction
  direction.normalize();
  
  // force to be applied
  float force = Math::hookeLaw(m_elasticity, displacement);

  // get the directional force with the force achieved in the hookes law
  Vector3 directionForce = direction * force;
  
  /**
   * Add a gravity force
   ***************************************/
  // gravity strength and direction
  Vector3 gravityForce = Vector3(0.0f, m_gravity * m_mass, 0.0f);

  // apply gravity to the movement direction
  directionForce += gravityForce;
  /*********************************/

  // accelerate & decelerate force
  Vector3 acceleration = directionForce / m_mass;

  // apply acceleration and drag to the weight
  m_velocity += acceleration * _deltaTime;
  m_velocity *= m_drag;
  // final position change
  weightPos += m_velocity * _deltaTime;

  // set the new position
  m_weight->m_transform.setTranslation(weightPos);
}

void
Spring::applyForce(Vector3 _dir, float _force, float _deltaTime)
{
  m_direction += _dir * _force * _deltaTime;
  m_direction.normalize();
}
