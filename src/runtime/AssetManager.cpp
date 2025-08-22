#include "runtime/AssetManager.hpp"
#include "core/Buffer.hpp"
#include "core/Object.hpp"
#include <SDL3/SDL_iostream.h>
#include <SDL3/SDL_log.h>
#include <filesystem>
#include <list>
#include <memory>
#include <vector>
bool AssetManager::resolve(const std::string &source, Identity &output) {
  std::string part;
  for (auto &ch : source) {
    if (ch == '.') {
      output.ns.push_back(part);
      part.clear();
    } else {
      part += ch;
    }
  }
  output.name = part;
  if (output.name.empty()) {
    return false;
  }
  return true;
}
bool AssetManager::initStore(const std::string &path) {
  if (!std::filesystem::is_directory(path)) {
    _logger->error("Failed to init asset store: {}", path);
    return false;
  }
  std::filesystem::path root = path;
  if (path.ends_with("/") || path.ends_with("\\")) {
    root = path.substr(0, path.size() - 1);
  }
  std::list<std::filesystem::path> workqueue = {root};
  while (!workqueue.empty()) {
    auto current = workqueue.back();
    workqueue.pop_back();
    if (std::filesystem::is_directory(current)) {
      for (auto &it : std::filesystem::directory_iterator(current)) {
        workqueue.push_front(it.path());
      }
    } else {
      std::string type = "unknown";
      std::string name = current.filename().string();
      if (current.has_extension()) {
        type = current.extension().string().substr(1);
        name = name.substr(0, name.size() - type.size() - 1);
      }
      std::shared_ptr<Object> asset;
      if (_loaders.contains(type)) {
        asset = _loaders.at(type)->load(current.string());
      } else {
        auto file = SDL_IOFromFile(current.string().c_str(), "r");
        if (file) {
          auto buf = std::make_shared<Buffer>();
          auto size = SDL_GetIOSize(file);
          buf->reset(size);
          SDL_ReadIO(file, buf->getData(), size);
          SDL_CloseIO(file);
          asset = buf;
        }
      }
      if (!asset) {
        _logger->error("Failed to load asset: {}", current.string());
        continue;
      }
      std::vector<std::string> ns;
      current = current.parent_path();
      while (current != root) {
        ns.push_back(current.filename().string());
        current = current.parent_path();
      }
      std::reverse(ns.begin(), ns.end());
      if (!store(ns, name, asset)) {
        return false;
      }
    }
  }
  return true;
}

void AssetManager::registerLoader(const std::string &type,
                                  const std::shared_ptr<AssetLoader> &loader) {
  _loaders[type] = loader;
}

bool AssetManager::store(const std::vector<std::string> &ns,
                         const std::string &name,
                         const std::shared_ptr<Object> asset) {
  if (!asset) {
    return false;
  }
  Node *node = &_root;
  for (auto &part : ns) {
    node = &node->children[part];
  }
  if (node->assets[name] != asset) {
    node->assets[name] = asset;
  }
  return true;
}

bool AssetManager::store(const std::string &fullname,
                         const std::shared_ptr<Object> asset) {
  Identity id;
  if (!resolve(fullname, id)) {
    return false;
  }
  return store(id.ns, id.name, asset);
}

const std::shared_ptr<Object> &
AssetManager::query(const std::vector<std::string> &ns,
                    const std::string &name) const {
  auto node = &_root;
  for (auto &part : ns) {
    if (!node->children.contains(part)) {
      return _notfound;
    }
    node = &node->children.at(part);
  }
  if (node->assets.contains(name)) {
    return node->assets.at(name);
  }
  return _notfound;
}

const std::shared_ptr<Object> &
AssetManager::query(const std::string &fullname) const {
  Identity id;
  if (!resolve(fullname, id)) {
    return _notfound;
  }
  return query(id.ns, id.name);
}
void AssetManager::reset() {
  _root.assets.clear();
  _root.children.clear();
}