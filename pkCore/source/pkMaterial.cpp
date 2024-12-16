#include "pkMaterial.h"
#include "pkTexture.h"

namespace pkEngineSDK
{
void
Material::setTexture(SPtr<Texture>& _pTexture, SPtr<Texture>& _pNewTexture)
{
  _pTexture = _pNewTexture;
}
}