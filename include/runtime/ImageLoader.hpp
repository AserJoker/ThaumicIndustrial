#pragma once
#include "runtime/AssetLoader.hpp"
class ImageLoader : public AssetLoader {
public:
  std::shared_ptr<Object> load(const std::string &path) override;
};