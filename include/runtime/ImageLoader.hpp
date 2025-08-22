#pragma once
#include "runtime/AssetLoader.hpp"
#include "runtime/Logger.hpp"
class ImageLoader : public AssetLoader {
private:
  Logger *_logger = Logger::getLogger("ImageLoader");
public:
  std::shared_ptr<Object> load(const std::string &path) override;
};