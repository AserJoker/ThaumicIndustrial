#pragma once
#include "core/Object.hpp"
#include <string>
#include <unordered_map>
class Locale : public Object {
private:
  std::unordered_map<std::string, std::string> _locales;
  std::unordered_map<std::string, std::string> _defaultLocales;
  std::string _lang;
  std::string _defaultLang;

private:
  static void resolve(std::unordered_map<std::string, std::string> &locale,
                      const std::string &source, const std::string &name);

public:
  void setLang(const std::string &name);
  void setDefaultLang(const std::string &name);
  inline const std::string &getLang() const { return _lang; };
  inline const std::string &getDefaultLang() const { return _defaultLang; }
  const std::string &i18n(const std::string &key) const;
  void reset();
};