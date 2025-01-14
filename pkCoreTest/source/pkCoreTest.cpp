#include "testApp.h"

int
main(int argc, const char** argv)
{
  TestApp app;
  app.init(argv);
  app.messageLoop();
  return 0;
}