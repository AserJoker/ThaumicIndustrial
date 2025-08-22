#include "runtime/ModManager.hpp"
#include "core/Buffer.hpp"
#include "runtime/Application.hpp"
#include <SDL3/SDL_iostream.h>
#include <cjson/cjson.h>
#include <filesystem>
#include <string>
#include <vector>
ModManager::ModManager() {
  auto app = Application::getInstance();
  _modPath = app->getCWD() + "mods/";
  if (!std::filesystem::exists(_modPath)) {
    std::filesystem::create_directory(_modPath);
  }
}
bool ModManager::checkCycleDependency(const ModInfo &info,
                                      std::vector<std::string> &dependencies) {
  for (auto &dep : info.dependencies) {
    if (std::find(dependencies.begin(), dependencies.end(), dep) !=
        dependencies.end()) {
      _logger->error("Cycle dependency detected: ");
      for (auto &dep : dependencies) {
        _logger->error("  {} ->", dep);
      }
      _logger->error("  {}", dep);
      return false;
    }
    dependencies.push_back(dep);
    if (_mods.contains(dep)) {
      auto &depInfo = _mods.at(dep);
      if (!checkCycleDependency(depInfo, dependencies)) {
        return false;
      }
    } else {
      _logger->warn("Dependency '{}' not found for mod '{}'", dep, info.name);
    }
  }
  return true;
}
bool ModManager::loadModManifest(const std::string &path) {
  std::string manifestPath = path + "/manifest.json";
  SDL_IOStream *file = SDL_IOFromFile(manifestPath.c_str(), "r");
  if (!file) {
    _logger->error("Failed to open mod manifest: {}", SDL_GetError());
    return false;
  }
  Buffer buffer;
  size_t size = SDL_GetIOSize(file);
  buffer.reset(size + 1);
  ((char *)buffer.getData())[size] = 0;
  if (SDL_ReadIO(file, buffer.getData(), size) != size) {
    _logger->error("Failed to read mod manifest: {}", SDL_GetError());
    SDL_CloseIO(file);
    return false;
  }
  SDL_CloseIO(file);
  cJSON *json = cJSON_Parse((const char *)buffer.getData());
  if (!json) {
    _logger->error("Failed to parse mod manifest: {}", cJSON_GetErrorPtr());
    return false;
  }
  ModInfo info;
  cJSON *nameItem = cJSON_GetObjectItem(json, "name");
  if (nameItem && cJSON_IsString(nameItem)) {
    info.name = nameItem->valuestring;
  } else {
    _logger->error("Mod manifest missing 'name' field");
    cJSON_Delete(json);
    return false;
  }
  cJSON *descriptionItem = cJSON_GetObjectItem(json, "description");
  if (descriptionItem && cJSON_IsString(descriptionItem)) {
    info.description = descriptionItem->valuestring;
  }
  cJSON *versionItem = cJSON_GetObjectItem(json, "version");
  if (versionItem && cJSON_IsString(versionItem)) {
    info.version = versionItem->valuestring;
  } else {
    _logger->warn("Mod manifest missing 'version' field");
  }
  cJSON *authorItem = cJSON_GetObjectItem(json, "author");
  if (authorItem && cJSON_IsString(authorItem)) {
    info.author = authorItem->valuestring;
  }
  cJSON *licenseItem = cJSON_GetObjectItem(json, "license");
  if (licenseItem && cJSON_IsString(licenseItem)) {
    info.license = licenseItem->valuestring;
  }
  cJSON *localeItem = cJSON_GetObjectItem(json, "locale");
  if (localeItem && cJSON_IsString(localeItem)) {
    info.locale = localeItem->valuestring;
  }
  cJSON *dependenciesItem = cJSON_GetObjectItem(json, "dependencies");
  if (dependenciesItem && cJSON_IsArray(dependenciesItem)) {
    size_t size = cJSON_GetArraySize(dependenciesItem);
    for (size_t i = 0; i < size; ++i) {
      cJSON *item = cJSON_GetArrayItem(dependenciesItem, i);
      if (cJSON_IsString(item)) {
        info.dependencies.push_back(item->valuestring);
      }
    }
  }
  cJSON *runtimeDependenciesItem =
      cJSON_GetObjectItem(json, "runtimeDependencies");
  if (runtimeDependenciesItem && cJSON_IsArray(runtimeDependenciesItem)) {
    size_t size = cJSON_GetArraySize(runtimeDependenciesItem);
    for (size_t i = 0; i < size; ++i) {
      cJSON *item = cJSON_GetArrayItem(runtimeDependenciesItem, i);
      if (cJSON_IsString(item)) {
        info.runtimeDependencies.push_back(item->valuestring);
      }
    }
  }
  cJSON_Delete(json);
  info.path = path;
  if (hasMod(info.name)) {
    _logger->error("Duplicate mod found: {}", info.name);
    return false;
  }
  addMod(info.name, info);
  return true;
}
bool ModManager::loadMod(const ModInfo &info) {
  for (auto &dep : info.dependencies) {
    if (!hasMod(dep)) {
      _logger->error("Dependency '{}' not found for mod '{}'", dep, info.name);
      return false;
    }
    auto &depInfo = getMod(dep);
    if (!loadMod(depInfo)) {
      return false;
    }
  }
  auto app = Application::getInstance();
  std::string assetPath = info.path + "/assets/";
  if (std::filesystem::exists(assetPath) &&
      std::filesystem::is_directory(assetPath)) {
    if (!app->getAssetManager()->initStore(assetPath)) {
      return false;
    }
  }
  if (!info.locale.empty()) {
    app->getLocaleManager()->resolveManifest(info.locale, info.name);
  }
  // TODO: script loading and initialization
  return true;
}

bool ModManager::loadAllMod() {
  for (const auto &entry : std::filesystem::directory_iterator(_modPath)) {
    if (entry.is_directory()) {
      auto modPath = entry.path().string();
      if (!std::filesystem::exists(modPath + "/manifest.json")) {
        continue;
      }
      if (!loadModManifest(modPath)) {
        _logger->error("Failed to load mod from path: {}", modPath);
        return false;
      }
    }
  }
  for (auto &[name, info] : _mods) {
    std::vector<std::string> dependencies;
    if (!checkCycleDependency(info, dependencies)) {
      return false;
    }
    if (info.ready) {
      continue;
    }
    if (!loadMod(info)) {
      _logger->error("Failed to load mod: {}", name);
      return false;
    }
    info.ready = true;
    _logger->info("Mod '{}' loaded successfully", name);
  }
  return true;
}