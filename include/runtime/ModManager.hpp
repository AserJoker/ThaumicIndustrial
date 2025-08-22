#pragma once
#include "core/Object.hpp"
#include "runtime/Logger.hpp"
#include <string>
#include <unordered_map>
#include <vector>
class ModManager : public Object {
public:
  struct ModInfo {
    std::string path;
    std::string name;
    std::string description;
    std::string version;
    std::string author;
    std::string license;
    std::vector<std::string> dependencies;
    std::vector<std::string> runtimeDependencies;
    std::string locale;
    bool ready = false;
  };

private:
  std::string _modPath;
  std::unordered_map<std::string, ModInfo> _mods;
  Logger *_logger = Logger::getLogger("ModManager");

private:
  bool loadModManifest(const std::string &path);
  bool checkCycleDependency(const ModInfo &info,
                            std::vector<std::string> &dependencies);
  bool loadMod(const ModInfo &info);

public:
  ModManager();
  bool hasMod(const std::string &name) const { return _mods.contains(name); }
  const ModInfo &getMod(const std::string &name) const {
    return _mods.at(name);
  }
  void addMod(const std::string &name, const ModInfo &info) {
    _mods[name] = info;
  }
  void removeMod(const std::string &name) { _mods.erase(name); }
  inline const std::unordered_map<std::string, ModInfo> &getMods() const {
    return _mods;
  }
  bool loadAllMod();
};