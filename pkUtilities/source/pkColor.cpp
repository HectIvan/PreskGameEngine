#include "pkColor.h"

namespace pkEngineSDK
{

Vector4
Color::colorTo01()
{
  return Vector4(static_cast<float>(getR()) / 255.0f,
                 static_cast<float>(getG()) / 255.0f,
                 static_cast<float>(getB()) / 255.0f,
                 static_cast<float>(getA()) / 255.0f);
}
}
