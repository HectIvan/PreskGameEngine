#include "pkMaterial.h"
#include "pkTexture.h"
#include "pkTextureManager.h"

namespace pkEngineSDK
{
Material::Material()
{
  TextureManager& tm = g_TextureManager();
  m_name = "";
  m_castShadow = true;
  m_receiveShadows = true;
  m_properties = MaterialProps();

  setDiffuse(tm.m_defaultDiff);
  setNormal(tm.m_defaultNormal);
  setOcclusion(tm.m_defaultAO);
  setHeight(tm.m_defaultHeight);
  setMetallic(tm.m_defaultMetallic);
  setRoughness(tm.m_defaultRough);
  setEmissive(tm.m_defaultEmissive);
}

void
Material::init()
{
  TextureManager& tm = g_TextureManager();
  m_name = "";
  m_castShadow = true;
  m_receiveShadows = true;
  m_properties = MaterialProps();

  setDiffuse(tm.m_defaultDiff);
  setNormal(tm.m_defaultNormal);
  setOcclusion(tm.m_defaultAO);
  setHeight(tm.m_defaultHeight);
  setMetallic(tm.m_defaultMetallic);
  setRoughness(tm.m_defaultRough);
  setEmissive(tm.m_defaultEmissive);
}

void
Material::setTexture(SPtr<Texture>& _pTexture, SPtr<Texture>& _pNewTexture)
{
  _pTexture = _pNewTexture;
}
}