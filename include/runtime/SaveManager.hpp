#pragma once
#include "core/Object.hpp"
#include "runtime/Logger.hpp"
#include <string>
class SaveManager : public Object {
private:
  std::string _savePath;
  Logger *_logger = Logger::getLogger("SaveManager");

public:
  SaveManager();
};