/*****************************************************************************/
/**
 * @file    pkActorResource.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    05/12/2025
 * @brief   Resource for engine actors.
 *
 * @bug    No known bugs.
 */
 /*****************************************************************************/
#pragma once

/*********************************************/
/**
 * Includes
 */
 /*********************************************/
#include "pkBaseResource.h"

namespace pkEngineSDK
{

class PK_CORE_EXPORT ActorResource : public BaseResource
{
 public:
  ActorResource() = default;
  virtual ~ActorResource() = default;

  /**
   * @brief Load the actor resource.
   */
  void
  load() override;

  /**
   * @brief Unload the actor resource.
   */
  void
  unload() override;

  /**
   * @brief Get the resource type.
   * @return The resource type.
   */
  RESOURCE_TYPE::E
  getType() const override { return RESOURCE_TYPE::kActor; }

  /**
   * @brief Get the resource type as a string.
   * @return The resource type string.
   */
  String
  getTypeString() const override { return "Actor"; }

 public:
  Vector3 m_position;
  Vector3 m_rotation;
  Vector3 m_scale;
  bool m_isActive;
  uint32 m_componentCount;
};
}
