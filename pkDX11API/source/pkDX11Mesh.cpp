#include "pkDX11Device.h"
#include "pkDX11Mesh.h"
#include "pkDX11Texture.h"

namespace pkEngineSDK
{

void
DX11Mesh::render(Device* _pDevice)
{
  if (DX11Device* deviceX = dynamic_cast<DX11Device*>(_pDevice->getDevice().get()))
  {
    uint32 offset = 0;
    for (uint32 i = 0; i < m_textures.size(); ++i)
    {
      if (DX11Texture* textureX = dynamic_cast<DX11Texture*>(m_textures[i]->getTexture().get()))
      {
        if (textureX[i].getType() == aiTextureType::aiTextureType_DIFFUSE)
        {
          //NOTE: have to figure out how to turn this so that it works with a generic Device
          deviceX->m_pImmediateContext->PSSetShaderResources(0, 1, textureX[i].getSRV());
        }
      }
    }
  }
}