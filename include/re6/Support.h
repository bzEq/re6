// Copyright (c) 2018 Kai Luo <gluokai@gmail.com>. All rights reserved.

#pragma once

#include <functional>

namespace re6::support {

class Defer {
public:
  template <typename Callable>
  explicit Defer(Callable &&defer) : defer_(std::forward<Callable>(defer)) {}

  ~Defer() {
    if (defer_) {
      defer_();
    }
  }

private:
  std::function<void()> defer_;
};

#define CONCAT_(a, b) a##b
#define CONCAT(a, b) CONCAT_(a, b)
#define DEFER_LAMBDA(lambda)                                                   \
  re6::support::Defer CONCAT(__espresso_deferred_lambda_, __COUNTER__)(lambda)

} // namespace re6::support
