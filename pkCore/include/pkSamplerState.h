/*****************************************************************************/
/**
 * @file    pkSamplerState.h
 * @author  Héctor  Iván Muñoz Ceballos
 * @date    04/03/2025
 * @brief   Sampler
 *
 * @bug    No known bugs.
 */
 /*****************************************************************************/
#pragma once

#include "pkPrerequisitesCore.h"

namespace SAM_STATE_FILTERS
{
enum PK_CORE_EXPORT E
{
  kFilterMigMagMipLinear = 21,
};
}

namespace SAM_STATE_ADRESS
{
enum PK_CORE_EXPORT E
{
  kWrap = 1,
  kMirror,
  kClamp,
  kBorder,
  kMirrorOnce
};
}

namespace pkEngineSDK
{

class PK_CORE_EXPORT SamplerState
{
 public:
  SamplerState() = default;
  virtual ~SamplerState() = default;
};
}