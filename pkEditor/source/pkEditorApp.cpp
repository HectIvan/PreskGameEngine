#include "PhysicsApp.h"

int
main(int argc, const char** argv)
{
  PhysicsApp app;
  app.init(argv, argc);
  app.messageLoop();
  return 0;
}
