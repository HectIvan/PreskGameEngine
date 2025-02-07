#include "PhysicsApp.h"
#include "ShaderTest.h"

int
main(int argc, const char** argv)
{
  PhysicsApp app;
  app.init(argv);
  app.messageLoop();
  return 0;
}