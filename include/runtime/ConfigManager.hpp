#pragma once
#include "core/Object.hpp"
#include "core/Variable.hpp"
#include "runtime/Logger.hpp"
#include <string>
#include <unordered_map>
class ConfigManager : public Object {
private:
  std::string _configPath;
  std::unordered_map<std::string, std::unordered_map<std::string, Variable>>
      _configs;
  Logger *_logger = Logger::getLogger("ConfigManager");

private:
  void loadConfig(const std::string &ns, const std::string &name);
  std::string writeToml(const Variable &variable);

public:
  ConfigManager();
  bool hasConfig(const std::string &ns, const std::string &name);
  Variable &getConfig(const std::string &ns, const std::string &name);
  void saveConfig(const std::string &ns, const std::string &name);
};