#pragma once
#include "core/Object.hpp"
#include <memory>
class AssetLoader : public Object {
public:
  virtual std::shared_ptr<Object> load(const std::string &path) = 0;
};