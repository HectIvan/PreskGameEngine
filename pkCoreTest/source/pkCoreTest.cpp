#include "pkBaseApp.h"

int
main(int argc, const char** argv)
{
  pkEngineSDK::BaseApp app;
  std::string modelName = "Emmisive_Eye_Class";
  std::string extension = "obj";
  app.init(argv, modelName, extension);
  return 0;
}