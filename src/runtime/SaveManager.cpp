#include "runtime/SaveManager.hpp"
#include "runtime/Application.hpp"
#include <filesystem>
SaveManager::SaveManager() {
  auto app = Application::getInstance();
  _savePath = app->getCWD() + "saves/";
  if (!std::filesystem::exists(_savePath)) {
    std::filesystem::create_directory(_savePath);
  }
}