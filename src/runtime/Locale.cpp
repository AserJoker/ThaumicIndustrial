#include "runtime/Locale.hpp"
#include "core/Buffer.hpp"
#include "runtime/Application.hpp"
#include <SDL3/SDL_log.h>
#include <memory>
#include <sstream>
void Locale::resolve(std::unordered_map<std::string, std::string> &locales,
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
      SDL_LogWarn(SDL_LOG_CATEGORY_SYSTEM, "Invalid format at: %s:%d",
                  name.c_str(), idx);
    }
    idx++;
  }
}
void Locale::setLang(const std::string &name) {
  if (_lang != name) {
    _locales.clear();
    _lang = name;
  }
  auto app = Application::getInstance();
  auto assetManager = app->getAssetManager();
  auto assetName = "lang:" + Application::APP_NAME + ".locale." + name;
  auto asset = assetManager->query(assetName);
  auto buffer = std::dynamic_pointer_cast<Buffer>(asset);
  if (buffer) {
    std::string source((const char *)buffer->getData(), buffer->getSize());
    resolve(_locales, source, assetName);
  }
}

void Locale::setDefaultLang(const std::string &name) {
  if (_defaultLang != name) {
    _locales.clear();
    _defaultLang = name;
  }
  auto app = Application::getInstance();
  auto assetManager = app->getAssetManager();
  auto assetName = "lang:" + Application::APP_NAME + ".locale." + name;
  auto asset = assetManager->query(assetName);
  auto buffer = std::dynamic_pointer_cast<Buffer>(asset);
  if (buffer) {
    std::string source((const char *)buffer->getData(), buffer->getSize());
    resolve(_defaultLocales, source, assetName);
  }
}

const std::string &Locale::i18n(const std::string &key) const {
  if (_locales.contains(key)) {
    return _locales.at(key);
  }
  if (_defaultLocales.contains(key)) {
    return _defaultLocales.at(key);
  }
  return key;
}

void Locale::addLanguage(const std::string &key, const std::string &name) {
  _languages[key] = name;
}
void Locale::removeLanguage(const std::string &key) { _languages.erase(key); }
bool Locale::hasLanguage(const std::string &key) const {
  return _languages.contains(key);
}
const std::string &Locale::getLanguageName(const std::string &key) const {
  if (_languages.contains(key)) {
    return _languages.at(key);
  }
  return key;
}
const std::unordered_map<std::string, std::string> &
Locale::getLanguages() const {
  return _languages;
}
void Locale::reset() {
  _locales.clear();
  _defaultLocales.clear();
  _lang = "";
  _defaultLang = "";
}