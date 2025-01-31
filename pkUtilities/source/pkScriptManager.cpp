#include "pkScriptManager.h"

namespace pkEngineSDK
{

void
ScriptManager::executeScript(const String& _script)
{
  m_state.script(_script);
}
}