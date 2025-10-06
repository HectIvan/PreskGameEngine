#include "pkColor.h"
#include "pkDX11Device.h"
#include "pkDX11Texture.h"
#include "pkLogger.h"

namespace pkEngineSDK
{

bool
DX11Texture::copyFrom(SPtr<Texture>& _pTexture)
{
  Logger& log = g_Logger().instance();
  // comvert to DirectX Texture.
  const auto dxTx = reinterpret_pointer_cast<DX11Texture>(_pTexture);
  if (!dxTx) {
    const String msg = "Failed to copy texture: '" + _pTexture->getName().toString() + "'.";
    log.registerMessage(msg, LOG_MSG_TYPE::kWarning);
    return false;
  }
  // copy the values.
  setSize(dxTx->getSize());
  setName(dxTx->getName());

  if (m_t2d) { m_t2d = dxTx->getTexture2D(); }
  if (m_sRV) { m_sRV = dxTx->getSRV(); };
  if (!m_uAVs.empty()) { m_uAVs = dxTx->getUAVs(); };
  if (!m_rTVs.empty()) { m_rTVs = dxTx->getRTVs(); };

  return true;
}
}