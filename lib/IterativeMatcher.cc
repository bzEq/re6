// Copyright (c) 2018 Kai Luo <gluokai@gmail.com>. All rights reserved.

#include "re6/IterativeMatcher.h"
#include "re6/re.h"

#include <queue>

namespace re6 {

bool IterativeMatcher::Match() const {
  // TODO: Make matching parallel
  std::queue<State> worklist;
  worklist.push({
      .nfa_state = start_,
      .slice = s_,
  });
  while (not worklist.empty()) {
    State current = std::move(worklist.front());
    worklist.pop();
    if (current.slice.empty() && current.nfa_state == finish_) {
      return true;
    }
    if (current.nfa_state->jump.count(EP)) {
      for (auto t : current.nfa_state->jump[EP]) {
        worklist.push({
            .nfa_state = t,
            .slice = current.slice,
        });
      }
    }
    if (not current.slice.empty() &&
        current.nfa_state->jump.count(current.slice[0])) {
      for (auto t : current.nfa_state->jump[current.slice[0]]) {
        worklist.push({
            .nfa_state = t,
            .slice = current.slice.advance(),
        });
      }
    }
  }
  return false;
}

} // namespace re6
