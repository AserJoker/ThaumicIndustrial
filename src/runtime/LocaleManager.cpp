#include "runtime/LocaleManager.hpp"
#include "core/Buffer.hpp"
#include "runtime/Application.hpp"
#include <SDL3/SDL_log.h>
#include <memory>
#include <sstream>
void LocaleManager::resolve(
    std::unordered_map<std::string, std::string> &locales,
    const std::string &source, const std::string &name) {
  std::stringstream ss(source.c_str());
  std::string line;
  int idx = 0;
  while (std::getline(ss, line)) {
    size_t comment_pos = line.find('#');
    if (comment_pos != std::string::npos) {
      line = line.substr(0, comment_pos);
    }
    for (auto &ch : line) {
      if (ch == '\t') {
        ch = ' ';
      }
    }
    line.erase(0, line.find_first_not_of(" "));
    line.erase(line.find_last_not_of(" ") + 1);
    if (line.empty()) {
      idx++;
      continue;
    }
    size_t equal_pos = line.find('=');
    if (equal_pos != std::string::npos) {
      std::string key = line.substr(0, equal_pos);
      std::string value = line.substr(equal_pos + 1);
      while (key.back() == ' ') {
        key.pop_back();
      }
      while (value.front() == ' ') {
        value.erase(value.begin());
      }
      while (value.back() == ' ' || value.back() == '\n' ||
             value.back() == '\r' || value.back() == '\t') {
        value.pop_back();
      }
      if (value.front() == '\"') {
        value = value.substr(1, value.length() - 2);
      }
      locales[key] = value;
    } else {
      _logger->warn("Invalid format at: {}:{}", name, idx);
    }
    idx++;
  }
}

void LocaleManager::setLang(const std::string &name) {
  if (_lang != name) {
    _locales.clear();
    _lang = name;
  }
  auto app = Application::getInstance();
  auto assetManager = app->getAssetManager();
  if (!_languages.contains(name)) {
    return;
  }
  for (auto &locale : _languages[name]) {
    auto assetName = locale.asset;
    auto asset = assetManager->query(assetName);
    auto buffer = std::dynamic_pointer_cast<Buffer>(asset);
    if (buffer) {
      std::string source((const char *)buffer->getData(), buffer->getSize());
      resolve(_locales, source, assetName);
    }
  }
}

void LocaleManager::setDefaultLang(const std::string &name) {
  if (_defaultLang != name) {
    _defaultLocales.clear();
    _defaultLang = name;
  }
  auto app = Application::getInstance();
  auto assetManager = app->getAssetManager();
  if (!_languages.contains(name)) {
    return;
  }
  for (auto &locale : _languages[name]) {
    auto assetName = locale.asset;
    auto asset = assetManager->query(assetName);
    auto buffer = std::dynamic_pointer_cast<Buffer>(asset);
    if (buffer) {
      std::string source((const char *)buffer->getData(), buffer->getSize());
      resolve(_defaultLocales, source, assetName);
    }
  }
}

std::string LocaleManager::i18n(
    const std::string &key,
    const std::unordered_map<std::string, std::string> &options) const {
  auto result = key;
  if (_locales.contains(key)) {
    result = _locales.at(key);
  } else if (_defaultLocales.contains(key)) {
    result = _defaultLocales.at(key);
  }
  for (auto &[key, value] : options) {
    auto keystr = "{" + key + "}";
    size_t pos = 0;
    while ((pos = result.find(keystr)) != std::string::npos) {
      result.replace(pos, keystr.length(), value);
      pos += value.length();
    }
  }
  return result;
}

void LocaleManager::addLanguage(const std::string &key, const Locale &locale) {
  _languages[key].push_back(locale);
}
void LocaleManager::removeLanguage(const std::string &key) {
  _languages.erase(key);
}
bool LocaleManager::hasLanguage(const std::string &key) const {
  return _languages.contains(key);
}
const std::unordered_map<std::string, std::vector<LocaleManager::Locale>> &
LocaleManager::getLanguages() const {
  return _languages;
}
void LocaleManager::reset() {
  _locales.clear();
  _defaultLocales.clear();
  _lang = "";
  _defaultLang = "";
}
void LocaleManager::resolveManifest(const std::string &manifest,
                                    const std::string &mod) {
  auto app = Application::getInstance();
  auto assetManager = app->getAssetManager();
  auto asset = assetManager->query(manifest);
  auto variable = std::dynamic_pointer_cast<Variable>(asset);
  if (!variable) {
    _logger->error("Failed to resolve manifest: {}", manifest);
    return;
  }
  auto languages = variable->getField("languages");
  if (languages->getType() != Variable::Type::ARRAY) {
    _logger->error("Invalid manifest format: languages is not an array");
    return;
  }
  for (const auto &lang : *languages->getArray()) {
    if (lang.getType() != Variable::Type::OBJECT) {
      _logger->error("Invalid manifest format: language is not an object");
      continue;
    }
    if (!lang.hasField("code") || !lang.hasField("name") ||
        !lang.hasField("asset")) {
      _logger->error(
          "Invalid manifest format: missing required fields in language");
      continue;
    }
    auto code = lang.getField("code").getString();
    Locale locale;
    locale.name = lang.getField("name").getString();
    locale.description = lang.getField("description").getString();
    locale.version = lang.getField("version").getString();
    locale.author = lang.getField("author").getString();
    locale.license = lang.getField("license").getString();
    locale.supported = lang.getField("supported").getString();
    locale.asset = lang.getField("asset").getString();
    locale.mod = mod;
    addLanguage(code, locale);
  }
}