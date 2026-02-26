#include "pkLogger.h"
#include "EditorApp.h"

using pkEngineSDK::Exception;
using pkEngineSDK::g_Logger;
using pkEngineSDK::Logger;

int
main(int argc, const char** argv)
{
  EditorApp app;
  Logger::startUp();
  Logger& log = g_Logger();

  try {
    app.init(argv, argc);
  }
  catch (const Exception& e) { // handled exceptions.
    LOG_PRINT(e.what());
    log.createLogFiles();
    return 1;
  }
  catch (...) { // non handled exceptions.
    LOG_PRINT("Unhandled exception");
    log.createLogFiles();
    return 2;
  }
  app.messageLoop();
  log.createLogFiles();

  return 0;
}
