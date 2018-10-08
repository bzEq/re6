// Copyright (c) 2018 Kai Luo <gluokai@gmail.com>. All rights reserved.

#include "re6/ExecutableBuffer.h"
#include "re6/IterativeMatcher.h"
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
  Concat ab({&a, &b});
  Concat cd({&c, &d});
  Branch s({&ab, &cd});
  Star ss(&s);
  EXPECT_TRUE(RE::to_string(&ss) == "(ab|cd)*");
}

TEST(RETest, Textify1) {
  Char a('a');
  Char b('b');
  Char c('c');
  Char d('d');
  Concat ab({&a, &b});
  Concat cd({&c, &d});
  Branch s({&ab, &cd});
  Plus sp(&s);
  EXPECT_TRUE(RE::to_string(&sp) == "(ab|cd)+");
}

TEST(RETest, Textify2) {
  Char a('a');
  Char b('b');
  Char c('c');
  Char d('d');
  Concat ab({&a, &b});
  Concat cd({&c, &d});
  Branch s({&ab, &cd});
  Question sq(&s);
  EXPECT_TRUE(RE::to_string(&sq) == "(ab|cd)?");
}

TEST(NFATest, Simple) {
  Char a('a');
  Char b('b');
  Char c('c');
  Char d('d');
  Concat ab({&a, &b});
  Concat cd({&c, &d});
  Branch s({&ab, &cd});
  Star ss(&s);
  StateAllocator alloc;
  NFAState start, finish;
  NFABuilder(&alloc, &start, &finish, &ss).Build();
  EXPECT_TRUE(Matcher(&start, &finish, "ab").Match());
  EXPECT_TRUE(Matcher(&start, &finish, "abab").Match());
  EXPECT_TRUE(Matcher(&start, &finish, "cd").Match());
  EXPECT_TRUE(Matcher(&start, &finish, "cdcd").Match());
  EXPECT_TRUE(Matcher(&start, &finish, "abcdababcdcd").Match());
  EXPECT_TRUE(Matcher(&start, &finish, "").Match());
}

TEST(NFATest, Simple1) {
  Char a('a');
  Char b('b');
  Char c('c');
  Char d('d');
  Concat ab({&a, &b});
  Concat cd({&c, &d});
  Branch s({&ab, &cd});
  Question sq(&s);
  StateAllocator alloc;
  NFAState start, finish;
  NFABuilder(&alloc, &start, &finish, &sq).Build();
  EXPECT_TRUE(Matcher(&start, &finish, "").Match());
  EXPECT_TRUE(Matcher(&start, &finish, "ab").Match());
  EXPECT_TRUE(not Matcher(&start, &finish, "abab").Match());
  EXPECT_TRUE(Matcher(&start, &finish, "cd").Match());
  EXPECT_TRUE(not Matcher(&start, &finish, "cdcd").Match());
  EXPECT_TRUE(not Matcher(&start, &finish, "abcd").Match());
}

TEST(NFATest, Simple2) {
  REAllocator re_alloc;
  Char *a = re_alloc.Create<Char>('a');
  Char *b = re_alloc.Create<Char>('b');
  Char *c = re_alloc.Create<Char>('c');
  Char *d = re_alloc.Create<Char>('d');
  Concat *ab = re_alloc.Create<Concat>({a, b});
  Concat *cd = re_alloc.Create<Concat>({c, d});
  Branch *s = re_alloc.Create<Branch>({ab, cd});
  Plus *sp = re_alloc.Create<Plus>(s);
  StateAllocator alloc;
  NFAState start, finish;
  NFABuilder(&alloc, &start, &finish, sp).Build();
  EXPECT_TRUE(not Matcher(&start, &finish, "").Match());
  EXPECT_TRUE(Matcher(&start, &finish, "abcd").Match());
}

TEST(NFATest, Simple3) {
  REAllocator re_alloc;
  auto re = re_alloc.Create<Branch>({
      re_alloc.Create<Char>('a'),
      re_alloc.Create<Char>('b'),
      re_alloc.Create<Char>('c'),
  });
  StateAllocator alloc;
  NFAState start, finish;
  NFABuilder(&alloc, &start, &finish, re).Build();
  EXPECT_TRUE(not IterativeMatcher(&start, &finish, "").Match());
  EXPECT_TRUE(IterativeMatcher(&start, &finish, "a").Match());
  EXPECT_TRUE(IterativeMatcher(&start, &finish, "b").Match());
  EXPECT_TRUE(IterativeMatcher(&start, &finish, "c").Match());
  EXPECT_TRUE(not IterativeMatcher(&start, &finish, "abc").Match());
}

} // namespace
