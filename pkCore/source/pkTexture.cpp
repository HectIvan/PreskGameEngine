#include "pkTexture.h"

namespace pkEngineSDK
{

void
Texture::setSize(Vector2 _size)
{
  m_width = toUint32(_size.x);
  m_height = toUint32(_size.y);
}
}