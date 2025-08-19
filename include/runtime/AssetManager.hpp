#pragma once
#include "AssetLoader.hpp"
#include "core/Object.hpp"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
class AssetManager : public Object {
private:
  struct Node {
    std::unordered_map<std::string, Node> children;
    std::unordered_map<std::string, std::shared_ptr<Object>> assets;
  };
  std::unordered_map<std::string, Node> _assets;
  std::unordered_map<std::string, std::shared_ptr<AssetLoader>> _loaders;

  std::shared_ptr<Object> _notfound;

private:
  struct Identity {
    std::string type;
    std::vector<std::string> ns;
    std::string name;
  };

private:
  static bool resolve(const std::string &source, Identity &output);

public:
  bool initStore(const std::string &path);
  void registerLoader(const std::string &type,
                      const std::shared_ptr<AssetLoader> &loader);
  bool store(const std::string &type, const std::vector<std::string> &ns,
             const std::string &name, const std::shared_ptr<Object> asset);
  bool store(const std::string &fullname, const std::shared_ptr<Object> asset);
  const std::shared_ptr<Object> &query(const std::string &type,
                                       const std::vector<std::string> &ns,
                                       const std::string &name) const;
  const std::shared_ptr<Object> &query(const std::string &fullname) const;
  void reset();
};