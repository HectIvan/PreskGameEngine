#include "PhysicsApp.h"
#include "pkPrerequisitesCore.h"
#include "ShaderTest.h"

using pkEngineSDK::int32;

int
main(int argc, const char** argv)
{
  ShaderTest app;
  app.init(argv, static_cast<int32>(argc));
  app.messageLoop();
  return 0;
}