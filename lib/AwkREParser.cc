// Copyright (c) 2018 Kai Luo <gluokai@gmail.com>. All rights reserved.

#include "re6/AwkREParser.h"

#include <iostream>

namespace re6 {

AwkREParser::ParseResult AwkREParser::Parse() {
  ParseResult result{
      nullptr,
      nullptr,
  };
  std::vector<RE *> branches;
  ParseBranch(&branches);
  result.re = allocator_->Create<Branch>(branches);
  result.allocator = std::move(allocator_);
  return std::move(result);
}

bool AwkREParser::ParseBranch(std::vector<RE *> *branches) {
  // std::cout << __FUNCTION__ << " " << src_.to_string() << std::endl;
  if (src_.empty()) {
    return true;
  }
  char c = src_[0];
  if (c == '|') {
    src_ = src_.advance();
    return ParseBranch(branches);
  } else if (c == ')') {
    return true;
  }
  std::vector<RE *> concat;
  ParseConcat(&concat);
  branches->emplace_back(allocator_->Create<Concat>(concat));
  return ParseBranch(branches);
}

bool AwkREParser::ParseConcat(std::vector<RE *> *concat) {
  // std::cout << __FUNCTION__ << " " << src_.to_string() << std::endl;
  if (src_.empty()) {
    return true;
  }
  char c = src_[0];
  if (c == '|') {
    return true;
  } else if (c == '(') {
    RE *re = ParseBracket();
    if (re != nullptr) {
      concat->emplace_back(re);
    }
    return ParseConcat(concat);
  } else if (c == ')') {
    return false;
  }
  if (c == '?') {
    if (not concat->empty()) {
      RE *re = concat->back();
      concat->pop_back();
      concat->emplace_back(allocator_->Create<Question>(re));
    }
  } else if (c == '*') {
    if (not concat->empty()) {
      RE *re = concat->back();
      concat->pop_back();
      concat->emplace_back(allocator_->Create<Star>(re));
    }
  } else if (c == '+') {
    if (not concat->empty()) {
      RE *re = concat->back();
      concat->pop_back();
      concat->emplace_back(allocator_->Create<Plus>(re));
    }
  } else {
    concat->emplace_back(allocator_->Create<Char>(c));
  }
  src_ = src_.advance();
  // std::cout << __FUNCTION__ << ": " << src_.to_string() << std::endl;
  return ParseConcat(concat);
}

RE *AwkREParser::ParseBracket() {
  // std::cout << __FUNCTION__ << " " << src_.to_string() << std::endl;
  if (src_.empty()) {
    return nullptr;
  }
  char c = src_[0];
  if (c != '(') {
    return nullptr;
  }
  src_ = src_.advance();
  std::vector<RE *> branches;
  ParseBranch(&branches);
  if (src_.empty()) {
    return nullptr;
  }
  c = src_[0];
  if (c != ')') {
    return nullptr;
  }
  src_ = src_.advance();
  return allocator_->Create<Branch>(branches);
}

} // namespace re6
