#include "pkEventQueue.h"

#if PK_PLATFORM == PK_PLATFORM_WIN32
#include <Windows.h>

namespace pkEngineSDK
{

void
EventQueue::poll()
{
  // update key states
  for (uint32 i = 0; i < 256; ++i)
  {
    // update the key, if the entry doesnt exist, it creates a new entry
    keys[i].id = i;
    keys[i].pressed = GetAsyncKeyState(i) & 0x8000;

    if (keys[i].pressed)
    {
      uint32 j = 0;
      j -= 1;
    }
  }
  // update cursor position
  POINT curPos;
  GetCursorPos(&curPos);
  mousePosition = Vector2(curPos.x, curPos.y);
}

bool
EventQueue::iskeyPressed(KEY::E _key)
{
  // get the key
  Key tempKey = keys[_key];
  // return the state
  return tempKey.pressed;
}
}

#endif