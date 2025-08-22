#pragma once
#include "AssetLoader.hpp"
#include "core/Object.hpp"
#include "runtime/Logger.hpp"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
class AssetManager : public Object {

private:
  struct Identity {
    std::vector<std::string> ns;
    std::string name;
  };
  struct Node {
    std::unordered_map<std::string, Node> children;
    std::unordered_map<std::string, std::shared_ptr<Object>> assets;
  };

private:
  Node _root;
  std::unordered_map<std::string, std::shared_ptr<AssetLoader>> _loaders;
  std::shared_ptr<Object> _notfound;

  Logger *_logger = Logger::getLogger("AssetManager");

private:
  static bool resolve(const std::string &source, Identity &output);

public:
  bool initStore(const std::string &path);
  void registerLoader(const std::string &type,
                      const std::shared_ptr<AssetLoader> &loader);
  bool store(const std::vector<std::string> &ns, const std::string &name,
             const std::shared_ptr<Object> asset);
  bool store(const std::string &fullname, const std::shared_ptr<Object> asset);
  const std::shared_ptr<Object> &query(const std::vector<std::string> &ns,
                                       const std::string &name) const;
  const std::shared_ptr<Object> &query(const std::string &fullname) const;
  void reset();
};