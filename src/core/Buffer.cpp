#include "core/Buffer.hpp"
#include <SDL3/SDL_iostream.h>
#include <new>
Buffer::~Buffer() {
  if (_data) {
    ::operator delete(_data);
    _data = nullptr;
    _size = 0;
  }
}
void Buffer::reset(size_t size, void *data) {
  if (_data) {
    ::operator delete(_data);
    _data = nullptr;
  }
  _size = size;
  if (_size) {
    _data = ::operator new(size);
  }
  if (data) {
    memcpy(_data, data, size);
  }
}
size_t Buffer::write(size_t offset, size_t len, void *data) {
  size_t size = len;
  if (size + offset > _size) {
    size = _size - offset;
  }
  memcpy((uint8_t *)_data + offset, data, size);
  return size;
}
size_t Buffer::read(size_t offset, size_t len, void *data) {
  size_t size = len;
  if (size + offset > _size) {
    size = _size - offset;
  }
  memcpy(data, (uint8_t *)_data + offset, size);
  return size;
}