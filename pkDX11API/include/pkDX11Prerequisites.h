#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include <d3d11.h>
#include "pkMatrix4.h"
#include "pkPrerequisitesCore.h"
#include "pkVector4.h"

namespace pkEngineSDK {

struct CBView
{
  Matrix4 mView;
};

struct CBProjection
{
  Matrix4 mProjection;
};

struct CBWorld
{
  Matrix4 mWorld;
  Vector4 vMeshColor;

}