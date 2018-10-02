// Copyright (c) 2018 Kai Luo <gluokai@gmail.com>. All rights reserved.

#include "re6/ExecutableBuffer.h"
#include "re6/Matcher.h"
#include "re6/NFA.h"
#include "re6/NFABuilder.h"
#include "re6/Support.h"
#include "re6/re.h"

#include "gtest/gtest.h"

#include <iostream>

namespace {
using namespace re6;

TEST(RETest, Textify) {
  Char a('a');
  Char b('b');
  Char c('c');
  Char d('d');
  Concat ab(std::make_pair(&a, &b));
  Concat cd(std::make_pair(&c, &d));
  Branch s(std::make_pair(&ab, &cd));
  Star ss(&s);
  EXPECT_TRUE(RE::to_string(&ss) == "(ab|cd)*");
}

TEST(RETest, Textify1) {
  Char a('a');
  Char b('b');
  Char c('c');
  Char d('d');
  Concat ab(std::make_pair(&a, &b));
  Concat cd(std::make_pair(&c, &d));
  Branch s(std::make_pair(&ab, &cd));
  Plus sp(&s);
  EXPECT_TRUE(RE::to_string(&sp) == "(ab|cd)+");
}

TEST(RETest, Textify2) {
  Char a('a');
  Char b('b');
  Char c('c');
  Char d('d');
  Concat ab(std::make_pair(&a, &b));
  Concat cd(std::make_pair(&c, &d));
  Branch s(std::make_pair(&ab, &cd));
  Question sq(&s);
  EXPECT_TRUE(RE::to_string(&sq) == "(ab|cd)?");
}

TEST(NFATest, Simple) {
  Char a('a');
  Char b('b');
  Char c('c');
  Char d('d');
  Concat ab(std::make_pair(&a, &b));
  Concat cd(std::make_pair(&c, &d));
  Branch s(std::make_pair(&ab, &cd));
  Star ss(&s);
  StateAllocator alloc;
  NFAState start, finish;
  NFABuilder(&alloc, &start, &finish, &ss).Build();
  EXPECT_TRUE(Matcher(&start, &finish, "abcdababcdcd").Match());
  EXPECT_TRUE(Matcher(&start, &finish, "").Match());
}

TEST(NFATest, Simple1) {
  Char a('a');
  Char b('b');
  Char c('c');
  Char d('d');
  Concat ab(std::make_pair(&a, &b));
  Concat cd(std::make_pair(&c, &d));
  Branch s(std::make_pair(&ab, &cd));
  Question sq(&s);
  StateAllocator alloc;
  NFAState start, finish;
  NFABuilder(&alloc, &start, &finish, &sq).Build();
  EXPECT_TRUE(Matcher(&start, &finish, "").Match());
  EXPECT_TRUE(Matcher(&start, &finish, "ab").Match());
  EXPECT_TRUE(Matcher(&start, &finish, "cd").Match());
  EXPECT_TRUE(not Matcher(&start, &finish, "abcd").Match());
}

TEST(NFATest, Simple2) {
  Char a('a');
  Char b('b');
  Char c('c');
  Char d('d');
  Concat ab(std::make_pair(&a, &b));
  Concat cd(std::make_pair(&c, &d));
  Branch s(std::make_pair(&ab, &cd));
  Plus sp(&s);
  StateAllocator alloc;
  NFAState start, finish;
  NFABuilder(&alloc, &start, &finish, &sp).Build();
  EXPECT_TRUE(not Matcher(&start, &finish, "").Match());
  EXPECT_TRUE(Matcher(&start, &finish, "abcd").Match());
}

} // namespace
