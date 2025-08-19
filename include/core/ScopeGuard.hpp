#pragma once
#include <functional>
class ScopeGuard {
private:
  std::function<void()> _handle;

public:
  ScopeGuard(const std::function<void()> &handle) : _handle(handle) {}
  ~ScopeGuard() { _handle(); }
};
#define DEFER(cb) ScopeGuard __guard##__LINE__(cb)