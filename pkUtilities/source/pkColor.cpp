#include "pkColor.h"

namespace pkEngineSDK
{

const Color Color::WHITE = Color(1, 1, 1, 0);
const Color Color::BLACK = Color(0, 0, 0, 0);
const Color Color::RED = Color(1, 0, 0, 0);
const Color Color::BLUE = Color(0, 0, 1, 0);
const Color Color::YELLOW = Color(1, 1, 0, 0);
const Color Color::GREEN = Color(0, 1, 0, 0);
const Color Color::MAGENTA = Color(1, 0, 1, 0);

Vector4
Color::colorTo01()
{
  return Vector4(static_cast<float>(getR()) / 255.0f,
                 static_cast<float>(getG()) / 255.0f,
                 static_cast<float>(getB()) / 255.0f,
                 static_cast<float>(getA()) / 255.0f);
}
}
