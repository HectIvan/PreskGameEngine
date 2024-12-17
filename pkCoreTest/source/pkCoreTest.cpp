#include "pkBaseApp.h"

int
main(int argc, const char** argv)
{
  pkEngineSDK::BaseApp app;
  std::string modelName = "Grass_Block";
  std::string extension = "obj";
  app.init(argv, modelName, extension);
  return 0;
}