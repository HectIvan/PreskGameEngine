#include "pkMaterial.h"
#include "pkTexture.h"

namespace pkEngineSDK
{
void Material::init()
{
  m_diffuse = make_shared<Texture>();
  m_normal = make_shared<Texture>();
  m_height = make_shared<Texture>();
  m_metallic = make_shared<Texture>();
  m_roughness = make_shared<Texture>();
  m_emissive = make_shared<Texture>();
  m_occlusion = make_shared<Texture>();
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