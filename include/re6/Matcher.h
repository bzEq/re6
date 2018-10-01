// Copyright (c) 2018 Kai Luo <gluokai@gmail.com>. All rights reserved.

#pragma once

#include "re6/NFA.h"
#include "re6/slice.h"

namespace re6 {

class Matcher {
public:
  explicit Matcher(NFAState *start, NFAState *finish, Slice s)
      : start_(start), finish_(finish), s_(s) {}

  bool Match() {
    if (start_ == finish_ && s_.empty()) {
      return true;
    }
    if (s_.empty()) {
      return StepMatch(EP, s_);
    }
    return StepMatch(s_[0], s_.advance());
  }

private:
  bool StepMatch(char head, Slice tail) {
    if (not start_->jump.count(head)) {
      return false;
    }
    std::set<NFAState *> &targets = start_->jump[head];
    for (auto t : targets) {
      if (Matcher(t, finish_, tail).Match()) {
        return true;
      }
    }
    return false;
  }

  NFAState *start_, *finish_;
  Slice s_;
};
} // namespace re6
