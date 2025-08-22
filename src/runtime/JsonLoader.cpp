#include "runtime/JsonLoader.hpp"
#include "core/Buffer.hpp"
#include "core/Variable.hpp"
#include <SDL3/SDL_iostream.h>
#include <cjson/cjson.h>
#include <memory>
static void resolveJSON(cJSON *json, Variable &variable) {
  if (cJSON_IsNull(json)) {
    return;
  } else if (cJSON_IsString(json)) {
    variable.setString(json->valuestring);
  } else if (cJSON_IsNumber(json)) {
    variable.setNumber(json->valuedouble);
  } else if (cJSON_IsBool(json)) {
    variable.setBoolean(json->valueint != 0);
  } else if (cJSON_IsArray(json)) {
    size_t size = cJSON_GetArraySize(json);
    for (size_t i = 0; i < size; ++i) {
      cJSON *item = cJSON_GetArrayItem(json, i);
      variable.setArray();
      resolveJSON(item, variable.push({}).getArray()->back());
    }
  } else if (cJSON_IsObject(json)) {
    cJSON *child = json->child;
    variable.setObject();
    while (child) {
      resolveJSON(
          child, *variable.setField(child->string, {}).getField(child->string));
      child = child->next;
    }
  }
}
std::shared_ptr<Object> JsonLoader::load(const std::string &path) {
  SDL_IOStream *file = SDL_IOFromFile(path.c_str(), "r");
  if (!file) {
    _logger->error("Failed to open JSON file: {}", SDL_GetError());
    return nullptr;
  }
  Buffer buffer;
  size_t size = SDL_GetIOSize(file);
  buffer.reset(size + 1);
  ((char *)buffer.getData())[size] = 0;
  if (SDL_ReadIO(file, buffer.getData(), size) != size) {
    _logger->error("Failed to read JSON file: {}", SDL_GetError());
    SDL_CloseIO(file);
    return nullptr;
  }
  SDL_CloseIO(file);
  cJSON *json = cJSON_Parse((const char *)buffer.getData());
  if (!json) {
    _logger->error("Failed to parse JSON: {}", cJSON_GetErrorPtr());
    return nullptr;
  }
  auto variable = std::make_shared<Variable>();
  resolveJSON(json, *variable);
  cJSON_Delete(json);
  return variable;
}