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


namespace pkEngineSDK
{

namespace PK_SAM_STATE_FILTERS
{
  enum E
  {
    kFilterMigMagMipLinear = 21,
  };
}

namespace PK_SAM_STATE_ADRESS
{
  enum E
  {
    kWrap = 1,
    kMirror,
    kClamp,
    kBorder,
    kMirrorOnce
  };
}

class PK_CORE_EXPORT SamplerState
{
 public:
  SamplerState() = default;
  virtual ~SamplerState() = default;
};
}