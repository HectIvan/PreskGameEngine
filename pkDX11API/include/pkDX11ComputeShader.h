/*****************************************************************************/
/**
 * @file    pkDX11ComputeShader.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    09/07/2025
 * @brief   This file contains the Compute Shader class using DirectX11 of the engine
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
#include "pkDX11Prerequisites.h"
#include "pkDX11Shader.h"

namespace pkEngineSDK
{

class DX11ComputeShader : public DX11Shader
{
public:
  DX11ComputeShader() = default;
  ~DX11ComputeShader() override {
    safeRelease(m_pShader);
  }

 public:
  ID3D11ComputeShader* m_pShader = nullptr;
};
}
