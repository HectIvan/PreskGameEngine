#include "pkColor.h"
#include "pkDX11Device.h"
#include "pkDX11Texture.h"
#include "pkLogger.h"

namespace pkEngineSDK
{
void
DX11Texture::setName(const String& _name)
{
  m_t2d->SetPrivateData(WKPDID_D3DDebugObjectName,
                        static_cast<UINT>(_name.size()),
                        _name.c_str());
}

const ANSICHAR*
DX11Texture::getName() const
{
  if (!m_t2d) { return ""; }

  uint32 nameSize = 0;
  String name = "";
  PKRESULT hr = m_t2d->GetPrivateData(WKPDID_D3DDebugObjectName, &nameSize, nullptr);

  if (PK_FAILED(hr)) {
    const String errMsg = LOG_GET_ERR_MSG(hr);
    const String msg = "Failed to get the texture name size. Error message: " + errMsg;
    LOG_ERROR(msg, __FILE__, __LINE__);
    return name.c_str();
  }

  Vector<ANSICHAR> nameVec(nameSize);
  hr = m_t2d->GetPrivateData(WKPDID_D3DDebugObjectName, &nameSize, nameVec.data());

  if (PK_FAILED(hr)) {
    const String errMsg = LOG_GET_ERR_MSG(hr);
    const String msg = "Failed to get the texture name. Error message: " + errMsg;
    LOG_ERROR(msg, __FILE__, __LINE__);
    return name.c_str();
  }
  name = String(nameVec.data(), nameSize);
  return name.c_str();
}
void
DX11Texture::copyFromReference(SPtr<Texture>& _pTexture)
{
  if (!_pTexture) {
    LOG_ERROR("Texture is null!", __FILE__, __LINE__);
    return;
  }

  auto d11Tex = reinterpret_pointer_cast<DX11Texture>(_pTexture);

  PK_ASSERT(d11Tex && "Texture is not a directX 11 Texture.");

  m_id = _pTexture->getID();
  m_width = _pTexture->getWidth();
  m_height = _pTexture->getHeight();

  m_t2d = d11Tex->m_t2d;
  m_rTVs = d11Tex->m_rTVs;
  m_dSV = d11Tex->m_dSV;
  m_sRV = d11Tex->m_sRV;
  m_uAVs = d11Tex->m_uAVs;
}
}
