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
#include "pkRasterizerState.h"
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
  Path vSDirectory;
  Path pSDirectory;
  Path cSDirectory;

  const char* vSEntry;
  const char* pSEntry;
  const char* cSEntry;

  const char* vSModel;
  const char* pSModel;
  const char* cSModel;

  SAM_STATE_ADRESS::E samAdress;
  SAM_STATE_FILTERS::E samFilters;

  // rasterizer state
  bool rSExists;
  RS_CULL_MODE::E rSCullMode;
  RS_FILL_MODE::E rSFillMode;
  bool rSFrontCounterClockwise;
  bool rSDepthClipEnable;

  Vector<SIZE_T> cBSizes;
  Vector<SPtr<Texture>> inputs;
  Vector<SPtr<Texture>> outputs;
  Vector<SPtr<Texture>> uavs;
  SPtr<Texture> pDepth;
};
}
