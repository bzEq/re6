// Copyright (c) 2018 Kai Luo <gluokai@gmail.com>. All rights reserved.

#pragma once

#include "re6/DFABuilder.h"
#include "re6/slice.h"

namespace re6 {

class DFAMatcher {
public:
  explicit DFAMatcher(const DFABuilder::Result &dfa, Slice s)
      : dfa_(dfa), s_(s) {}

  bool Match() const;

private:
  DFABuilder::Result dfa_;
  Slice s_;
};

} // namespace re6
