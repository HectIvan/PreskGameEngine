/*****************************************************************************/
/**
 * @file    pkInputLayoutDesc.h
 * @author  Héctor  Iván Muñoz Ceballos
 * @date    19/11/2024
 * @brief   Description
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
#include "pkGraphicTypes.h"

namespace INPUT_LAYOUT
{
  enum PK_CORE_EXPORT E
  {
    kPosition = 0,
    kNormal,
    kTexCoord,
    kTangent,
    kBinormal,
    kColor
  };
}

namespace pkEngineSDK
{
 
struct PK_CORE_EXPORT InputDesc
{
  INPUT_LAYOUT::E type;
  TEXTURE_FORMATS::E format;
  uint32 size;
};
}