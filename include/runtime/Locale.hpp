#pragma once
#include "core/Object.hpp"
#include "runtime/Logger.hpp"
#include <string>
#include <unordered_map>
class Locale : public Object {
private:
  std::unordered_map<std::string, std::string> _locales;
  std::unordered_map<std::string, std::string> _defaultLocales;
  std::string _lang;
  std::string _defaultLang;
  std::unordered_map<std::string, std::string> _languages;
  Logger *_logger = Logger::getLogger("Locale");

private:
  void resolve(std::unordered_map<std::string, std::string> &locale,
               const std::string &source, const std::string &name);

public:
  void setLang(const std::string &name);
  void setDefaultLang(const std::string &name);
  inline const std::string &getLang() const { return _lang; };
  inline const std::string &getDefaultLang() const { return _defaultLang; }
  std::string
  i18n(const std::string &key,
       const std::unordered_map<std::string, std::string> &options = {}) const;
  void addLanguage(const std::string &key, const std::string &name);
  void removeLanguage(const std::string &key);
  bool hasLanguage(const std::string &key) const;
  const std::string &getLanguageName(const std::string &key) const;
  const std::unordered_map<std::string, std::string> &getLanguages() const;
  void reset();
};