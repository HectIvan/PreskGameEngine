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

String
DX11Texture::getName()
{
  if (!m_t2d) { return String(); }

  UINT nameSize = 0;
  HRESULT hr = m_t2d->GetPrivateData(WKPDID_D3DDebugObjectName, &nameSize, nullptr);

  if (PK_FAILED(hr)) {
    const String errMsg = LOG_GET_ERR_MSG(hr);
    const String msg = "Failed to get the texture name size. Error message: " + errMsg;
    LOG_ERROR(msg, __FILE__, __LINE__);
    return String();
  }

  Vector<ANSICHAR> nameVec(nameSize);
  hr = m_t2d->GetPrivateData(WKPDID_D3DDebugObjectName, &nameSize, nameVec.data());

  if (PK_FAILED(hr)) {
    const String errMsg = LOG_GET_ERR_MSG(hr);
    const String msg = "Failed to get the texture name. Error message: " + errMsg;
    LOG_ERROR(msg, __FILE__, __LINE__);
    return String();
  }

  return String(nameVec.data(), nameSize);
}

bool
DX11Texture::copyFrom(SPtr<Texture>& _pTexture)
{
  if (!_pTexture) {
    return false;
  }
  // comvert to DirectX Texture.
  const auto dxTx = reinterpret_pointer_cast<DX11Texture>(_pTexture);
  if (!dxTx) {
    const String msg = "Failed to copy texture";
    LOG_WARNING(msg, __FILE__, __LINE__);
    return false;
  }
  // copy the values.
  setSize(dxTx->getSize());

  if (dxTx->m_t2d) { m_t2d = dxTx->getTexture2D(); }
  if (dxTx->m_sRV) { m_sRV = dxTx->getSRV(); };
  if (!dxTx->m_uAVs.empty()) { m_uAVs = dxTx->getUAVs(); };
  if (!dxTx->m_rTVs.empty()) { m_rTVs = dxTx->getRTVs(); };

  return true;
}
}