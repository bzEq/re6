// Copyright (c) 2018 Kai Luo <gluokai@gmail.com>. All rights reserved.

#pragma once

#include "re6/DFA.h"
#include "re6/NFA.h"

#include <set>

namespace re6 {

class DFABuilder {
public:
  struct Result {
    DFAState *start;
    std::set<DFAState *> finish_set;
  };

public:
  static uint64_t EncodeNFASet(const std::set<NFAState *> &nfa_set);

  explicit DFABuilder(DFAStateAllocator *alloc, NFAState *start,
                      NFAState *finish)
      : alloc_(alloc), nfa_start_(start), nfa_finish_(finish), result_{} {}

  void Build();

  Result result() const { return result_; }

private:
  DFAStateAllocator *alloc_;
  NFAState *nfa_start_, *nfa_finish_;
  Result result_;
  std::map<uint64_t, DFAState *> states_map_;

private:
  DFAState *GetOrCreateState(const std::set<NFAState *> &nfa_set);
  DFAState *GetOrCreateState(uint64_t hash);
};

} // namespace re6
