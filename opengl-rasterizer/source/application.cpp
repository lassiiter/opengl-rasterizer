#include "pch.h"

#include "application.h"

#include "window/gl_window.h"

Application::Application(const std::string& app_name)
{
  mWindow = std::make_unique<nwindow::GLWindow>();
  mWindow->init(1024, 720, app_name);

}

void Application::loop()
{
  while (mWindow->is_running())
  {
    mWindow->render();
  }
}