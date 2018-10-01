// Copyright (c) 2018 Kai Luo <gluokai@gmail.com>. All rights reserved.

#pragma once

#include <string>

namespace re6 {

enum REType {
  kChar,
  kConcat,
  kBranch,
  kStar,
  kPlus,
};

class RE {
public:
  static std::string to_string(RE *re);
  virtual REType GetType() = 0;
};

class Char : public RE {
public:
  explicit Char(char c) : c_(c) {}

  virtual REType GetType() { return kChar; }

  char c() const { return c_; }

private:
  char c_;
};

class Concat : public RE {
public:
  explicit Concat(const std::pair<RE *, RE *> &ops) : ops_(ops) {}

  REType GetType() { return kConcat; }

  std::pair<RE *, RE *> ops() const { return ops_; }

private:
  std::pair<RE *, RE *> ops_;
};

class Branch : public RE {
public:
  explicit Branch(const std::pair<RE *, RE *> &ops) : ops_(ops) {}

  REType GetType() { return kBranch; }

  std::pair<RE *, RE *> &ops() { return ops_; }

private:
  std::pair<RE *, RE *> ops_;
};

class Star : public RE {
public:
  explicit Star(RE *op) : op_(op) {}

  REType GetType() { return kStar; }

  RE *op() { return op_; }

private:
  RE *op_;
};

class Plus : public RE {
public:
  explicit Plus(RE *op) : op_(op) {}

  REType GetType() { return kPlus; }

  RE *op() { return op_; }

private:
  RE *op_;
};

inline std::string RE::to_string(RE *re) {
  std::string result;
  switch (re->GetType()) {
  case kChar: {
    Char *c = dynamic_cast<Char *>(re);
    result.push_back(c->c());
    break;
  }
  case kConcat: {
    Concat *concat = dynamic_cast<Concat *>(re);
    result.append(to_string(concat->ops().first));
    result.append(to_string(concat->ops().second));
    break;
  }
  case kBranch: {
    Branch *branch = dynamic_cast<Branch *>(re);
    result.append(to_string(branch->ops().first));
    result.append("|");
    result.append(to_string(branch->ops().second));
    break;
  }
  case kPlus: {
    Plus *plus = dynamic_cast<Plus *>(re);
    result.append("(");
    result.append(to_string(plus->op()));
    result.append(")");
    result.append("+");
    break;
  }
  case kStar: {
    Star *star = dynamic_cast<Star *>(re);
    result.append("(");
    result.append(to_string(star->op()));
    result.append(")");
    result.append("*");
  }
  }
  return result;
}

} // namespace re6
