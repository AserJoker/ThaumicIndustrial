#include "core/Variable.hpp"
#include <any>

Variable &Variable::setNil() {
  _type = Type::NIL;
  _value = nullptr;
  return *this;
}
Variable &Variable::setNumber(float value) {
  _type = Type::NUMBER;
  _value = value;
  return *this;
}
Variable &Variable::setString(const std::string &value) {
  _type = Type::STRING;
  _value = value;
  return *this;
}
Variable &Variable::setBoolean(bool value) {
  _type = Type::BOOLEAN;
  _value = value;
  return *this;
}
Variable &Variable::setArray() {
  _type = Type::ARRAY;
  _value = Array{};
  return *this;
}
Variable &Variable::setObject() {
  _type = Type::OBJECT;
  _value = Object{};
  return *this;
}
Variable::Number *Variable::getNumber() {
  if (_type == Type::NUMBER) {
    return std::any_cast<Number>(&_value);
  }
  return nullptr;
}
Variable::String *Variable::getString() {
  if (_type == Type::STRING) {
    return std::any_cast<String>(&_value);
  }
  return nullptr;
}
Variable::Boolean *Variable::getBoolean() {
  if (_type == Type::BOOLEAN) {
    return std::any_cast<Boolean>(&_value);
  }
  return nullptr;
}
Variable::Array *Variable::getArray() {
  if (_type == Type::ARRAY) {
    return std::any_cast<Array>(&_value);
  }
  return nullptr;
}
Variable::Object *Variable::getObject() {
  if (_type == Type::OBJECT) {
    return std::any_cast<Object>(&_value);
  }
  return nullptr;
}

Variable::Number Variable::getNumber(Number defaultValue) const {
  if (_type == Type::NUMBER) {
    return std::any_cast<Number>(_value);
  }
  return defaultValue;
}
const Variable::String &Variable::getString(const String &defaultValue) const {
  if (_type == Type::STRING) {
    return std::any_cast<const String &>(_value);
  }
  return defaultValue;
}
Variable::Boolean Variable::getBoolean(Boolean defaultValue) const {
  if (_type == Type::BOOLEAN) {
    return std::any_cast<Boolean>(_value);
  }
  return defaultValue;
}
const Variable::Array &Variable::getArray(const Array &defaultValue) const {
  if (_type == Type::ARRAY) {
    return std::any_cast<const Array &>(_value);
  }
  return defaultValue;
}
const Variable::Object &Variable::getObject(const Object &defaultValue) const {
  if (_type == Type::OBJECT) {
    return std::any_cast<const Object &>(_value);
  }
  return defaultValue;
}
Variable &Variable::push(const Variable &value) {
  auto arr = getArray();
  if (arr) {
    arr->push_back(value);
  }
  return *this;
}
Variable &Variable::setField(const std::string &key, const Variable &value) {
  auto obj = getObject();
  if (obj) {
    obj->insert({key, value});
  }
  return *this;
}
size_t Variable::getSize() const {
  if (_type == Type::ARRAY) {
    return getArray().size();
  }
  if (_type == Type::OBJECT) {
    return getObject().size();
  }
  return 0;
}
Variable *Variable::getField(const std::string &key) {
  auto obj = getObject();
  if (obj && obj->contains(key)) {
    return &obj->at(key);
  }
  return nullptr;
}
const Variable &Variable::getField(const std::string &key,
                                   const Variable &defaultValue) const {
  if (_type != Variable::Type::OBJECT) {
    return defaultValue;
  }
  auto &obj = getObject();
  if (obj.contains(key)) {
    return obj.at(key);
  }
  return defaultValue;
}
Variable &Variable::removeField(const std::string &key) {
  auto obj = getObject();
  if (obj && obj->contains(key)) {
    obj->erase(key);
  }
  return *this;
}
bool Variable::hasField(const std::string &key) const {
  if (_type != Variable::Type::OBJECT) {
    return false;
  }
  auto &obj = getObject();
  if (obj.contains(key)) {
    return true;
  }
  return false;
}