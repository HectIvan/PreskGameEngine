#include "pkColor.h"
#include "pkDX11Device.h"
#include "pkDX11Texture.h"

namespace pkEngineSDK
{

void
DX11Texture::create(DX11Device* _pDevice,
                    uint32 _width,
                    uint32 _height,
                    Vector<Color> _colors)
{
  // texture description
  D3D11_TEXTURE2D_DESC desc;
  desc.Width = _width;
  desc.Height = _height;
  desc.MipLevels = 1;
  desc.ArraySize = 1;
  desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  desc.SampleDesc.Count = 1;
  desc.SampleDesc.Quality = 0;
  desc.Usage = D3D11_USAGE_DEFAULT;
  desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
  desc.CPUAccessFlags = 0;
  desc.MiscFlags = 0;

  // data of the texture
  D3D11_SUBRESOURCE_DATA initData;
  initData.pSysMem = _colors.data();
  initData.SysMemPitch = static_cast<uint32>(_colors.size() * sizeof(Color));
  initData.SysMemSlicePitch = static_cast<uint32>(_colors.size());

  // create the texture
  ID3D11Texture2D* tex = nullptr;
  _pDevice->m_pd3dDevice->CreateTexture2D(&desc, &initData, &tex);
}

void
DX11Texture::clear()
{
  safeRelease(m_t2d);
  safeRelease(m_srv);
}
}