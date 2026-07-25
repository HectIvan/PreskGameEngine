#include "pkDX11SwapChain.h"
#include "pkLogger.h"
#include "pkDX11Texture.h"
#include "pkGraphicsAPI.h"
#include "pkDX11Device.h"

namespace pkEngineSDK
{

void
DX11SwapChain::createRenderTargetView(const SPtr<DX11Device>& _pDevice)
{
  for (uint32 i = 0; i < m_bufferCount; ++i) {
    // get buffer data
    D3DTexture2D* pBackBuffer = nullptr;
    D3D11_TEXTURE2D_DESC* tDesc = new D3D11_TEXTURE2D_DESC();
    // Get the buffers in the swap chain
    int32 hr = m_pSch->GetBuffer(i, __uuidof(D3DTexture2D), (LPVOID*)&pBackBuffer);
    // if the buffer is not correct
    if (PK_FAILED(hr)) {
      String msg = "Failed to get the buffer to create a render target view. Error: " +
                   LOG_GET_ERR_MSG(hr);
      LOG_FATAL(msg, __FILE__, __LINE__);
      THROW_ERROR(msg);
      return;
    }
    pBackBuffer->GetDesc(tDesc);

    // reinterpret as a directX texture
    SPtr<DX11Texture> rTargetView = make_shared<DX11Texture>(pBackBuffer);

    // create the render target view
    auto device = reinterpret_pointer_cast<DX11Device>(_pDevice);
    if (!device) {
      const String msg = "Failed to utilize the DX device in the render target view creation.";
      LOG_FATAL(msg, __FILE__, __LINE__);
      THROW_ERROR(msg);
      return;
    }
    hr = device->m_pd3dDevice->CreateRenderTargetView(pBackBuffer,
                                                      nullptr,
                                                      &rTargetView->m_rTVs[0]);
    // if the creation was not successful
    if (PK_FAILED(hr)) {
      const String msg = "Failed to create a render target view. Error: " + LOG_GET_ERR_MSG(hr);
      LOG_FATAL(msg, __FILE__, __LINE__);
      THROW_ERROR(msg);
      return;
    }

    rTargetView->setSize(Vector2(tDesc->Width, tDesc->Height));

    m_buffers.push_back(rTargetView);
    pBackBuffer->Release();
  }
}

void
DX11SwapChain::resizebuffers(const Vector2& _size)
{
  const uint32 hr = m_pSch->ResizeBuffers(m_bufferCount,
                                          toUint32(_size.x),
                                          toUint32(_size.y),
                                          DXGI_FORMAT_UNKNOWN,
                                          0);

  if (PK_FAILED(hr)) {
    const String msgErr = LOG_GET_ERR_MSG(hr);
    const String msg = "Failed to resize the swap chain buffers. Error: " + msgErr;
    LOG_ERROR(msg, __FILE__, __LINE__);
    return;
  }

  m_width = toUint32(_size.x);
  m_height = toUint32(_size.y);
}
}
