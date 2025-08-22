#pragma once
#include "AssetLoader.hpp"
#include "core/Object.hpp"
#include "runtime/Logger.hpp"
#include <memory>
class JsonLoader : public AssetLoader {
private:
  Logger *_logger = Logger::getLogger("JsonLoader");

public:
  std::shared_ptr<Object> load(const std::string &path) override;
};