// Copyright (c) 2018 Kai Luo <gluokai@gmail.com>. All rights reserved.

#include "re6/DFAMatcher.h"

#include <iostream>

namespace re6 {

bool DFAMatcher::Match() const {
  DFAState *current = dfa_.start;
  for (auto c : s_.to_string()) {
    if (not current->jump.count(c)) {
      return false;
    }
    current = current->jump[c];
  }
  return dfa_.finish_set.count(current);
}

} // namespace re6
