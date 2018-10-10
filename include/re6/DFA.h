// Copyright (c) 2018 Kai Luo <gluokai@gmail.com>. All rights reserved.

#pragma once

#include <map>
#include <vector>

namespace re6 {

struct DFAState {
  std::map<char, DFAState *> jump;

  void SetJump(char c, DFAState *target) {
    jump.insert(std::make_pair(c, target));
  }
};

class DFAStateAllocator {
public:
  DFAState *Create() {
    alloc_.emplace_back(new DFAState);
    return alloc_.back();
  }

  ~DFAStateAllocator() {
    for (auto p : alloc_) {
      delete p;
    }
  }

private:
  std::vector<DFAState *> alloc_;
};

} // namespace re6
