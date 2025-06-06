#include "ShaderTest.h"

int
main(int argc, const char** argv)
{
  ShaderTest app;
  app.init(argv, argc);
  app.messageLoop();
  return 0;
}
