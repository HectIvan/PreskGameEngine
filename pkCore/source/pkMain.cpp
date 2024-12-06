#include "pkWindow.h"
#include "pkWindowDesc.h"

int main()
{
  pkEngineSDK::Window window;
  pkEngineSDK::PKWindowDesc desc;
  desc.height = 1080;
  desc.width = 1920;
  desc.posX = 0;
  desc.posY = 0;
  window.create(desc, "test window");
}