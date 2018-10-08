// Copyright (c) 2018 Kai Luo <gluokai@gmail.com>. All rights reserved.

#pragma once

#include "re6/NFA.h"
#include "re6/slice.h"

namespace re6 {

class IterativeMatcher {
public:
  struct State {
    NFAState *nfa_state;
    Slice slice;
  };

public:
  explicit IterativeMatcher(NFAState *start, NFAState *finish, Slice s)
      : start_(start), finish_(finish), s_(s) {}

  bool Match() const;

private:
  NFAState *start_, *finish_;
  Slice s_;
};

} // namespace re6
