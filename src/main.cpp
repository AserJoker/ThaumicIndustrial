#include "runtime/Application.hpp"
#ifdef __WIN32__
#include <windows.h>
#endif
auto main(int argc, char *argv[]) -> int {
#ifdef __WIN32__
  SetConsoleOutputCP(CP_UTF8);
#endif
  auto app = Application::getInstance();
  return app->run(argc, argv);
};