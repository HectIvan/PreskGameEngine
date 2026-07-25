#include "pkColor.h"
#include "pkFColor.h"

namespace pkEngineSDK
{

const FColor FColor::WHITE =   FColor(1.0f, 1.0f, 1.0f, 0.0f);
const FColor FColor::BLACK =   FColor(0.0f, 0.0f, 0.0f, 0.0f);
const FColor FColor::RED =     FColor(1.0f, 0.0f, 0.0f, 0.0f);
const FColor FColor::BLUE =    FColor(0.0f, 0.0f, 1.0f, 0.0f);
const FColor FColor::YELLOW =  FColor(1.0f, 1.0f, 0.0f, 0.0f);
const FColor FColor::GREEN =   FColor(0.0f, 1.0f, 0.0f, 0.0f);
const FColor FColor::MAGENTA = FColor(1.0f, 0.0f, 1.0f, 0.0f);
const FColor FColor::CYAN =    FColor(0.0f, 1.0f, 1.0f, 0.0f);

FColor::FColor(const Color& _color)
{
  r = static_cast<float>(_color._color.R) / 255.0f;
  g = static_cast<float>(_color._color.G) / 255.0f;
  b = static_cast<float>(_color._color.B) / 255.0f;
  a = static_cast<float>(_color._color.A) / 255.0f;
}

Color
FColor::toColor() const
{
  return Color(static_cast<uint8>(r * 255.0f),
               static_cast<uint8>(g * 255.0f),
               static_cast<uint8>(b * 255.0f),
               static_cast<uint8>(a * 255.0f));
}
}
