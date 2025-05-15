#include "pkImGui.h"
#include "pkDX11GraphicsAPI.h"

namespace pkEngineSDK
{

bool
DX11_Init(ID3D11Device* device, ID3D11DeviceContext* device_context)
{
  device = device;
  device_context = device_context;
  return false;
}

void
// ImguiEngine::APIInit()
// {
//   // get the api
//   GraphicsAPI& api = GraphicsAPI::instance();
//   SPtr<GraphicsAPI> pointerApi = make_shared<GraphicsAPI>(api);
//   auto dxAPI = reinterpret_pointer_cast<DX11GraphicsAPI>(pointerApi);
// 
//   // get a pointer to te api device
//   SPtr<DX11Device> dxDevice = dxAPI->getDevice();
// }
}