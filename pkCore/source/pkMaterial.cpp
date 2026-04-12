#include "pkActor.h"
#include "pkMaterial.h"
#include "pkShader.h"
#include "pkTexture.h"
#include "pkTextureManager.h"

namespace pkEngineSDK
{
Material::Material(const String _name)
{
  TextureManager& tm = g_TextureManager();
  m_name = _name;
  m_castShadow = true;
  m_receiveShadows = true;
  m_properties = MaterialProps();

  setAlbedo(tm.m_defaultAlb);
  setNormal(tm.m_defaultNormal);
  setOclussion(tm.m_defaultAO);
  setHeight(tm.m_defaultHeight);
  setMetallic(tm.m_defaultMetallic);
  setRoughness(tm.m_defaultRough);
  setEmissive(tm.m_defaultEmissive);
}

void
Material::update(Actor&)
{
}

const Vector<WPtr<Texture>>
Material::getTextures()
{
  return { m_albedo, m_normal, m_oclussion, m_roughness, m_metallic, m_emissive };
}
}