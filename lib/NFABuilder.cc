// Copyright (c) 2018 Kai Luo <gluokai@gmail.com>. All rights reserved.

#include "re6/NFABuilder.h"

#include <iostream>

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
    NFAState *prev = start_;
    for (size_t i = 0; i < concat->ops().size(); ++i) {
      NFAState *next = finish_;
      if (i != concat->ops().size() - 1) {
        next = state_allocator_->Create();
      }
      NFABuilder(state_allocator_, prev, next, concat->ops().at(i)).Build();
      prev = next;
    }
    break;
  }
  case REType::kBranch: {
    Branch *branch = dynamic_cast<Branch *>(re_);
    for (auto re : branch->ops()) {
      NFABuilder(state_allocator_, start_, finish_, re).Build();
    }
    break;
  }
  case REType::kPlus: {
    Plus *plus = dynamic_cast<Plus *>(re_);
    NFABuilder(state_allocator_, start_, finish_, plus->op()).Build();
    NFABuilder(state_allocator_, start_, start_, plus->op()).Build();
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
