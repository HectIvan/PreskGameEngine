#include "pkColor.h"
#include "pkDX11Device.h"
#include "pkDX11Texture.h"
#include "pkLogger.h"

namespace pkEngineSDK
{

bool
DX11Texture::copyFrom(SPtr<Texture>& _pTexture)
{
  if (!_pTexture) {
    return false;
  }
  Logger& log = g_Logger();
  // comvert to DirectX Texture.
  const auto dxTx = reinterpret_pointer_cast<DX11Texture>(_pTexture);
  if (!dxTx) {
    const String msg = "Failed to copy texture: '" + _pTexture->getNameString() + "'.";
    log.registerMessage(msg, __FILE__, __LINE__, LOG_MSG_TYPE::kWarning);
    return false;
  }
  // copy the values.
  setSize(dxTx->getSize());
  setName(dxTx->getName());

  if (dxTx->m_t2d) { m_t2d = dxTx->getTexture2D(); }
  if (dxTx->m_sRV) { m_sRV = dxTx->getSRV(); };
  if (!dxTx->m_uAVs.empty()) { m_uAVs = dxTx->getUAVs(); };
  if (!dxTx->m_rTVs.empty()) { m_rTVs = dxTx->getRTVs(); };

  return true;
}
}