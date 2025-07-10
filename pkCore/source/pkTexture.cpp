#include "pkTexture.h"

namespace pkEngineSDK
{

void
Texture::setSize(Vector2 _size)
{
  m_width = static_cast<uint32>(_size.x);
  m_height = static_cast<uint32>(_size.y);
}
}