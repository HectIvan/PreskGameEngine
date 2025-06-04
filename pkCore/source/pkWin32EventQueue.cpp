#include "pkEventQueue.h"
#include "pkWindow.h"

#if PK_PLATFORM == PK_PLATFORM_WIN32
#include <Windows.h>

namespace pkEngineSDK
{

void
EventQueue::poll()
{
  // update key states
  for (uint32 i = 0; i < 256; ++i) {
    // update the key, if the entry doesnt exist, it creates a new entry
    keys[i].id = i;
    keys[i].pressed = GetAsyncKeyState(i) & 0x8000;

    if (keys[i].pressed) {
      uint32 j = 0;
      j -= 1;
    }
  }
  // update cursor position
  POINT curPos;
  GetCursorPos(&curPos);
  mousePosition = Vector2(static_cast<float>(curPos.x),
                          static_cast<float>(curPos.y));
}

bool
EventQueue::iskeyPressed(KEY::E _key)
{
  // get the key
  Key tempKey = keys[_key];
  // return the state
  return tempKey.pressed;
}

void
EventQueue::windowInput(Window& _window)
{
  MSG msg = { 0 };
  if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  _window.m_isFocused = GetFocus() == _window.getWindowHandle();
}
}
#endif