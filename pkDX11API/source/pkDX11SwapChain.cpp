#include "pkDX11SwapChain.h"
#include "pkLogger.h"
#include "pkDX11Texture.h"
#include "pkGraphicsAPI.h"

namespace pkEngineSDK
{

void
DX11SwapChain::createRenderTargetView(SPtr<Device> _pDevice)
{
  for (uint32 i = 0; i < m_bufferCount; ++i)
  {
    // get buffer data
    ID3D11Texture2D* pBackBuffer = nullptr;
    // Get the buffers in the swap chain
    int32 hr = m_pSch->GetBuffer(i, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
    // if the buffer is not correct
    if (hr != 0x00000000) {
      String errMsg = g_Logger().getMessageError(hr);
      g_Logger().print("Failed to get the buffer to create a render target view. Error: " +
        errMsg);
      return;
    }

    // reinterpret as a directX texture
    SPtr<DX11Texture> rTargetView = make_shared<DX11Texture>(pBackBuffer);

    // create the render target view
    auto device = reinterpret_pointer_cast<DX11Device>(_pDevice);
    if (!device) {
      g_Logger().print("Failed to utilize the DX device in the render target view creation.");
    }
    hr = device->m_pd3dDevice->CreateRenderTargetView(pBackBuffer,
                                                      nullptr,
                                                      &rTargetView->m_rTVs[0]);
    // if the creation was not successful
    if (hr != 0x00000000) {
      String errMsg = g_Logger().getMessageError(hr);
      g_Logger().print("Failed to create a render target view. Error: " + errMsg);
      return;
    }

    m_buffers.push_back(rTargetView);
    pBackBuffer->Release();
  }

}

SPtr<Texture>&
DX11SwapChain::getBuffer(uint32 _index)
{
  return m_buffers[_index];
}
}
