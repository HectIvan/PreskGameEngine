/*****************************************************************************/
/**
 * @file    pkPassDesc.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    11/06/2025
 * @brief   Pass description for the creation of passes.
 *
 * @bug    No known bugs.
 *
 * @changes  11/06/2025
 * Created the file
 */
 /*****************************************************************************/
#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkPrerequisitesCore.h"
#include "pkTexture.h"
#include "pkSamplerState.h"

/*********************************************/
/**
* Usings
**/
/*********************************************/
using TEXTURE_FORMAT::kPK_FORMAT_R32G32B32A32_FLOAT;
using PK_USAGE::kPK_USAGE_DEFAULT;

using PK_BIND_FLAG::kPK_BIND_DEPTH_STENCIL;
using PK_BIND_FLAG::kPK_BIND_RENDER_TARGET;
using PK_BIND_FLAG::kPK_BIND_SHADER_RESOURCE;
using PK_BIND_FLAG::kPK_BIND_UNORDERED_ACCESS;

using SAM_STATE_ADRESS::kClamp;
using SAM_STATE_ADRESS::kWrap;
using SAM_STATE_FILTERS::kFilterMigMagMipLinear;

namespace pkEngineSDK
{

struct PassDesc
{
  WString vSDirectory;
  WString pSDirectory;

  const char* vSEntry;
  const char* pSEntry;

  const char* vSModel;
  const char* pSModel;

  SAM_STATE_ADRESS::E samAdress;
  SAM_STATE_FILTERS::E samFilters;

  Vector<SIZE_T> cBSizes;
  Vector<SPtr<Texture>> inputs;
  Vector<SPtr<Texture>> outputs;
  SPtr<Texture> pDepth;
};
}
