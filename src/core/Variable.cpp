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

const Variable::Number *Variable::getNumber() const {
  if (_type == Type::NUMBER) {
    return std::any_cast<Number>(&_value);
  }
  return nullptr;
}
const Variable::String *Variable::getString() const {
  if (_type == Type::STRING) {
    return std::any_cast<String>(&_value);
  }
  return nullptr;
}
const Variable::Boolean *Variable::getBoolean() const {
  if (_type == Type::BOOLEAN) {
    return std::any_cast<Boolean>(&_value);
  }
  return nullptr;
}
const Variable::Array *Variable::getArray() const {
  if (_type == Type::ARRAY) {
    return std::any_cast<Array>(&_value);
  }
  return nullptr;
}
const Variable::Object *Variable::getObject() const {
  if (_type == Type::OBJECT) {
    return std::any_cast<Object>(&_value);
  }
  return nullptr;
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
    return getArray()->size();
  }
  if (_type == Type::OBJECT) {
    return getObject()->size();
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
const Variable *Variable::getField(const std::string &key) const {
  auto obj = getObject();
  if (obj && obj->contains(key)) {
    return &obj->at(key);
  }
  return nullptr;
}
Variable &Variable::removeField(const std::string &key) {
  auto obj = getObject();
  if (obj && obj->contains(key)) {
    obj->erase(key);
  }
  return *this;
}
bool Variable::hasField(const std::string &key) const {
  auto obj = getObject();
  if (obj && obj->contains(key)) {
    return true;
  }
  return false;
}