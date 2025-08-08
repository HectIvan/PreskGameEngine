/*****************************************************************************/
/**
 * @file    pkConstantBuffer.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    13/12/2024
 * @brief   Constant Buffer class for the engine.
 *
 * @bug    No known bugs.
 */
 /*****************************************************************************/
#pragma once

namespace pkEngineSDK
{

class Device;

class ConstantBuffer
{
 public:
  ConstantBuffer() = default;
  virtual ~ConstantBuffer() = default;
};
}