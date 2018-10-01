// Copyright (c) 2018 Kai Luo <gluokai@gmail.com>. All rights reserved.

#include "re6/NFABuilder.h"

namespace re6 {

void NFABuilder::Build() {
  switch (re_->GetType()) {
  case REType::kChar: {
    Char *c = dynamic_cast<Char *>(re_);
    start_->AddJump(c->c(), finish_);
    break;
  }
  case REType::kConcat: {
    Concat *concat = dynamic_cast<Concat *>(re_);
    NFAState *middle = state_allocator_->Create();
    NFABuilder(state_allocator_, start_, middle, concat->ops().first).Build();
    NFABuilder(state_allocator_, middle, finish_, concat->ops().second).Build();
    break;
  }
  case REType::kBranch: {
    Branch *branch = dynamic_cast<Branch *>(re_);
    NFABuilder(state_allocator_, start_, finish_, branch->ops().first).Build();
    NFABuilder(state_allocator_, start_, finish_, branch->ops().second).Build();
    break;
  }
  case REType::kPlus: {
    Plus *plus = dynamic_cast<Plus *>(re_);
    NFAState *middle = state_allocator_->Create();
    Char ep(EP);
    NFABuilder(state_allocator_, start_, finish_, plus->op()).Build();
    NFABuilder(state_allocator_, start_, middle, &ep).Build();
    NFABuilder(state_allocator_, middle, start_, plus->op()).Build();
    break;
  }
  case REType::kStar: {
    Star *star = dynamic_cast<Star *>(re_);
    Char ep(EP);
    NFABuilder(state_allocator_, start_, finish_, &ep).Build();
    NFABuilder(state_allocator_, start_, start_, star->op()).Build();
    break;
  }
  case REType::kQuestion: {
    Question *question = dynamic_cast<Question *>(re_);
    Char ep(EP);
    NFABuilder(state_allocator_, start_, finish_, &ep).Build();
    NFABuilder(state_allocator_, start_, finish_, question->op()).Build();
    break;
  }
  }
}

} // namespace re6
