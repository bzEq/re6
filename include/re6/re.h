// Copyright (c) 2018 Kai Luo <gluokai@gmail.com>. All rights reserved.

#pragma once

#include <string>
#include <vector>

namespace re6 {

enum class REType {
  kChar,
  kConcat,
  kBranch,
  kStar,
  kPlus,
  kQuestion,
};

class RE {
public:
  static std::string to_string(RE *re);
  virtual REType GetType() = 0;
  virtual ~RE() {}
};

class Char : public RE {
public:
  explicit Char(char c) : c_(c) {}

  virtual REType GetType() { return REType::kChar; }

  char c() const { return c_; }

private:
  char c_;
};

class Concat : public RE {
public:
  explicit Concat(const std::vector<RE *> &ops) : ops_(ops) {}

  REType GetType() { return REType::kConcat; }

  const std::vector<RE *> &ops() const { return ops_; }

private:
  std::vector<RE *> ops_;
};

class Branch : public RE {
public:
  explicit Branch(const std::vector<RE *> &ops) : ops_(ops) {}

  REType GetType() { return REType::kBranch; }

  const std::vector<RE *> &ops() const { return ops_; }

private:
  std::vector<RE *> ops_;
};

class Star : public RE {
public:
  explicit Star(RE *op) : op_(op) {}

  REType GetType() { return REType::kStar; }

  RE *op() { return op_; }

private:
  RE *op_;
};

class Plus : public RE {
public:
  explicit Plus(RE *op) : op_(op) {}

  REType GetType() { return REType::kPlus; }

  RE *op() { return op_; }

private:
  RE *op_;
};

class Question : public RE {
public:
  explicit Question(RE *op) : op_(op) {}

  REType GetType() { return REType::kQuestion; }

  RE *op() { return op_; }

private:
  RE *op_;
};

inline std::string RE::to_string(RE *re) {
  std::string result;
  switch (re->GetType()) {
  case REType::kChar: {
    Char *c = dynamic_cast<Char *>(re);
    if (c->c() != EP) {
      result.push_back(c->c());
    }
    break;
  }
  case REType::kConcat: {
    Concat *concat = dynamic_cast<Concat *>(re);
    for (auto re : concat->ops()) {
      result.append(to_string(re));
    }
    break;
  }
  case REType::kBranch: {
    Branch *branch = dynamic_cast<Branch *>(re);
    for (size_t i = 0; i < branch->ops().size(); ++i) {
      auto re = branch->ops().at(i);
      result.append(to_string(re));
      if (i != branch->ops().size() - 1) {
        result.append("|");
      }
    }
    break;
  }
  case REType::kPlus: {
    Plus *plus = dynamic_cast<Plus *>(re);
    result.append("(");
    result.append(to_string(plus->op()));
    result.append(")");
    result.append("+");
    break;
  }
  case REType::kStar: {
    Star *star = dynamic_cast<Star *>(re);
    result.append("(");
    result.append(to_string(star->op()));
    result.append(")");
    result.append("*");
    break;
  }
  case REType::kQuestion: {
    Question *question = dynamic_cast<Question *>(re);
    result.append("(");
    result.append(to_string(question->op()));
    result.append(")");
    result.append("?");
    break;
  }
  }
  return result;
}

class REAllocator {
public:
  template <typename T, typename... Args>
  T *Create(Args &&... args) {
    T *re = new T(std::forward<Args>(args)...);
    alloc_.emplace_back(re);
    return re;
  }

  ~REAllocator() {
    for (auto re : alloc_) {
      delete re;
    }
  }

private:
  std::vector<RE *> alloc_;
};

} // namespace re6
