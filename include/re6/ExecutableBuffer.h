// Copyright (c) 2018 Kai Luo <gluokai@gmail.com>. All rights reserved.

#pragma once

#include <sys/mman.h>

#include "re6/slice.h"

#include <cstddef>
#include <cstdint>

namespace re6::jit {

class ExecutableBuffer {
public:
  explicit ExecutableBuffer(size_t pages)
      : pages_(pages), buffer_(nullptr), size_(0), point_(0) {}

  bool Init();

  bool Write(Slice s);

  ~ExecutableBuffer() {
    if (buffer_) {
      munmap(buffer_, size_);
      buffer_ = nullptr;
    }
  }

private:
  const size_t pages_;
  uint8_t *buffer_;
  size_t size_, point_;
};

} // namespace re6::jit
