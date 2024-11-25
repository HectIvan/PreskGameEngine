#include "pkMesh.h"
#include "pkDevice.h"

namespace pkEngineSDK
{

void
Mesh::draw(Device* _pDevice)
{
  uint32 offset = 0;
  for (uint32 i = 0; i < m_textures.size(); ++i)
  {
    if (m_textures[i].GetType() == aiTextureType::aiTextureType_DIFFUSE)
    {
      _pDevice->m_pImmediateContext->PSSetShaderResources(0, 1, &m_textures[i].m_srv);
    }
  }
}
}