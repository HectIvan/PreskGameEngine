#include "PhysicsApp.h"
#include "shaderTest.h"

int
main(int argc, const char** argv)
{
  PhysicsApp app;
  app.init(argv);
  app.messageLoop();
  return 0;
}