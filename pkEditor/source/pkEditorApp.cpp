#include "ShaderTest.h"

int
main(int argc, const char** argv)
{
  ShaderTest app;
  app.init(argv);
  app.messageLoop();
  return 0;
}