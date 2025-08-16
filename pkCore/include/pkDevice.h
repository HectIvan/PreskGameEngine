/*****************************************************************************/
/**
 * @file    pkDevice.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    22/11/2024
 * @brief   Device base class for the engine.
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

namespace pkEngineSDK
{

class PK_CORE_EXPORT Device
{
 public:
  Device() = default;
  virtual ~Device() = default;

  /**
   * @brief Get the raw version of the device.
   * @return The pointer to the device.
   */
  virtual void*
  getDevice() = 0;

  /**
   * @brief Get the raw version of the device context.
   * @return The pointer to the device context.
   */
  virtual void*
  getDeviceContext() = 0;
};
}