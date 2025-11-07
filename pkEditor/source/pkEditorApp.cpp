#include "shaderTest.h"
#include <iostream>
int
main(int argc, const char** argv)
{
  ShaderTest app;
  try {
    app.init(argv, argc);
  }
  catch (const std::exception& e) {
    std::cout << e.what() << std::endl;
  }
  app.messageLoop();
  return 0;
}
