#pragma once

#include "pkPrerequisitesCore.h"
#include "pkWindow.h"

namespace pkEngineSDK
{

class PK_CORE_EXPORT BaseApp
{
 public:
  BaseApp() = default;
  virtual ~BaseApp() = default;

  /**
  * Initialize the app.
  **/
  void
  init(const char** _argv);

  /**
  * Initialize the app window.
  **/
  void
  initWindow();

  /**
  * Initialize the api.
  **/
  void
  initAPI(const char** _argv);

public:
  Window window;
};
}