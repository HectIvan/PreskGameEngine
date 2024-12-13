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
  Matrix4 view;
};

struct CBProjection
{
  Matrix4 projection;
};

struct CBWorld
{
  Matrix4 world;
  Vector4 meshColor;

};
}