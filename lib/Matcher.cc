// Copyright (c) 2018 Kai Luo <gluokai@gmail.com>. All rights reserved.

#include "re6/Matcher.h"
#include "re6/re.h"

namespace re6 {

bool Matcher::Match() const {
  if (start_ == finish_ && s_.empty()) {
    return true;
  }
  if (not StepMatch(EP, s_)) {
    if (s_.empty()) {
      return false;
    } else {
      return StepMatch(s_[0], s_.advance());
    }
  }
  return true;
}

bool Matcher::StepMatch(char head, Slice tail) const {
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

} // namespace re6
