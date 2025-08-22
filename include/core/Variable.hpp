#pragma once
#include "core/Object.hpp"
#include <any>
#include <cstddef>
#include <string>
#include <unordered_map>
#include <vector>
class Variable : public Object {
public:
  using Nil = std::nullptr_t;
  using Number = float;
  using String = std::string;
  using Boolean = bool;
  using Array = std::vector<Variable>;
  using Object = std::unordered_map<std::string, Variable>;

public:
  enum class Type { NIL, NUMBER, STRING, BOOLEAN, ARRAY, OBJECT };

private:
  Type _type;
  std::any _value = nullptr;

public:
  Variable() : _type(Type::NIL) {}
  inline const Type &getType() const { return _type; };
  Variable &setNil();
  Variable &setNumber(float value = .0f);
  Variable &setString(const std::string &value = "");
  Variable &setBoolean(bool value = false);
  Variable &setArray();
  Variable &setObject();
  Number *getNumber();
  String *getString();
  Boolean *getBoolean();
  Array *getArray();
  Object *getObject();
  Number getNumber(Number defaultValue = 0.f) const;
  const String &getString(const String &defaultValue = "") const;
  Boolean getBoolean(Boolean defaultValue = false) const;
  const Array &getArray(const Array &defaultValue = {}) const;
  const Object &getObject(const Object &defaultValue = {}) const;
  Variable &push(const Variable &value);
  size_t getSize() const;
  Variable &setField(const std::string &key, const Variable &value);
  Variable *getField(const std::string &key);
  const Variable &getField(const std::string &key,
                           const Variable &defaultValue = {}) const;
  Variable &removeField(const std::string &key);
  bool hasField(const std::string &key) const;
};