// Copyright (c) 2018 Kai Luo <gluokai@gmail.com>. All rights reserved.

#include <unistd.h>

#include "re6/ExecutableBuffer.h"

namespace re6::jit {

bool ExecutableBuffer::Init() {
  size_ = pages_ * sysconf(_SC_PAGE_SIZE);
  buffer_ = static_cast<uint8_t *>(mmap(nullptr, size_,
                                        PROT_READ | PROT_WRITE | PROT_EXEC,
                                        MAP_PRIVATE | MAP_ANONYMOUS, -1, 0));
  if (buffer_ == MAP_FAILED) {
    buffer_ = nullptr;
    return false;
  }
  return true;
}

bool ExecutableBuffer::Write(Slice s) {
  if (point_ + s.size() > size_) {
    return false;
  }
  memcpy(buffer_, s.data(), s.size());
  point_ += s.size();
  return true;
}

} // namespace re6::jit
