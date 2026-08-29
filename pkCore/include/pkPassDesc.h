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
#include "pkShader.h"
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
  PK_SAM_STATE_ADRESS::E samAdress;
  PK_SAM_STATE_FILTERS::E samFilters;
  
  String name;
  // rasterizer state
  bool rSExists;
  RS_CULL_MODE::E rSCullMode;
  RS_FILL_MODE::E rSFillMode;
  bool rSFrontCounterClockwise;
  bool rSDepthClipEnable;
};

struct PixelPassDesc : public PassDesc
{
  WPtr<Shader> pVShader;
  WPtr<Shader> pPShader;

  Vector<SIZE_T> vSBuffers;
  Vector<SIZE_T> pSBuffers;

  Vector<WPtr<Texture>> pTex;
  Vector<WPtr<Texture>> vTex;

  Vector<WPtr<Texture>> outputs;

  SPtr<Texture> pDepth;
};

struct ComputePassDesc : public PassDesc
{
  WPtr<Shader> pCShader;
  Vector<SIZE_T> cBSizes;

  Vector<SPtr<Texture>> uavs;
  Vector<WPtr<Texture>> cTex;
};

struct GeometryPassDesc : public PassDesc
{
};
}
