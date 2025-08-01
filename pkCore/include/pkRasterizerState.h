/*****************************************************************************/
/**
 * @file    pkRasterizerstate.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    10/07/2025
 * @brief   Rasterizer state for the engine.
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

namespace RS_FILL_MODE
{
  enum E
  {
    kPK_FILL_WIREFRAME = 2,
    kPK_FILL_SOLID = 3
  };
}

namespace RS_CULL_MODE {
  enum E
  {
    kPK_CULL_NONE = 1,
    kPK_CULL_FRONT = 2,
    kPK_CULL_BACK = 3
  };
}

struct RASTERIZER_DESC
{
  RS_FILL_MODE::E fillMode;
  RS_CULL_MODE::E cullMode;
  bool frontCounterClockwise;
  int32 depthBias;
  float depthBiasClamp;
  float slopeScaledDepthBias;
  bool depthClipEnable;
  bool scissorEnable;
  bool multisampleEnable;
  bool antialiasedLineEnable;
};

class PK_CORE_EXPORT RasterizerState
{
 public:
  RasterizerState() = default;
  virtual ~RasterizerState() = default;
};
}
