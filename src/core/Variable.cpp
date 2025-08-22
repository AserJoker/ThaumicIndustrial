#include "core/Variable.hpp"
#include <any>

void Variable::setNil() {
  _type = Type::NIL;
  _value = nullptr;
}
void Variable::setNumber(float value) {
  _type = Type::NUMBER;
  _value = value;
}
void Variable::setString(const std::string &value) {
  _type = Type::STRING;
  _value = value;
}
void Variable::setBoolean(bool value) {
  _type = Type::BOOLEAN;
  _value = value;
}
void Variable::setArray() {
  _type = Type::ARRAY;
  _value = Array{};
}
void Variable::setObject() {
  _type = Type::OBJECT;
  _value = Object{};
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