#include "pkLogger.h"
#include "shaderTest.h"

using pkEngineSDK::Exception;
using pkEngineSDK::g_Logger;
using pkEngineSDK::Logger;

int
main(int argc, const char** argv)
{
  ShaderTest app;
  Logger::startUp();
  Logger& log = g_Logger();
  log.init();

  try {
    app.init(argv, argc);
  }
  catch (const Exception& e) { // 
    log.print(e.what());
    log.createLogFiles();
    return 1;
  }
  catch (...) { // non handled exceptions
    log.print("Unhandled exception");
    log.createLogFiles();
    return 2;
  }
  app.messageLoop();
  log.createLogFiles();

  return 0;
}
