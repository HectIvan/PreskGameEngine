/*****************************************************************************/
/**
 * @file    GraphicsInspector.h
 * @author  Héctor Iván Muñoz Ceballos
 * @date    11/03/2026
 * @brief   Inspector for the Editor App graphics pannel.
 *
 * @bug     No known bugs.
 */
 /*****************************************************************************/
#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkWindow.h"

using pkEngineSDK::Window;

class GraphicsInspector
{
 public:
  GraphicsInspector() = default;
  virtual ~GraphicsInspector() = default;

  static void
  init(Window& _window,
       bool& _ibl,
       float& _iblInt,
       float& _exp,
       float& _blurRad,
       float& _blurStrength,
       float& _lumThreshold,
       float& _emissRad,
       float& _emissStr,
       bool& _ssao,
       float& _ssaoRad,
       float& _ssaoScale,
       float& _ssaoBias,
       float& _ssaoInt);
};
