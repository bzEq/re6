// Copyright (c) 2018 Kai Luo <gluokai@gmail.com>. All rights reserved.

#include "re6/NFA.h"
#include "re6/NFABuilder.h"
#include "re6/re.h"

#include <iostream>

int main() {
  using namespace re6;
  Char a('a');
  Char b('b');
  Char c('c');
  Char d('d');
  Concat ab(std::make_pair(&a, &b));
  Concat cd(std::make_pair(&c, &d));
  Select s(std::make_pair(&ab, &cd));
  Star ss(&s);
  std::cout << RE::to_string(static_cast<RE *>(&ss)) << "\n";
  StateAllocator alloc;
  NFAState start, finish;
  NFABuilder(&alloc, &start, &finish, &ss).Build();
  return 0;
}
