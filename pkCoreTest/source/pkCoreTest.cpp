#include "pkBaseApp.h"

int
main(int argc, const char** argv)
{
  pkEngineSDK::BaseApp app;
  std::string modelName = "Emmisive Eye Class";
  std::string extension = "obj";
  app.init(argv, modelName, extension);
  return 0;
}