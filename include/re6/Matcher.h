// Copyright (c) 2018 Kai Luo <gluokai@gmail.com>. All rights reserved.

#pragma once

#include "re6/NFA.h"
#include "re6/slice.h"

namespace re6 {

class Matcher {
public:
  explicit Matcher(NFAState *start, NFAState *finish, Slice s)
      : start_(start), finish_(finish), s_(s) {}

  bool Match() const;

private:
  bool StepMatch(char head, Slice tail) const;

private:
  NFAState *start_, *finish_;
  Slice s_;
};
} // namespace re6
