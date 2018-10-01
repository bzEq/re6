// Copyright (c) 2018 Kai Luo <gluokai@gmail.com>. All rights reserved.

#include "re6/ExecutableBuffer.h"
#include "re6/Matcher.h"
#include "re6/NFA.h"
#include "re6/NFABuilder.h"
#include "re6/Support.h"
#include "re6/re.h"

#include <cassert>
#include <iostream>

int main() {
  using namespace re6;
  Char a('a');
  Char b('b');
  Char c('c');
  Char d('d');
  Concat ab(std::make_pair(&a, &b));
  Concat cd(std::make_pair(&c, &d));
  Branch s(std::make_pair(&ab, &cd));
  Star ss(&s);
  Plus sp(&s);
  DEFER_LAMBDA(([&ss, &sp] {
    std::cout << RE::to_string(static_cast<RE *>(&ss)) << "\n";
    std::cout << RE::to_string(static_cast<RE *>(&sp)) << "\n";
  }));
  StateAllocator alloc;
  NFAState start, finish;
  NFABuilder(&alloc, &start, &finish, &ss).Build();
  NFABuilder(&alloc, &start, &finish, &sp).Build();
  Slice empty("");
  Slice t("abcdababcdcd");
  assert(Matcher(&start, &finish, t).Match());
  assert(Matcher(&start, &finish, empty).Match());
  jit::ExecutableBuffer buffer(4);
  assert(buffer.Init());
  return 0;
}
