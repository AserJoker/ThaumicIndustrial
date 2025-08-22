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
  Type _type = Type::NIL;
  std::any _value = nullptr;

public:
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
  const Number *getNumber() const;
  const String *getString() const;
  const Boolean *getBoolean() const;
  const Array *getArray() const;
  const Object *getObject() const;
  Variable &push(const Variable &value);
  size_t getSize() const;
  Variable &setField(const std::string &key, const Variable &value);
  Variable *getField(const std::string &key);
  const Variable *getField(const std::string &key) const;
  Variable &removeField(const std::string &key);
  bool hasField(const std::string &key) const;
};