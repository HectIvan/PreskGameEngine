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
#include "pkShaderKey.h"

/*********************************************/
/**
* Usings
**/
/*********************************************/
using pkEngineSDK::PK_GRAPHICS_FORMAT::kPK_FORMAT_R32G32B32A32_FLOAT;
using pkEngineSDK::PK_RESOURCE_USAGE::kUSAGE_DEFAULT;

using pkEngineSDK::PK_BIND_FLAG::kPK_BIND_DEPTH_STENCIL;
using pkEngineSDK::PK_BIND_FLAG::kPK_BIND_RENDER_TARGET;
using pkEngineSDK::PK_BIND_FLAG::kPK_BIND_SHADER_RESOURCE;
using pkEngineSDK::PK_BIND_FLAG::kPK_BIND_UNORDERED_ACCESS;

using pkEngineSDK::PK_SAM_STATE_ADRESS::kClamp;
using pkEngineSDK::PK_SAM_STATE_ADRESS::kWrap;
using pkEngineSDK::PK_SAM_STATE_FILTERS::kFilterMigMagMipLinear;

namespace pkEngineSDK
{

struct PassDesc
{
  Vector<SIZE_T> cBSizes;
  
  PK_SAM_STATE_ADRESS::E samAdress;
  PK_SAM_STATE_FILTERS::E samFilters;
  
  Vector<WPtr<Texture>> inputs;
  Vector<WPtr<Texture>> outputs;
  String name;
  // rasterizer state
  bool rSExists;
  RS_CULL_MODE::E rSCullMode;
  RS_FILL_MODE::E rSFillMode;
  bool rSFrontCounterClockwise;
  bool rSDepthClipEnable;
};

struct PixelDesc : public PassDesc
{
  ShaderKey vSKey;
  ShaderKey pSKey;

  SPtr<Texture> pDepth;
};

struct ComputeDesc : public PassDesc
{
  ShaderKey cSKey;
  Vector<SIZE_T> cBSizes;

  Vector<SPtr<Texture>> uavs;
};

struct GeometryPass : public PassDesc
{
};
}
