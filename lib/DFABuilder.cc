// Copyright (c) 2018 Kai Luo <gluokai@gmail.com>. All rights reserved.

#include "re6/DFABuilder.h"
#include "re6/xxhash.h"

namespace re6 {

void DFABuilder::Build() {
  result_.start = alloc_->Create();
  // TODO
}

DFAState *DFABuilder::GetOrCreateState(uint64_t hash) {
  auto it = states_map_.find(hash);
  if (it != states_map_.end()) {
    return it->second;
  }
  DFAState *state = alloc_->Create();
  states_map_.insert(std::make_pair(hash, state));
  return state;
}

uint64_t DFABuilder::EncodeNFASet(const std::set<NFAState *> &nfa_set) {
  std::string temp;
  for (auto p : nfa_set) {
    temp.append(reinterpret_cast<const char *>(p), sizeof(p));
  }
  return XXH64(temp.data(), temp.size(), 0);
}

DFAState *DFABuilder::GetOrCreateState(const std::set<NFAState *> &nfa_set) {
  return GetOrCreateState(EncodeNFASet(nfa_set));
}

} // namespace re6
