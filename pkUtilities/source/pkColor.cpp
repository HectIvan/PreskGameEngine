#include "pkColor.h"
#include "pkFColor.h"

namespace pkEngineSDK
{

const Color Color::WHITE =   Color(255, 255, 255, 0);
const Color Color::BLACK =   Color(0, 0, 0, 0);
const Color Color::RED =     Color(255, 0, 0, 0);
const Color Color::BLUE =    Color(0, 0, 255, 0);
const Color Color::YELLOW =  Color(255, 255, 0, 0);
const Color Color::GREEN =   Color(0, 255, 0, 0);
const Color Color::MAGENTA = Color(255, 0, 255, 0);
const Color Color::CYAN =    Color(0, 255, 255, 0);

FColor
Color::toFColor() const
{
  return FColor(static_cast<float>(_color.R) / 255.0f,
                static_cast<float>(_color.G) / 255.0f,
                static_cast<float>(_color.B) / 255.0f,
                static_cast<float>(_color.A) / 255.0f);
}
}
