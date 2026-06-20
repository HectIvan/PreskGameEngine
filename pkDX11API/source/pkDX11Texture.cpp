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

  uint32 nameSize = 0;
  PKRESULT hr = m_t2d->GetPrivateData(WKPDID_D3DDebugObjectName, &nameSize, nullptr);

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
}
