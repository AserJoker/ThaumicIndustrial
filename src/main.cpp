#include "runtime/Application.hpp"
auto main(int argc, char *argv[]) -> int {
  auto &app = Application::getInstance();
  return app.run(argc, argv);
};