// Copyright (c) 2018 Kai Luo <gluokai@gmail.com>. All rights reserved.

#pragma once

#include "re6/NFA.h"
#include "re6/re.h"

namespace re6 {

class NFABuilder {
public:
  explicit NFABuilder(StateAllocator *state_allocator, NFAState *start,
                      NFAState *finish, RE *re)
      : state_allocator_(state_allocator), start_(start), finish_(finish),
        re_(re) {}

  void Build();

private:
  StateAllocator *state_allocator_;
  NFAState *start_, *finish_;
  RE *re_;
};

} // namespace re6
