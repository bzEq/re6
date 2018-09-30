// Copyright (c) 2018 Kai Luo <gluokai@gmail.com>. All rights reserved.

#pragma once

#include <map>
#include <set>
#include <vector>

namespace re6 {

constexpr char EP = -1;

struct NFAState {
  using state_t = uint64_t;

  std::map<char, std::set<NFAState *>> jump;

  void AddJump(char c, NFAState *target) {
    if (not jump.count(c)) {
      jump.insert(std::make_pair(c, std::set<NFAState *>()));
    }
    jump[c].insert(target);
  }
};

class StateAllocator {
public:
  NFAState *Create() {
    alloc_.emplace_back(new NFAState);
    return alloc_.back();
  }

  ~StateAllocator() {
    for (auto p : alloc_) {
      delete p;
    }
  }

private:
  std::vector<NFAState *> alloc_;
};

} // namespace re6
