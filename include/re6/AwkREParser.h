// Copyright (c) 2018 Kai Luo <gluokai@gmail.com>. All rights reserved.

#pragma once

#include "re6/re.h"
#include "re6/slice.h"

#include <iostream>
#include <memory>

namespace re6 {

class AwkREParser {
public:
  struct ParseResult {
    RE *re;
    std::unique_ptr<REAllocator> allocator;
  };

public:
  explicit AwkREParser(Slice src) : allocator_(new REAllocator), src_(src) {}

  ParseResult Parse();

private:
  bool ParseBranch(std::vector<RE *> *branches);
  bool ParseConcat(std::vector<RE *> *concat);
  RE *ParseBracket();

private:
  std::unique_ptr<REAllocator> allocator_;
  Slice src_;
};

} // namespace re6
