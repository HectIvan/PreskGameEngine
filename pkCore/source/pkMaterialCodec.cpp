/*****************************************************************************/
/**
 * @file    pkMaterialCodec.cpp
 * @author  Héctor Iván Muñoz Ceballos
 * @date    27/10/2025
 * @brief   Codec for saving materials.
 *
 * @bug    No known bugs.
 */
 /*****************************************************************************/

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkMaterialCodec.h"
#include "pkMaterial.h"
#include "pkMaterialResource.h"
#include "pkTexture.h"
#include "pkPath.h"

namespace pkEngineSDK
{

MaterialResource*
MaterialCodec::createResource(const SPtr<Material> _pMaterial)
{
  MaterialResource* matResource = new MaterialResource();
  matResource->m_diffusePath = _pMaterial->m_diffuse->getName().toString();

  return matResource;
}

PK_CORE_EXPORT MaterialCodec&
g_MaterialCodec()
{
  return MaterialCodec::instance();
}
}