#include "runtime/ConfigManager.hpp"
#include "core/Buffer.hpp"
#include "core/Variable.hpp"
#include "runtime/Application.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_iostream.h>
#include <filesystem>
#include <sstream>
#include <toml++/toml.hpp>
ConfigManager::ConfigManager() {
  auto app = Application::getInstance();
  _configPath = app->getCWD() + "configs/";
  if (!std::filesystem::exists(_configPath)) {
    std::filesystem::create_directory(_configPath);
  }
}
static void resolveToml(Variable &variable, toml::node &node) {
  if (node.is_string()) {
    variable.setString(node.as_string()->get());
  } else if (node.is_number()) {
    variable.setNumber(node.as_floating_point()->get());
  } else if (node.is_boolean()) {
    variable.setBoolean(node.as_boolean()->get());
  } else if (node.is_array() || node.is_array_of_tables()) {
    variable.setArray();
    auto arr = node.as_array();
    for (auto &item : *arr) {
      (*variable.getArray()).push_back({});
      resolveToml((*variable.getArray()).back(), item);
    }
  } else if (node.is_table()) {
    variable.setObject();
    for (auto &[key, value] : *node.as_table()) {
      resolveToml((*variable.getObject())[key.data()], value);
    }
  }
}
void writeToObject(const Variable &variable, toml::table &table);
void writeToArray(const Variable &variable, toml::array &array) {
  auto &arr = *variable.getArray();
  for (auto &value : arr) {
    switch (value.getType()) {
    case Variable::Type::NUMBER:
      array.push_back(*value.getNumber());
      break;
    case Variable::Type::STRING:
      array.push_back(*value.getString());
      break;
    case Variable::Type::BOOLEAN:
      array.push_back(*value.getBoolean());
      break;
    case Variable::Type::ARRAY: {
      toml::array arr;
      writeToArray(value, arr);
      array.push_back(arr);
    } break;
    case Variable::Type::OBJECT: {
      toml::table tlb;
      writeToObject(value, tlb);
      array.push_back(tlb);
      break;
    }
    default:
      break;
    }
  }
}
void writeToObject(const Variable &variable, toml::table &table) {
  auto &object = *variable.getObject();
  for (auto &[key, value] : object) {
    switch (value.getType()) {
    case Variable::Type::NUMBER:
      table.insert(key, *value.getNumber());
      break;
    case Variable::Type::STRING:
      table.insert(key, *value.getString());
      break;
    case Variable::Type::BOOLEAN:
      table.insert(key, *value.getBoolean());
      break;
    case Variable::Type::ARRAY: {
      toml::array arr;
      writeToArray(value, arr);
      table.insert(key, arr);
    } break;
    case Variable::Type::OBJECT: {
      toml::table tlb;
      writeToObject(value, tlb);
      table.insert(key, tlb);
    } break;
    default:
      break;
    }
  }
}

std::string ConfigManager::writeToml(const Variable &variable) {
  if (variable.getType() != Variable::Type::OBJECT) {
    _logger->error("Invalid config format, root node is not object");
    return "";
  }
  toml::table root;
  writeToObject(variable, root);
  std::stringstream ss;
  ss << root;
  return ss.str();
}
void ConfigManager::loadConfig(const std::string &ns, const std::string &name) {
  std::string path = _configPath + ns + "/" + name + ".toml";
  auto file = SDL_IOFromFile(path.c_str(), "r");
  if (!file) {
    _logger->warn("Failed to load config '{}.{}': {}", ns, name,
                  SDL_GetError());
    return;
  }
  Variable &variable = _configs[ns][name];
  size_t size = SDL_GetIOSize(file);
  Buffer buffer;
  buffer.reset(size);
  SDL_ReadIO(file, buffer.getData(), size);
  SDL_CloseIO(file);
  std::string content((const char *)buffer.getData(), size);
  try {
    auto config = toml::parse(content, path);
    resolveToml(variable, config);
  } catch (toml::parse_error &error) {
    _logger->error("Failed to parse config file: {}", error.what());
  }
}
bool ConfigManager::hasConfig(const std::string &ns, const std::string &name) {
  std::string path = _configPath + ns + "/" + name + ".toml";
  if (std::filesystem::exists(path) && !std::filesystem::is_directory(path)) {
    return true;
  }
  return false;
}

Variable &ConfigManager::getConfig(const std::string &ns,
                                   const std::string &name) {
  if (!_configs.contains(ns) || !_configs[ns].contains(name)) {
    loadConfig(ns, name);
  }
  return _configs[ns][name];
}

void ConfigManager::saveConfig(const std::string &ns, const std::string &name) {
  std::string nsPath = _configPath + ns + "/";
  std::string path = nsPath + name + ".toml";
  if (!std::filesystem::exists(nsPath)) {
    std::filesystem::create_directory(nsPath);
  }
  auto file = SDL_IOFromFile(path.c_str(), "w");
  if (!file) {
    _logger->error("Failed to save config '{}.{}': {}", ns, name,
                   SDL_GetError());
    return;
  }
  if (_configs.contains(ns) && _configs[ns].contains(name)) {
    std::string str = writeToml(_configs[ns][name]);
    SDL_WriteIO(file, str.c_str(), str.length());
  }
  SDL_CloseIO(file);
}