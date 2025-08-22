#pragma once
#include "core/Object.hpp"
#include "runtime/Logger.hpp"
#include <string>
#include <unordered_map>
#include <vector>
class LocaleManager : public Object {
public:
  struct Locale {
    std::string name;
    std::string description;
    std::string version;
    std::string author;
    std::string license;
    std::string supported;
    std::string asset;
    std::string mod;
  };

private:
  std::unordered_map<std::string, std::string> _locales;
  std::unordered_map<std::string, std::string> _defaultLocales;
  std::string _lang;
  std::string _defaultLang;
  std::unordered_map<std::string, std::vector<Locale>> _languages;
  Logger *_logger = Logger::getLogger("LocaleManager");

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
  void addLanguage(const std::string &key, const Locale &locale);
  void removeLanguage(const std::string &key);
  bool hasLanguage(const std::string &key) const;
  const std::unordered_map<std::string, std::vector<Locale>> &
  getLanguages() const;
  void reset();
  void resolveManifest(const std::string &manifest,
                       const std::string &mod = "internal");
};