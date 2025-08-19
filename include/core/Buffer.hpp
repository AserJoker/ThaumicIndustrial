#pragma once
#include "core/Object.hpp"
#include <SDL3/SDL.h>
class Buffer : public Object {
private:
  void *_data{};
  size_t _size{};

public:
  ~Buffer() override;
  inline size_t getSize() const { return _size; }
  inline void *getData() const { return _data; }
  void reset(size_t size, void *data = nullptr);
  size_t write(size_t offset, size_t len, void *data);
  size_t read(size_t offset, size_t len, void *data);
};