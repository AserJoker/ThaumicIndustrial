#pragma once
#include "core/Object.hpp"
#include <string>
class SaveManager : public Object {
private:
  std::string _savePath;

public:
  SaveManager();
};