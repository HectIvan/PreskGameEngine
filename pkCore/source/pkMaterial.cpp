#include "pkMaterial.h"
#include "pkTexture.h"

namespace pkEngineSDK
{
void Material::init()
{
  diffuse = make_shared<Texture>();
  normal = make_shared<Texture>();
  height = make_shared<Texture>();
  metallic = make_shared<Texture>();
  occlusion = make_shared<Texture>();
  m_name = "";
  m_castShadow = true;
  m_receiveShadows = true;
  m_properties = MaterialProps();
}

void
Material::setTexture(SPtr<Texture>& _pTexture, SPtr<Texture>& _pNewTexture)
{
  _pTexture = _pNewTexture;
}
}